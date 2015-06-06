///////////////////////////////////////////////////////////////
//packet.h 
//
///////////////////////////////////////////////////////////////

#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <vector>
#include <string.h>
#include <stdint.h>

#include <stdio.h>

class Packet {
public:
    Packet();
   ~Packet();
    
    const unsigned char* get_msg() { return &buffer.front(); }
    size_t get_msg_size() { return buffer.capacity(); }

    void set_msg_size(size_t bufsize);
    void print();

    template<class T>
    void write (T& data);

    template<class T>
    void swap_bytes_write(T& data);

    void write_wstring(std::wstring ws);

    template<class T>
    T read();

    template<class T>
    void seek();

    void swap_bytes(unsigned short &bytes);
    void swap_bytes(unsigned int &bytes);
    void swap_bytes(unsigned long int &bytes);

    int get_writeptr() { return writeptr; }


private:
    std::vector<unsigned char> buffer;
    int size;
    int writeptr;
    int readptr;
};
    

template <class T>
void Packet::write(T& data) 
    {
	std::cout << writeptr << std::endl;
	buffer.insert(buffer.begin() + writeptr, reinterpret_cast<unsigned char *>(&data),
	reinterpret_cast<unsigned char *>(&data) + sizeof(T));
	writeptr += sizeof(T);
    }

template <class T>
void Packet::swap_bytes_write(T& data) {
	std::cout << writeptr << std::endl;
	swap_bytes(data);
	buffer.insert(buffer.begin() + writeptr, reinterpret_cast<unsigned char *>(&data),
	reinterpret_cast<unsigned char *>(&data) + sizeof(T));
	writeptr += sizeof(T);
    }

template <class T>
T Packet::read()
    {
	T bytes;
       // memcpy(&bytes, buffer + readptr, sizeof(T));
	memcpy(&bytes, buffer.data() + readptr, sizeof(T));
	readptr += sizeof(T);
	return bytes;
    }

template <class T>
    void Packet::seek()
    {
	readptr += sizeof(T);
    }

#endif
