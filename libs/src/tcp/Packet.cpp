#include <tcp/Packet.h>
#include <stdexcept>
#include <cstring>
#include <unistd.h>

auto Packet::begin(void) -> std::vector<char>::iterator {
    return this->buffer.begin(); 
}

auto Packet::end(void) -> std::vector<char>::iterator {
    return this->buffer.end();
}

void Packet::Read(int fd) {
    int s = -1;
    read(fd, (char*)&s, 4);

    if(s != -1) {
        this->buffer.resize(s);
        read(fd, &buffer[0], s);
    } else {
        throw std::runtime_error("Packet::Read(int fd) -> error reading packet size");
    }

}

void Packet::Write(int fd) {
    int i = buffer.size();
    write(fd, (char*)&i, 4);
    write(fd, &buffer[0], buffer.size());
}

void Packet::clear(void) {
    this->buffer.clear();
}

void Packet::push(char* buf, int s) {
    for(int i = 0; i < s; i++)
        this->buffer.push_back(buf[i]);
}

void Packet::pop(char* buf, int s) {
    std::memcpy(buf, &this->buffer[0], s);
    auto start = this->buffer.begin();
    this->buffer.erase(start, start+s);
}

auto Packet::operator<<(int i) -> Packet& {
    this->push((char*)&i, 4);
    return *this;
}

auto Packet::operator<<(float f) -> Packet& {
    this->push((char*)&f, 4);
    return *this;
}

auto Packet::operator<<(std::string s) -> Packet& {
    this->push((char*)s.c_str(), s.size()+1);
    return *this;
}

auto Packet::operator>>(int& i) -> Packet& {
    this->pop((char*)&i, 4);
    return *this;
}

auto Packet::operator>>(float& f) -> Packet& {
    this->pop((char*)&f, 4);
    return *this;
}

auto Packet::operator>>(std::string& s) -> Packet& {
    // hacky simple way to do this
    s = &this->buffer[0];

    auto iter = this->buffer.begin();
    buffer.erase(iter, iter+s.size()+1);

    return *this;
}

auto Packet::operator>>(std::pair<std::pair<int, int>, void*> p) -> Packet& {
    int& num_elements = p.first.second;
    int& element_type = p.first.first;

    switch(element_type) {
        case INTEGER_P:
        case FLOAT_P:
            this->pop((char*)p.second, 4*num_elements);
            break;
        case STRING_P:
            {
                std::string* s_ptr = (std::string*)p.second;
                for(int i = 0; i < num_elements; i++)
                    *this >> s_ptr[i];
            }
            break;
        default:
            throw std::runtime_error("Packet::operator<<(std::pair<std::pair<int, int>, void*>) -> unknown type");
    }

    return *this;
}

auto Packet::operator<<(std::pair<std::pair<int, int>, void*> p) -> Packet& {
    int& num_elements = p.first.second;
    int& element_type = p.first.first;

    switch(element_type) {
        case INTEGER_P:
            {
                int* i_ptr = (int*)p.second;
                this->push((char*)i_ptr, num_elements*4);
            }
            break;
        case FLOAT_P:
            {
                float* f_ptr = (float*)p.second;
                this->push((char*)f_ptr, num_elements*4);
            }
            break;
        case STRING_P:
            {
                std::string* s_ptr = (std::string*)p.second;
                for(int i = 0; i < num_elements; i++)
                    this->push((char*)s_ptr[i].c_str(), s_ptr[i].size()+1);
            }
            break;
        default:
            throw std::runtime_error("Packet::operator<<(std::pair<std::pair<int, int>, void*>) -> unknown type");
            break;
    }

    return *this;
}
