# ServerClientFIFO

**Overview**
This project implements a basic interprocess communication (IPC) system using named pipes (FIFOs) in C. It consists of two main components: a server and a client. The server listens for requests from multiple clients and performs specific system calls based on the client's request. The client can request various operations like doubling or tripling a number, storing a value in server memory, or retrieving a stored value.

**File Description**
server.c: Contains the code for the server side of the IPC system. It sets up a named pipe to receive requests from clients, processes these requests (like connecting, doubling/triple a number, storing, and recalling values), and sends responses back to the clients.

client.c: Contains the client-side implementation. It communicates with the server through named pipes, sending requests like connecting to the server, performing arithmetic operations, and storing and recalling values from the server's memory.

**Features**
Connect to Server: Clients can establish a connection with the server.
Arithmetic Operations: Perform operations like doubling or tripling a number.
Memory Operations: Store and recall values in/from the server's memory.
Server Termination: Clients can send a termination signal to the server.

# How to Run
**Compile the server and client code.**
gcc server.c -o server
gcc client.c -o client

**Start the server in one terminal.**
./server

**Open another terminal window and start the client.**
./client

Follow the on-screen instructions on the client terminal to interact with the server.

**System Requirements**
Linux-based operating system (due to the use of UNIX-specific system calls and named pipes)
GCC compiler

**Limitations**
The current implementation is basic and serves as a proof of concept for IPC using FIFOs.
Error handling is minimal, and the system assumes ideal operating conditions.
