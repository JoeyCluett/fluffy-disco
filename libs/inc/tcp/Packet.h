#ifndef __JJC__PACKET__H__
#define __JJC__PACKET__H__

#include <vector>
#include <utility>
#include <string>
#include <iterator>

#define INTEGER_P 0
#define FLOAT_P   1
#define STRING_P  2

#define ARR(a, b, c) pair<pair<int, int>, void*>{{a, b}, (void*)c}

/*
    Packet is not associated with any transmision 
    type (files, sockets), it simply stores data in binary form
*/

class Packet {
private:
    std::vector<char> buffer;
    void push(char* buf, int s);
    void pop(char* buf, int s);

public:
    // a catch-all for arrays
    // pack << {{P_INTEGER, 4}, src*}
    auto operator<<(std::pair<std::pair<int, int>, void*>) -> Packet&;

    // store single data
    auto operator<<(int) -> Packet&;
    auto operator<<(float) -> Packet&;
    auto operator<<(std::string) -> Packet&;

    // Packet does not check if user has properly allocated memory
    // pack >> {{P_INTEGER, 4}, src*}
    auto operator>>(std::pair<std::pair<int, int>, void*>) -> Packet&;

    // read single data
    auto operator>>(int&) -> Packet&;
    auto operator>>(float&) -> Packet&;
    auto operator>>(std::string&) -> Packet&;

    // automatically prepends the size of the packet
    // capitalized because write is already a syscall
    void Write(int fd);

    // reads the size of the incoming packet first
    // capitalized because read is already a syscall
    void Read(int fd);

    // clear the internal buffer
    void clear(void);

    // Packet supports full forwards iteration
    auto begin(void) -> std::vector<char>::iterator;
    auto end(void) -> std::vector<char>::iterator;

};

#endif // __JJC__PACKET__H__