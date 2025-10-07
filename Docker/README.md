# R-Type Docker Setup

This directory contains Docker configuration files for containerizing the R-Type game client and server.

## Architecture

- **rtype-server**: Lightweight container running the game server on UDP port 4242
- **rtype-client**: GUI container with SDL2 dependencies running the game client
- **Custom network**: Isolated Docker network for secure inter-container communication

## Prerequisites

- Docker Engine 20.10+
- Docker Compose 2.0+
- At least 4GB RAM available for containers

## Quick Start

1. **Build and run the entire stack:**
   ```bash
   cd Docker
   docker-compose up --build
   ```

2. **Run in detached mode:**
   ```bash
   docker-compose up -d --build
   ```

3. **View logs:**
   ```bash
   docker-compose logs -f
   ```

4. **Stop the stack:**
   ```bash
   docker-compose down
   ```

## Individual Components

### Server Only
```bash
docker build -f Dockerfile.server -t rtype-server ..
docker run -p 4242:4242/udp --name rtype-server rtype-server
```

### Client Only
```bash
docker build -f Dockerfile.client -t rtype-client ..
docker run -p 4243:4243/udp --name rtype-client rtype-client
```

## Networking

- **Server Port**: 4242/UDP (exposed to host)
- **Client Port**: 4243/UDP (exposed to host)
- **Internal Network**: 172.20.0.0/16 subnet

## GUI Client Considerations

The client container includes SDL2 libraries but may require additional setup for GUI display:

### Linux with X11
```bash
# Allow Docker to connect to X server
xhost +local:docker

# Run with X11 forwarding
docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix rtype-client
```

### macOS
```bash
# Install XQuartz and allow connections
# Run with X11 forwarding (requires socat)
docker run -e DISPLAY=host.docker.internal:0 rtype-client
```

### Windows
```bash
# Use VcXsrv or MobaXterm for X11 server
# Run with appropriate DISPLAY environment variable
```

## Development

### Volume Mounting (for development)
Uncomment the volumes section in `docker-compose.yml` and modify for your needs:

```yaml
volumes:
  - ../client:/app/client
  - ../server:/app/server
  - ../shared:/app/shared
  - ./build-cache:/app/build
```

### Rebuilding
```bash
# Rebuild specific service
docker-compose build rtype-server

# Rebuild all services
docker-compose build --no-cache
```

## Troubleshooting

### Build Issues
- Ensure Docker has sufficient memory (4GB+ recommended)
- Check that all dependencies are available in Ubuntu 22.04
- Verify network connectivity for CPM.cmake downloads

### Runtime Issues
- **Port conflicts**: Change exposed ports in docker-compose.yml
- **GUI not displaying**: Check X11 forwarding setup
- **Network issues**: Verify firewall settings for UDP ports

### Logs and Debugging
```bash
# View all logs
docker-compose logs

# View specific service logs
docker-compose logs rtype-server

# Enter running container
docker exec -it rtype-server /bin/bash
```

## Configuration

### Environment Variables
Add environment variables in docker-compose.yml:

```yaml
environment:
  - SERVER_HOST=rtype-server
  - SERVER_PORT=4242
  - LOG_LEVEL=INFO
```

### Scaling
To run multiple clients, uncomment the additional client service in docker-compose.yml and adjust ports accordingly.

## Security Notes

- Containers run as non-root user `rtype`
- Network isolation using custom Docker network
- Minimal attack surface with multi-stage builds
- No privileged containers required
