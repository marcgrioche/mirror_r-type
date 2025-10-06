# Comparative Study — Multiplayer Game (RTYPE)

## 1. Executive Summary

This document presents a comparative study of the technologies, techniques, algorithms, data structures, storage methods, and security mechanisms used in our **multiplayer game** project.

**Project context:**
- **Language:** C++
- **Graphics Library:** SDL2
- **Architecture:** Entity Component System (ECS)
- **Networking:** Custom UDP-based implementation using system libraries
- **Protocol:** Binary-based (compact and efficient)
- **Programming Paradigm:** Object-Oriented Programming (OOP)

**Goals of this study:**
- Justify the technological and architectural decisions made.
- Compare alternative techniques and evaluate trade-offs.
- Ensure that performance, scalability, and security are properly balanced.

---

## 2. Evaluation Criteria

To objectively assess the design and implementation quality of our multiplayer game, several evaluation criteria were defined. These criteria focus on performance, architecture, reliability, and maintainability. Each aspect is evaluated based on the technologies, algorithms, and design techniques employed.

### 2.1. Performance and Efficiency
- **Low latency** communication achieved through a custom UDP binary-based protocol.
- **Optimized rendering** using hardware-accelerated SDL2.
- **Efficient data structures** such as:
    - `std::vector` for contiguous memory and fast iteration.
    - `std::unordered_map` / `std::map` for quick entity or player lookups.
    - `std::queue` and `std::deque` for message buffering and event queues.
- **Use of RAII** (Resource Acquisition Is Initialization) and smart pointers (`std::unique_ptr`, `std::shared_ptr`) to ensure automatic resource management and prevent memory leaks.

### 2.2. Architecture and Modularity
- **Entity Component System (ECS)** architecture to separate data from behavior.
- **Object-Oriented Programming (OOP)** for high-level abstractions (Game, Player, NetworkManager, Renderer).
- **Layered structure** (network, logic, rendering) promoting modularity and scalability.
- **Reusability** of components through C++ class hierarchies and polymorphism.

### 2.3. Network Reliability and Scalability
- Custom **UDP protocol** with optional **ACK/NACK** for selective reliability.
- **Binary message format** for compact communication.
- **Sequence numbers** and **timestamps** for synchronization.
- Designed for scalability across multiple clients with minimal overhead.

### 2.4. Graphics and Rendering
- **SDL2** used for window creation, input handling, and rendering.
- **Rationale for SDL2 choice:**
    - Lightweight and cross-platform.
    - Offers low-level access suitable for a custom ECS render system.
    - Efficient for 2D graphics while still supporting OpenGL integration.
- Compared to **SFML**, **Raylib**, and **Vulkan**, SDL2 provides the optimal balance between performance and simplicity.

### 2.5. Data Management and Storage
- In-memory data structures (vectors, maps) used for fast runtime access.
- Persistent data (if any) can be stored in binary or JSON format for simplicity.
- Use of **smart pointers** ensures proper memory ownership semantics.
- Focus on **low latency access** and **minimal serialization overhead**.

### 2.6. Security and Robustness
- Validation of incoming UDP packets (checksum, sequence validation).
- Input sanitization and bounds checking to prevent buffer overflows.
- Limited use of external libraries reduces dependency-based vulnerabilities.
- RAII and exception-safe code improve stability under failure conditions.

### 2.7. Development Tools and Libraries
| Category            | Technology                                     | Purpose                                      |
|---------------------|------------------------------------------------|----------------------------------------------|
| **Language**        | C++                                            | Core game logic and systems programming      |
| **Graphics**        | SDL2                                           | Rendering, window management, input handling |
| **Data Structures** | `vector`, `map`, `unordered_map`, `unique_ptr` | Efficient entity and state management        |
| **Networking**      | BSD sockets (UDP)                              | Low-level control, custom binary protocol    |
| **Architecture**    | ECS, OOP                                       | Separation of concerns, flexibility          |
| **Build System**    | CMake                                          | Cross-platform project configuration         |
| **Debugging Tools** | Valgrind, GDB                                  | Memory leak detection and runtime debugging  |

---

