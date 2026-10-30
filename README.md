# TCP Multithreaded Server

A C++ systems programming project focused on **TCP socket programming, multithreading, synchronization, and a custom thread pool**.

The project contains two core systems-programming components:

- A TCP server implemented using POSIX sockets
- A custom C++ thread pool demonstrating concurrent task execution

The components are currently implemented and tested independently.

---

## Current Status

### Completed

- TCP socket programming fundamentals
- TCP server socket creation
- `socket()`
- `setsockopt()`
- `bind()`
- `listen()`
- `accept()`
- `recv()`
- `write()`
- Custom C++ thread pool
- Worker thread creation
- Shared task queue
- `std::mutex`
- `std::condition_variable`
- Graceful worker shutdown
- Thread joining
- Concurrent task execution testing
- CMake build system
- Docker support

### Future Work

- Integrate TCP client handling with the thread pool
- Handle multiple TCP clients concurrently using worker threads
- Minimal HTTP request handling
- Performance and concurrency measurements
- Improved logging and error handling

---

## Architecture

The current project contains two independently implemented components.

### TCP Server

```text
TCP Client
    |
    v
 socket()
    |
    v
  bind()
    |
    v
 listen()
    |
    v
  accept()
    |
    v
  recv()
    |
    v
Client Handling
    |
    v
 write()
    |
    v
 close()
```
The TCP server listens for incoming TCP connections, accepts a client, receives data, sends a response, and closes the client connection.

## Thread Pool
                ThreadPool
                    |
                    v
            Shared Task Queue
                    |
        +-----------+-----------+
        |           |           |
        v           v           v
     Worker 1    Worker 2    Worker 3 ...
        |           |           |
        +-----------+-----------+
                    |
                    v
               Execute Tasks
               The thread pool maintains a fixed number of worker threads and a shared queue of tasks.

The TCP server and thread pool are currently separate components.
The custom Threadpool contains:
workers
tasks
queueMutex
condition_variable
stop flag

## Worker Flow

Each worker follows this basic process:
Worker starts
     |
     v
Lock task queue
     |
     v
Wait until:
  task exists OR
  shutdown requested
     |
     v
Take a task
     |
     v
Unlock queue
     |
     v
Execute task
     |
     v
Repeat
The mutex protects access to the shared task queue.

The condition variable prevents worker threads from continuously polling an empty queue.

The worker executes the task after releasing the mutex, allowing other workers to access the queue concurrently.

## Concurrency Test

The current thread-pool test creates four workers and four test tasks.

Each task sleeps for two seconds:
std::this_thread::sleep_for(std::chrono::seconds(2));
With four workers, the tasks can execute concurrently:

Worker 1 -> Task 1 -> 2 seconds
Worker 2 -> Task 2 -> 2 seconds
Worker 3 -> Task 3 -> 2 seconds
Worker 4 -> Task 4 -> 2 seconds

Instead of approximately eight seconds of sequential execution, the total execution time should be close to two seconds, subject to scheduling and system overhead.

The exact order in which tasks start and finish is not deterministic because the worker threads execute concurrently.

Technologies
C++17
POSIX TCP sockets
std::thread
std::mutex
std::condition_variable
std::queue
std::function
C++ STL
CMake
Docker
Project Structure
TCP multithreaded server/

├── include/
│   ├── TCPserver.h
│   └── ThreadPool.h
│
├── src/
│   ├── Tcpserver.cpp
│   ├── ThreadPool.cpp
│   └── main.cpp
│
├── CMakeLists.txt
├── Dockerfile
├── .dockerignore
├── .gitignore
├── CONCURRENCY.md
└── README.md
Building

From the project root:

mkdir -p build
cd build
cmake ..
cmake --build .
Running

From the build directory:
./tcp_server
The TCP server listens on port 8080

## Docker
Build the Docker image from the project root:

docker build -t tcp-threadpool-server .

Run the container:

docker run -p 8080:8080 tcp-threadpool-server

## Example
The current program creates a thread pool with four workers:

Threadpool pool(4);

The thread-pool constructor creates several test tasks so that the worker threads can be observed executing concurrently.

The TCP server is started independently:

TcpServer server;
server.start();

Therefore, the current program demonstrates both systems in the same project, but the TCP server does not currently submit client-handling tasks to the thread pool.

## Key Concepts Demonstrated

TCP Socket Programming

The TCP server demonstrates the basic server-side socket lifecycle:

socket()
   |
bind()
   |
listen()
   |
accept()
   |
recv()
   |
write()
   |
close()

## Thread Pool

Instead of creating a new thread for every piece of work, a fixed number of worker threads are created and reused.

## Shared Task Queue

Workers obtain pending tasks from a shared queue.

## Mutex

The mutex prevents multiple workers from modifying the queue simultaneously.

## Condition Variable

Workers sleep when there is no work and wake when a task becomes available or shutdown is requested.

## Graceful Shutdown

When the thread pool is destroyed:

stop = true
     |
     v
notify workers
     |
     v
workers finish remaining tasks
     |
     v
join()
     |
     v
threads terminate safely

## Learning Goals

This project is primarily intended as a systems-programming learning project.

The main goals are to understand:

How TCP sockets work at the OS level
How a TCP server accepts clients
How multiple threads can process work concurrently
How shared data creates synchronization requirements
How mutexes protect shared resources
How condition variables coordinate threads
How a thread pool manages worker threads
How CMake can be used to build a C++ project
How Docker can be used to package and run an application


## Build
For the project root:


```bash
mkdir -p build
cd build
cmake ..
cmake --build .

- Then run  : 
./tcp_server

- ## UML class diagram

'''mermaid
classDiagram
%% Existing TcpServer Class

class TcpServer{
    -serverSocket :int
    -running : bool
    -serverAddr : struct sockaddr_in 

    +TcpServer()
    +start():void
    +stop():void
    -setupSocket():void
    -acceptClients():void
    -handleclient(int clientSocket):void
}

%% Threadpool class

class Threadpool{
    -cv: std::condition_variable
    -queueMutex: std::mutex
    -stop : bool
    -workers : std::vector<std::thread>
    -tasks :  std::queue<std::function<void()>>

    +worker() : void
    +Threadpool(numThreads : size_t)
    +~Threadpool()
}
## Future Architecture

A possible future version could connect the two components so that TCP client handling is delegated to the thread pool:
Client
   |
   | TCP connection
   v
socket()
   |
bind()
   |
listen()
   |
accept()
   |
   v
New Client
   |
   v
Submit Client Task
   |
   v
Task Queue
   |
   +----------+----------+----------+
   |          |          |          |
   v          v          v          v
Worker 1   Worker 2   Worker 3   Worker 4
   |          |          |          |
   +----------+----------+----------+
                  |
                  v
             Handle Client
             This integration is intentionally kept as future work so that the TCP server and thread-pool implementations can first be understood and tested independently.
