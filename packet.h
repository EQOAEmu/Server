#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <vector>
#include <string.h>
#include <stdint.h>

#include <stdio.h>

/**
 * Packet Class
 *
 * A container used to store bytes. These bytes can be in the form of a message received
 * from the client, or dynamically constructed as a new message that will be sent to the client.
 */
class Packet
{
public:
  Packet();
  ~Packet();

  /**
   * Returns the contents of the packet
   *
   * @return pointer to first element in the packet
   */
  const unsigned char* get_msg() {
    return buffer.data();
  }

  /**
   * Returns the size of the packet
   *
   * @return Integer size
   */
  size_t get_msg_size() {
    return size;
  }

  /**
   * Sets the size of the packet
   *
   * @param bufsize An integer used to resize the packet
   */
  void set_msg_size(int bufsize);


  /**
   * Prints the packet to the terminal
   *
   */
  void print();

  /**
   * Writes data to the packet
   *
   * @param data The hex digit, e.g. 0xFF, to be written to the packet
   */
  template<class T>
  void write (T& data);

  /**
   * Writes data to the packet, but changes endianness first
   *
   * @param data The hex digit, e.g. 0xFF, to be written to the packet
   */
  template<class T>
  void swap_bytes_write(T& data);

  /**
   * Writes a wide string to the packet
   *
   * @param ws The wide string to be written to the packet
   */
  void write_wstring(std::wstring ws);

  /**
   * Reads from a packet starting at the current read pointer
   * Increments the read pointer by the number of bytes read
   *
   * @return The bytes read from the packet
   */
  template<class T>
  T read();

  /**
   * Reads from a packet starting at the current read pointer
   * Does not increment the read pointer after the bytes are read
   *
   * @return The bytes read from the packet
   */
  template<class T>
  T peek(T data);

  /**
   * Increments the read pointer
   *
   */
  template<class T>
  void seek();

  /**
   * Reverses endianness for short
   *
   */
  void swap_bytes(unsigned short &bytes);

  /**
   * Reverses endianness for int
   *
   */
  void swap_bytes(unsigned int &bytes);

  /**
   * Reverses endianness for long
   *
   */
  void swap_bytes(unsigned long int &bytes);

  /**
   * Returns the write pointer
   *
   * @return The write pointer
   */
  int get_writeptr() {
    return writeptr;
  }


private:
  std::vector<unsigned char> buffer;
  int size;
  int writeptr;
  int readptr;
};


template <class T>
void Packet::write(T& data)
{
  buffer.insert(buffer.begin() + writeptr, reinterpret_cast<unsigned char *>(&data),
                reinterpret_cast<unsigned char *>(&data) + sizeof(T));
  writeptr += sizeof(T);
}

template <class T>
void Packet::swap_bytes_write(T& data)
{
  swap_bytes(data);
  buffer.insert(buffer.begin() + writeptr, reinterpret_cast<unsigned char *>(&data),
                reinterpret_cast<unsigned char *>(&data) + sizeof(T));
  writeptr += sizeof(T);
}

template <class T>
T Packet::read()
{
  T bytes;
  memcpy(&bytes, buffer.data() + readptr, sizeof(T));
  readptr += sizeof(T);
  return bytes;
}

template <class T>
T Packet::peek(T pos)
{
  T bytes;
  memcpy(&bytes, buffer.data() + pos, sizeof(T));
  return bytes;
}

template <class T>
void Packet::seek()
{
  readptr += sizeof(T);
}

#endif