**Summary:**
> The evaluation focuses on ensuring high performance, modular design, and safe resource management.  
> By combining modern C++ practices (RAII, smart pointers) with efficient data structures and a custom UDP system, the project achieves both speed and reliability.


---

## 3. Networking and Communication

### Comparison of Transport Protocols

| Protocol                     | Latency                      | Reliability           | Complexity | Typical Usage                          |
|------------------------------|------------------------------|-----------------------|------------|----------------------------------------|
| **TCP**                      | High (head-of-line blocking) | Reliable, ordered     | Simple     | Chat, authentication, configuration    |
| **UDP**                      | Very low                     | Unreliable by default | Simple     | Real-time gameplay, positions, actions |
| **UDP + Custom Reliability** | Low                          | Selective reliability | Moderate   | Hybrid real-time systems (like ours)   |

### Chosen Solution: **UDP + Custom Binary Protocol**

We implemented our own binary-based protocol on top of **UDP** for the following reasons:

- **Low latency:** Essential for real-time responsiveness in a multiplayer environment.
- **Compact data:** Binary format reduces bandwidth usage compared to text formats.
- **Flexibility:** We designed custom packet headers with sequence numbers and checksums.
- **Control:** We can choose which messages require acknowledgment (reliable) and which can be dropped (unreliable).

### Technical Characteristics

- **Non-blocking sockets** using system libraries (`poll`).
- **Custom packet structure** containing:
    - Message Type
    - Version
    - Sequence number
    - Player ID
    - Payload length
    - Payload
- **ACK/NACK system** for reliability when needed.
- **Message aggregation** for small updates to reduce overhead.

---

## 4. Serialization and Data Format

| Format                        | Size         | Speed     | Readability | Suitability                    |
|-------------------------------|--------------|-----------|-------------|--------------------------------|
| **Binary (Custom)**           | Very compact | Very fast | Low         | ✔ Used in our project          |
| **Protocol Buffers**          | Compact      | Good      | Moderate    | Optional alternative           |
| **FlatBuffers / Cap’n Proto** | Compact      | Excellent | Moderate    | Advanced alternative           |
| **JSON / Text**               | Large        | Slow      | High        | Not suitable for real-time use |

**Chosen Format: Custom Binary Serialization**

**Justification:**
- Fast encoding/decoding for real-time communication.
- No third-party dependencies.
- Deterministic layout with fixed-size headers.
- Minimal bandwidth consumption.

---

## 5. Choice of Programming Language and Graphics Library

### 5.1 Programming Language Comparison

| Language                    | Performance | Control                       | Ease of Use | Ecosystem                | Typical Use in Games                |
|-----------------------------|-------------|-------------------------------|-------------|--------------------------|-------------------------------------|
| **C++**                     | ⭐⭐⭐⭐⭐       | Full memory & CPU control     | Moderate    | Vast (engines, libs)     | AAA engines (Unreal, Unity backend) |
| **C#**                      | ⭐⭐⭐         | Managed memory, less control  | High        | Strong (Unity, MonoGame) | Indie & mid-sized projects          |
| **Python**                  | ⭐⭐          | High-level, interpreted       | Very easy   | Huge (scripting, tools)  | Prototyping, scripting AI           |
| **Rust**                    | ⭐⭐⭐⭐        | Safe, modern systems language | Moderate    | Growing                  | Experimental engines, tools         |
| **JavaScript / TypeScript** | ⭐⭐          | Managed by browser/engine     | High        | Large (Web games)        | Web & casual games                  |

**Chosen Language:** **C++**

**Justification:**
- **Performance-critical:** Our game requires fast, real-time updates with minimal latency.
- **Memory control:** C++ allows fine-grained control over allocations — crucial for ECS and network buffers.
- **Deterministic performance:** No garbage collection pauses.
- **Industry standard:** Most professional engines (Unreal Engine, CryEngine, Source) are C++-based.
- **Compatibility:** SDL2 and system-level APIs integrate naturally with C++.

**Advantages of C++ for Our Project**
- Zero-cost abstractions (templates, inline functions).
- Cross-platform compilation (Linux, Windows).
- Direct integration with OS networking and SDL2.
- Mature ecosystem for systems-level tasks (threads, sockets, ECS).

