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

| Name         | Value | Direction         | Description                |
|--------------|-------|-------------------|----------------------------|
| CONNECT      | 1     | Client → Server   | Request connection         |
| INPUT        | 2     | Client → Server   | Player input               |
| PING         | 3     | Client → Server   | Keepalive                  |
| DISCONNECT   | 4     | Client → Server   | Disconnect request         |
| CREATE_LOBBY | 5     | Client → Server   | Create a new game lobby    |
| JOIN_LOBBY   | 6     | Client → Server   | Join an existing lobby     |
| START_GAME   | 7     | Client → Server   | Start game in lobby        |
| LOBBY_STATE  | 8     | Client → Server   | Request lobby information  |
| CONNECT_ACK  | 101   | Server → Client   | Connection accepted        |
| GAME_STATE   | 102   | Server → Client   | Game state update          |
| PONG         | 103   | Server → Client   | Keepalive response         |
| LOBBY_INFO   | 104   | Server → Client   | Lobby creation/join info   |
| SPAWN_ENTITY | 105   | Server → Client   | Spawn game entity          |
| ROLLBACK     | 106   | Server → Client   | Rollback game state        |

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

### LOBBY_INFO
- **Direction**: Server → Client
- **Payload**:
  - uint32: Lobby ID (0 = not in lobby or operation failed)
  - uint8: Lobby state (0=WAITING, 1=RUNNING, 2=FINISHED)

### SPAWN_ENTITY
- **Direction**: Server → Client
- **Payload**: Entity spawn data (TBD - depends on ECS implementation)

### ROLLBACK
- **Direction**: Server → Client
- **Payload**: Rollback state data (TBD - depends on game state)

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
