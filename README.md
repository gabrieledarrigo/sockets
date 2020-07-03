# Sockets 🔌
> A repository that stores various C exercises on network sockets

This repository contains various exercises and examples on network socket programming in C that I'm currently studyng for the "Networking" course of ["Sicurezza dei Sistemi e delle Reti Informatiche"](http://sicurezzaonline.di.unimi.it/) bachelor's degree program.
They are probably simple and trivial programs but, you know, it's worth sharing!

## Exercises and examples

- [tcp-server-iterative](./tcp-server-iterative): A basic iterative TCP server.
- [tcp-server-fork](./tcp-server-fork): A TCP server that spawns a child process for each incoming connection.
- [tcp-server-select](./tcp-server-select): A TCP server handles connection via select.
- [hello-world-udp](./hello-world-udp): An UDP client that sends a simple "Hello World" datagram to a fixed address UDP Server.
- [udp-client](./hello-world-udp): An iterative UDP client that accepts a message from the command line and send it to an UDP Server.
- [pizza](./pizza): A TCP pizza 🍕 delivery service.
- [popper](./popper): A simple dummy TCP POP3 client 📫 📧.
- [utils](./utils): A fast up-and-running Node.JS UDP server that listens on 127.0.0.1.41234 and send back a message.

### References 🔖📚

Here's some material that I used to study and learn:

- The monster 🦖: [Computer Networks, 5th Edition Andrew S. Tanenbaum](https://www.pearson.com/us/higher-education/program/Tanenbaum-Computer-Networks-5th-Edition/PGM270019.html)
- A classic C book: [C Programming: A Modern Approach - K. N. King](http://knking.com/books/c2/index.html)
- Linux man pages, useful for a better description of network system calls: [https://linux.die.net/](https://linux.die.net/)
- A useful book on network programming [Hands-On Network Programming with C](https://www.packtpub.com/networking-and-servers/hands-network-programming-c)
- The great [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)