**Limitations**
- Manual memory management requires caution (hence use of `unique_ptr`, RAII).
- Longer compile times.
- Steeper learning curve compared to managed languages.

---

### 5.2 Comparison of Graphics Libraries

| Library    | Level      | Performance | API Style     | Features                       | Difficulty | Suitable For                         |
|------------|------------|-------------|---------------|--------------------------------|------------|--------------------------------------|
| **SDL2**   | Mid-level  | High        | C-style API   | Windowing, input, 2D rendering | Moderate   | Cross-platform 2D & low-level access |
| **SFML**   | Mid-level  | High        | C++ OOP API   | Audio, graphics, networking    | Easy       | Rapid prototyping, 2D/3D basics      |
| **Raylib** | High-level | Moderate    | Simple C API  | Beginner-friendly 2D/3D        | Very easy  | Education, lightweight projects      |
| **Vulkan** | Low-level  | Very High   | Explicit API  | Full GPU control, 3D           | Very hard  | AAA engines, 3D rendering backends   |
| **OpenGL** | Mid-level  | High        | State machine | Hardware-accelerated 2D/3D     | Hard       | 3D and advanced 2D rendering         |

**Chosen Library:** **SDL2 (Simple DirectMedia Layer 2)**

**Reasons for Choosing SDL2:**
- **Cross-platform:** Works on Linux, Windows, macOS, and even embedded systems.
- **Low-level access:** Provides precise control over input, rendering, and window management.
- **Lightweight:** Ideal for building custom engines or ECS-based architectures.
- **Integration:** Well-suited for integrating with C++ and custom render loops.
- **Performance:** Uses hardware-accelerated 2D rendering (via OpenGL or Direct3D backends).
- **Networking compatibility:** Plays well with system-level sockets (our UDP implementation).

---

## 6. Entity Component System (ECS)

### ECS Overview

We use an **Entity Component System** architecture to manage game entities efficiently.

| Aspect        | Description                                                |
|---------------|------------------------------------------------------------|
| **Entity**    | Unique ID representing an object in the world.             |
| **Component** | Plain data (e.g., Position, Velocity).                     |
| **System**    | Logic operating on entities that have specific components. |

### ECS Storage Techniques

| Pattern                       | Pros                                  | Cons                   |
|-------------------------------|---------------------------------------|------------------------|
| **Array of Components (SoA)** | Cache-friendly, fast iteration        | Requires index mapping |
| **Sparse Set**                | Flexible, efficient for dynamic data  | Slightly complex       |
| **Archetype ECS**             | High performance for grouped entities | Complex implementation |

**Chosen Solution:** **Sparse Set ECS**

**Reasons:**
- Fast iteration and dynamic addition/removal of components.
- Cache-friendly access via `std::vector` storage.
- Simple entity-to-index mapping via `std::unordered_map`.

---

## 7. Spatial Partitioning and Collision

| Technique           | Query Cost       | Update Cost | Suitable For                  |
|---------------------|------------------|-------------|-------------------------------|
| **Uniform Grid**    | O(nearby cells)  | O(1)        | Many moving entities, 2D maps |
| **Quadtree**        | Logarithmic      | Moderate    | Sparse environments           |
| **BVH**             | High             | High        | Static geometry               |
| **Spatial Hashing** | Constant average | Low         | Dynamic 2D gameplay           |

**Chosen Solution:** **Spatial Hashing**

**Reasoning:**
- Low computational cost.
- Works well with frequently moving entities.
- Easy to implement alongside ECS.
- Adapts well to 2D environments such as our SDL2 game map.

---

## 8. Client Prediction and Server Reconciliation

### Overview

Real-time multiplayer games must minimize the effects of **latency** while keeping the game state **authoritative and consistent**.  
To achieve this, our implementation combines **client-side prediction**, **server authority**, and **reconciliation**.

### Techniques Used

1. **Client-Side Prediction**
    - The client immediately simulates movement when the player provides input.
    - This prevents visible lag between keypress and on-screen movement.

2. **Server Authority**
    - The server is the single source of truth for the game state.
    - It validates every movement, position, and action to prevent cheating.

