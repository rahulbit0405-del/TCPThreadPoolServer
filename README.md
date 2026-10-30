# TCP Thread Pool Server

A C++17 learning project exploring TCP sockets, concurrency, and a custom thread pool. It includes a TCP server and a thread pool as separate components.

> **Current behavior:** The server handles one client at a time. The thread pool runs four demonstration tasks when the program starts; client connections are not yet submitted to the pool.

## Features

- TCP server built with POSIX sockets
- Custom fixed-size worker pool using `std::thread`
- Shared task queue protected by a mutex and condition variable
- CMake build configuration
- Docker image for running the server on Linux

## Requirements

- C++17 compiler
- CMake 3.16 or later
- POSIX-compatible environment (Linux, macOS, or WSL)
- Docker (optional)

The server uses POSIX socket APIs and is not designed to build natively on Windows.

## Build and run

From the project root:

```bash
cmake -S . -B build
cmake --build build
./build/tcp_server
```

The server listens on port **8080** and prints a message when it is ready for clients.

To send a quick test message from another terminal, if `nc` (netcat) is installed:

```bash
printf 'Hello from client\n' | nc 127.0.0.1 8080
```

The server prints the received message and responds with:

```text
HELLO WORLD
```

Stop the server with `Ctrl+C`.

## Run with Docker

Build the image from the project root:

```bash
docker build -t tcp-threadpool-server .
```

Start the server and publish its port:

```bash
docker run --rm -p 8080:8080 tcp-threadpool-server
```

Send a test message from another terminal using the `nc` command above.

## How it works

### TCP server

The server creates a TCP socket, binds to port 8080 on all network interfaces, listens for connections, and accepts clients. For each accepted connection, it receives a message, sends `HELLO WORLD`, and closes the client socket.

```text
Client → socket → bind → listen → accept
                                  ↓
                         receive and reply
                                  ↓
                              close
```

Client connections are currently handled synchronously in the accept loop.

### Thread pool

The thread pool starts a fixed number of worker threads. At startup, four demonstration tasks are queued; each prints progress and sleeps for two seconds. Workers wait on a condition variable when the queue is empty, take tasks while holding the queue mutex, and execute tasks after releasing the lock.

```text
Queued tasks → shared task queue → worker threads → execute tasks
```

The server and thread pool are demonstrated by the same executable, but are not yet integrated: the server does not enqueue client handling as thread-pool work.

## Project layout

```text
.
├── include/
│   ├── TCPserver.h
│   └── ThreadPool.h
├── src/
│   ├── Tcpserver.cpp
│   ├── ThreadPool.cpp
│   └── main.cpp
├── CMakeLists.txt
├── Dockerfile
├── CONCURRENCY.md
└── README.md
```

## Concepts demonstrated

- TCP socket lifecycle: `socket`, `bind`, `listen`, `accept`, `recv`, and `write`
- Worker threads and task queues
- Mutual exclusion with `std::mutex`
- Worker coordination with `std::condition_variable`
- Joining worker threads during thread-pool destruction

## Future improvements

- Submit client handling to the thread pool so multiple clients can be served concurrently
- Add robust socket error handling and graceful server shutdown
- Add automated tests and concurrency benchmarks
- Extend the server with simple HTTP request handling

## More details

See [CONCURRENCY.md](CONCURRENCY.md) for notes on the thread pool and its synchronization model.
