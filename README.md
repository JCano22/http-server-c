# TCP Server From Scratch in C

## Overview

This project is a **learning-focused implementation of a basic TCP server written in C**, built from scratch using POSIX sockets.

The goal of this project is **not** to build a full web server, but to understand the **core networking primitives** that all networked applications rely on — including web servers, APIs, and distributed systems.

By writing the server at the socket level, this project explores how programs communicate over a network using the operating system’s networking stack.

---

## Why This Project Exists

Most modern applications use high-level frameworks that hide networking details. This project intentionally avoids those abstractions to focus on:

- How a process becomes a network server
- How sockets work at the OS level
- The lifecycle of a TCP server
- How clients connect and exchange data with a server
- How higher-level protocols (like HTTP) are built on top of TCP

The emphasis is on **understanding concepts and flow**, not memorizing syntax.

---

## What Is Being Built

At its core, this project builds a **TCP server** that:

- Creates a socket
- Binds it to an IP address and port
- Listens for incoming connections
- Accepts a client connection
- Exchanges data with the client
- Cleans up system resources

At this stage, the server communicates using **raw TCP**, not HTTP.

---

## Current Status

- [x] Project structure initialized  
- [x] Development environment verified  
- [x] Server socket creation implemented  
- [x] Address configuration completed  
- [ ] Listening for connections  
- [ ] Accepting clients  
- [ ] Sending and receiving data  
- [ ] HTTP support (later stage)

---

## Project structure

```text
.
├── src/
│   └── server.c
├── README.md
└── .gitignore
```

## High-level lifecycle (conceptual)

- Create a socket (ask the OS for a network endpoint)

- Configure the address (IP + port)

- Bind the socket to that address (reserve the port)

- Listen (tell the OS to queue incoming connections)

- Accept (take one connection from the queue)

- Read/Write (exchange bytes with the client)

- Close (release OS resources)

## Code walkthrough
### 1) Header files (imports)

These headers give access to standard C utilities and OS networking APIs.

Header	What it provides	Why we need it
stdio.h	printf, puts, etc.	Print messages / debug output
stdlib.h	exit, general utilities	Exit on fatal errors, utilities
string.h	memset, string helpers	Clear structs, handle buffers
unistd.h	close, POSIX calls	Close sockets (file descriptors)
arpa/inet.h	sockaddr_in, htons, inet_pton, etc.	Core IPv4 socket structures + byte order helpers

### 2) Create the server socket

This step creates the server’s socket, which is the fundamental object used for network communication.

The socket is requested from the operating system using the socket() system call. The OS then manages all low-level networking details such as packet transmission, buffering, and TCP state.

The call uses three arguments:

- The address family, which specifies IPv4 networking.

- The socket type, which specifies a stream-based socket (TCP).

- The protocol value, which is set to zero so the OS selects the correct protocol automatically.

If the call succeeds, it returns a non-negative integer known as a file descriptor. This file descriptor is the handle the program uses in all subsequent networking calls.

If the call fails, it returns -1, which indicates that the socket could not be created. Common reasons include resource exhaustion or insufficient permissions.

This step is required because all later server operations — binding to a port, listening for connections, and accepting clients — operate on this socket. Without it, no network communication is possible.

***project in progress...more info to come***