3. **Reconciliation**
    - When the server sends back the authoritative state, the client corrects its local simulation.
    - The client replays unacknowledged inputs since the last confirmed state to stay synchronized.

### Advantages

- Reduced input latency for the player.
- Smooth, real-time feedback.
- Server ensures fairness and prevents state divergence.

### Drawbacks

- Requires precise timing and rollback management.
- Poor handling can lead to visible "snapping" or jitter.

### Mitigation for Desynchronization:

- Linear interpolation between old and new states.
- Smoothing corrections to avoid abrupt movement.

---

## 9. Concurrency and Server Thread Model

### Overview

Our game server uses multi-threading to handle multiple lobbies efficiently.
By leveraging parallelism, the server can process incoming packets, update game logic, and send responses concurrently without bottlenecks.

### Models Compared


| Model                    | Description                                              | Pros                           | Cons                              |
|--------------------------|----------------------------------------------------------|--------------------------------|-----------------------------------|
| **Single-threaded loop** | One main loop handles everything                         | Simple, deterministic          | Limited scalability               |
| **Threaded pool**        | Fixed number of worker threads handle tasks              | Parallelism, efficient CPU use | Requires synchronisation          |
| **Shared instances**     | Each lobby has its own thread with its own game instance | Scalable, isolated states      | Complex inter-shard communication |

**Chosen Model:** **Thread pool**

**Architecture:**
- **One I/O thread** handles UDP communication (receiving and sending packets).
- **Multiple worker threads** execute game logic, ECS updates, and message processing.
- Tasks are distributed dynamically to avoid CPU idling.

### Synchronization
- **Mutexes** protect shared data (e.g., player states, entity maps).
- **Atomic variables** used for counters and flags.
- ECS systems are designed to operate on independent component sets when possible.

### Advantages

- Efficient utilization of multi-core CPUs.
- Reduced response time under heavy load.
- Balanced performance between I/O and simulation.

### Limitations

- Synchronization adds complexity.
- Potential for race conditions if locks are misused.
- Debugging multi-threaded issues can be more difficult.

### Summary

| **Component**      | **Thread Responsibility**            | **Benefits**         |
|--------------------|--------------------------------------|----------------------|
| **Network Thread** | Handle incoming/outgoing UDP packets | Low latency I/O      |
| **Worker Threads** | Run ECS updates and game logic       | Parallel computation |

---

## 10. Memory Management and Storage

### In-Memory Structures

| **Purpose**       | **Data Structure**   | **Justification**           |
|-------------------|----------------------|-----------------------------|
| Component Storage | `std::vector`        | Contiguous, cache-friendly  |
| Entity mapping    | `std::unordered_map` | O(1) lookup                 |
| Ownership         | `std::unique_ptr`    | Safe RAII ownership         |
| Temporary Objects | Object Pool          | Avoids frequent allocations |

### Persistent Storage

Not yet implemented.

---

## 11. Security and Anti-Cheat

### Security Measures

| **Threat**                  | **Mitigation**                      |
|-----------------------------|-------------------------------------|
| Unauthorised access         | Token-based authentication          |
| Cheating (e.g, speed hacks) | Server validation of all actions    |
| Flooding or spam            | Rate limiting and connection quotas |

### Decision principles

- **Server Authoritative Model:** The server always validates actions.
- **No trusted client state:** Clients send only inputs, not results.
- **Optional Encryption:** Can add HMAC or lightweight encryption to binary packets.

---

## 12. Decision Matrix

| **Option**                  | **Latency** | **Reliability** | **Complexity** | **Security** | **Scalability** | **Score** |
|-----------------------------|-------------|-----------------|----------------|--------------|-----------------|-----------|
| **TCP**                     | 2           | 5               | 5              | 5            | 4               | 3.9       |
| **UDP (ours)**              | 5           | 3               | 3              | 4            | 5               | 4.3       |
| **UDP + Reliability Layer** | 5           | 4               | 4              | 4            | 5               | 4.6       |

**Chosen: UDP + Custom Reliability Layer**
Because it provides the best compromise between latency, scalability, and control.

