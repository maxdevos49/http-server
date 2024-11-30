# Notes:

#### `#include <sys/socket.h>`
Supplies a POSIX implementation of https://en.wikipedia.org/wiki/Berkeley_sockets. Capable of creating TCP, UDP, and other types of sockets.
- Use the `struct sockaddr_in address;` to supply the address information for a TCP IP connection casted to `(struct sockaddr *)` when passing to relevant functions.

#### Unit Testing
- MinUnit: A minimal unit test framework: https://github.com/siu/minunit/blob/master/README.md 
