CONCURRENCY DESIGN

1. Overview
This is a TCP multithreaded server written in c++.

The server uses a fixed thread pool size of 4 workers to process the requests of the clients concurrently . Instead of creating a new thread for every new connection ,ncoming client requests are placed into a shared task queue and processed by a pool of worker threads.

The main concurrency components are:
-  mutex
-  condition_variable
-  vector of threads
-  shared task queue
- Graceful conditions

2. ThreadPool
A thread pool of 4 workers is used . Instead of writing std::thread for each and every connection .
std::vector<std::thread> workers; is used .

3. Why a Thread Pool?

A simple multithreaded server could create a new thread for every client:

Client 1 -> Thread 1
Client 2 -> Thread 2
Client 3 -> Thread 3
Client 4 -> Thread 4

However, creating and destroying threads repeatedly introduces overhead and can become inefficient when many clients connect.

The thread pool creates a fixed number of worker threads once and reuses them.

Many client requests
        |
        v
   Task Queue
        |
        v
 Fixed Worker Pool

This separates connection handling from task execution.

4. Shared task queue 
It is done via:
std::queue<std::function<void()>>tasks
A task represents work that a worker thread should execute.

When the TCP server accepts a client, it can create a task containing the client-handling operation and add it to the queue.

Multiple worker threads may access this queue, so access must be synchronized.

5. Mutex
Mutex is used to avoid race conditions in the ciritcal sections present in the code for the shared queue of the thread pool.
std::mutex queueMuutex is used for the shared queue.

6. Condition variable 
A condition variable cv is used for notifying all the threads and waiting till the shared queue of the tasks is empty .
std::condition_variable cv is used .
7.  Worker Thread Lifecycle

Each worker repeatedly performs the following operations:

Start worker
     |
     v
Acquire queue lock
     |
     v
Wait if queue is empty
     |
     v
Take a task
     |
     v
Release queue lock
     |
     v
Execute task
     |
     v
Repeat
The task itself should be executed outside the queue lock so that other workers can access the queue while one worker is processing a task.

8. Graceful Shutdown

The thread pool maintains a shutdown flag:

bool stop;

When shutdown begins:

stop = true
     |
     v
Wake waiting workers
     |
     v
Workers finish/exit
     |
     v
join()
     |
     v
ThreadPool destroyed

The destructor joins the worker threads so that the program does not terminate while worker threads are still running.

9. Concurrency Model
The model of this TCP multithreaded server is based on a producer consumer problem pattern.

Where TCP server is the producer 
and
worker threads acts as the consumers with a shared buffer/queue of tasks.
