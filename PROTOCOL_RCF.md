# R-Type UDP Binary Protocol (RCF Documentation)

## 1. Overview

This protocol defines the structure and encoding for all messages exchanged
between server and clients in the r-type game. It is designed for UDP, binary
format, and easy implementation in any language.

## 2. Packet Structure

Each packet consists of a fixed-size header followed by a variable-size
payload.

| Field           | Type      | Size (bytes) | Description                  |
|-----------------|-----------|--------------|------------------------------|
| Message Type    | uint8     | 1            | Enum value (see below)       |
| Version         | uint8     | 1            | Protocol version             |
| Sequence Number | uint16    | 2            | For ordering/reliability     |
| Player ID       | uint32    | 4            | Unique client/server ID      |
| Payload Length  | uint16    | 2            | Length of payload in bytes   |
| Payload         | bytes     | variable     | Message-specific data        |

All multi-byte fields are encoded in **network byte order** (big-endian).

## 3. Message Types

| Name         | Value | Direction         | Description                               |
|--------------|-------|-------------------|-------------------------------------------|
| CONNECT      | 1     | Client → Server   | Request connection                        |
| INPUT        | 2     | Client → Server   | Player input                              |
| PING         | 3     | Client → Server   | Keepalive                                 |
| DISCONNECT   | 4     | Client → Server   | Disconnect request                        |
| CREATE_LOBBY | 5     | Client → Server   | Create a new game lobby                   |
| JOIN_LOBBY   | 6     | Client → Server   | Join an existing lobby                    |
| START_GAME   | 7     | Client → Server   | Start game in lobby                       |
| LOBBY_STATE  | 8     | Client → Server   | Request lobby information                 |
| SET_USERNAME | 9     | Client → Server   | Set or update player username             |
| KICK_PLAYER  | 10    | Client → Server   | Player: kick player from lobby            |
| CONNECT_ACK  | 101   | Server → Client   | Connection accepted                       |
| GAME_STATE   | 102   | Server → Client   | Game state update                         |
| PONG         | 103   | Server → Client   | Keepalive response                        |
| LOBBY_INFO   | 104   | Server → Client   | Lobby creation/join info                  |
| SPAWN_ENTITY | 105   | Server → Client   | Spawn game entity                         |
| ROLLBACK     | 106   | Server → Client   | Rollback game state                       |
| USERNAME_ACK | 107   | Server → Client   | Username set/updated confirmation         |
| KICK_ACK     | 108   | Server → Client   | Acknowledge kick request                  |
| KICK_NOTICE  | 109   | Server → Client   | Notification sent to players in the lobby |

## 4. Encoding Rules

- All fields are packed with no padding.
- Strings are encoded as length-prefixed UTF-8.
- Payload layout depends on message type (see examples).
- All multi-byte numbers use big-endian.

## 5. Reliability & Data Loss Handling

- Sequence Number is incremented for each message.
- Critical messages (CONNECT, DISCONNECT) should be acknowledged with a response (CONNECT_ACK, DISCONNECT_NOTICE).
- Retransmit unacknowledged critical messages after a timeout (e.g., 100ms).
- Game state and input messages are sent frequently; missing packets can be ignored or interpolated.
- Clients and servers should track the last received sequence number to detect loss or out-of-order delivery.
- Optionally, implement a simple window for reordering or duplicate suppression.

## 6. Extensibility Guidelines

- Add new message types by extending the MessageType enum.
- Reserve unused values for future features.
- Use the Version field to signal protocol changes; clients/servers should reject incompatible versions.
- Document new payload layouts for each message type.
- Keep header format unchanged for backward compatibility.
- For new features, prefer optional fields in the payload rather than changing the header.

## 7. Payload Layouts

### CREATE_LOBBY
- **Direction**: Client → Server
- **Payload**: Empty (0 bytes)
- **Response**: LOBBY_INFO with lobby ID

### JOIN_LOBBY
- **Direction**: Client → Server
- **Payload**: uint32 (lobby ID to join)
- **Response**: LOBBY_INFO with lobby ID (0 if join failed)

### START_GAME
- **Direction**: Client → Server
- **Payload**: Empty (0 bytes)
- **Response**: None (game starts if successful)

### LOBBY_STATE
- **Direction**: Client → Server
- **Payload**: Empty (0 bytes)
- **Response**: LOBBY_INFO with current lobby state

