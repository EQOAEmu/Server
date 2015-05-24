///////////////////////////////////////////////////////////////
//packet.h 
//
///////////////////////////////////////////////////////////////

#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <string.h>

#include <stdio.h>

class Packet {
public:
    Packet();
    
//      unsigned char* get_msg() { &buffer[0]; }
    unsigned char* get_msg() { return buffer; }
    int get_msg_size() { return size; }

    void set_msg_size(int bufsize);
    void print();

    template<class T>
    void write(T& data);

    template<class T>
    void swap_bytes_write(T& data);

    template<class T>
    T read();

    template<class T>
    void seek();

    void swap_bytes(unsigned short &bytes);
    void swap_bytes(unsigned int &bytes);
    void swap_bytes(unsigned long int &bytes);

    int get_writeptr() { return writeptr; }


/*
    template<class T>
    T read(int ptr, T);
*/

private:
    unsigned char* buffer; 
    int size;
    int writeptr;
    int readptr;
};
    

template <class T>
void Packet::write(T& data)
{
    memcpy(buffer+writeptr, &data, sizeof(T));
    writeptr += sizeof(T);
}


template <class T>
void Packet::swap_bytes_write(T& data)
{
    swap_bytes(data);
    memcpy(buffer+writeptr, &data, sizeof(T));
    writeptr += sizeof(T);
}

template <class T>
T Packet::read()
{
    T bytes;
    memcpy(&bytes, buffer + readptr, sizeof(T));
    readptr += sizeof(T);
    return bytes;
}

template <class T>
void Packet::seek()
{
    readptr += sizeof(T);
}

/*
template <class T>
T Packet::read()
{
    T bytes = read<T>(readptr, bytes);   
    readptr += sizeof(T);
    return bytes;
}

template <class T>
T Packet::read(int readptr, T bytes)
{
    memcpy(&bytes, buffer+readptr, sizeof(T));
    return bytes;
}
*/
#endif
