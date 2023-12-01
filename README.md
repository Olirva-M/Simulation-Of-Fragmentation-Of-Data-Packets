
# Identifying the first and last fragments of TCP segments

### About
The project aims to develop a C program for identifying the first and last fragments of TCP segments within network traffic, a critical function in the context of IP fragmentation. Here, large data packets are divided into smaller segments to fit within the network's Maximum Transmission Unit (MTU) requirements. This tool is essential for efficient and secure network communication.

### Packages
Standard C libraries: math.h, netinet/in.h, stdio.h, stdlib.h, string.h, sys/socket.h, sys/types.h, unistd.h, time.h.

### Usage

- The server binds to a UDP socket on port 7428.
- It receives a message from the client to establish a connection.
- The server prompts the user to enter the Maximum Segment Size (MSS) and data size.
- The data is fragmented using the MSS, and fragments are sent back to the client.
- The client recieves information about each fragment, including packet ID, fragment number, size, offset, and flags (D and M).
- Using M flag and offset, client identifies the first and the last fragment.
- When fragmentation is not needed, client identifies this and prints an appropriate message.

## Compilation and Execution

Runs on linux environment.

```bash
gcc Server.c -o Server
```

```bash
gcc Client.c -o Client
```

```bash
./Server
```

```bash
./Client
```