### SET_USERNAME
- **Direction**: Client → Server
- **Payload**: String
- **Response**: USERNAME_ACK with the request state

### INPUT
- **Direction**: Client → Server
- **Payload**:
  - uint32: Tick number (client's current tick)
  - uint8: Number of inputs in this message
  - For each input:
    - uint8: GameInput enum value (0=UP/jump, 1=DOWN, 2=LEFT, 3=RIGHT, 4=ATTACK, 5=DASH)
    - uint8: Input state (0=released, 1=pressed)
- **Response**: None (inputs are processed server-side)

### LOBBY_INFO
- **Direction**: Server → Client
- **Payload**:
  - uint32: Lobby ID (0 = not in lobby or operation failed)
  - uint8: Lobby state (0=WAITING, 1=RUNNING, 2=FINISHED)
  - uint32: Owner Player ID
  - uint8: Number of players (N)
  - For i in 0..N-1:
    - uint32: Player ID
    - uint8: Username length (L)
    - bytes[L]: Username (UTF-8)

### KICK_PLAYER
- **Direction**: Client → Server
- **When to use**: Sent by the lobby owner to request removal of a player.
- **Payload**:
  - uint32: Lobby ID
  - uint32: Target Player ID (the player to remove)
- **Response**: server MUST send KICK_ACK in response.

### SPAWN_ENTITY
- **Direction**: Server → Client
- **Payload**:
  - uint32: Entity ID (unique identifier)
  - uint8: Entity Type (0=Player, 1=Projectile, 2=Platform, 3=Enemy)
  - Entity-specific component data (see below)

**Entity Type Data Formats:**

**All Entity Types:**
- Position: `float x, float y` (network byte order)

**Player (Type 0):**
- Health: `int32 health`
- Hitbox: `float width, float height, float offset_x, float offset_y`
- PlayerID: `uint32 player_id` (for client to identify which player is theirs)
- Username length: `uint32 length`
- Username: `String`

**Projectile (Type 1):**
- Velocity: `float vx, float vy`
- Damage: `float damage`
- Hitbox: `float width, float height, float offset_x, float offset_y`
- Parent Entity: `uint32 parent_entity_id, uint32 parent_entity_version` (weapon entity that spawned this projectile)
- Lifetime: `float lifetime_seconds`

**Enemy (Type 3):**
- Health: `int32 health`
- Hitbox: `float width, float height, float offset_x, float offset_y`

### GAME_STATE
- **Direction**: Server → Client
- **Payload**:
  - uint32: Current tick number
  - uint8: Number of players in this update
  - For each player:
    - uint32: Entity ID
    - float: Position X
    - float: Position Y
    - uint32: Health value

### ROLLBACK
- **Direction**: Server → Client
- **Payload**: Rollback state data (TBD - depends on game state)

### USERNAME_ACK
- **Direction**: Server → Client
- **Payload**:
  - uint8: State of the request (0 = rejected, 1 = accepted)

### KICK_ACK
- **Direction**: Server → Client (owner/requester)
- **When to use**: Acknowledge the KICK_PLAYER request and indicate success or failure.
- **Payload**:
  - uint32: Lobby ID
  - uint32: Target Player ID
  - uint8: Status (0 = failure, 1 = success)
- Notes:
  - On success, server also sends KICK_NOTICE to all players in the lobby.

### KICK_NOTICE
- **Direction**: Server → Client (kicked player)
- **When to use**: Notify the players including the kicked player they were removed.
- **Payload**:
  - uint32: Lobby ID
  - uint32: Kicked Player ID
  - uint32: Requester Player ID (the owner who issued the kick)


## 8. Example Packet (CONNECT)

| Field           | Example Value | Hex Representation      |
|-----------------|--------------|-------------------------|
| Message Type    | 1            | 01                      |
| Version         | 1            | 01                      |
| Sequence Number | 42           | 00 2A                   |
| Player ID       | 12345678     | 00 BC 61 4E             |
| Payload Length  | 0            | 00 00                   |
| Payload         | -            |                         |

Full packet (hex): `01 01 00 2A 00 BC 61 4E 00 00`

## 8. Implementation Notes

- See Message.hpp/Message.cpp for C++ reference implementation.
- For other languages, follow the field order and encoding rules above.
- Always check payload length and sequence number for reliability.
- Use UDP sockets and handle retransmission logic in application code.
