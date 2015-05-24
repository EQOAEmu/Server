///////////////////////////////////////////////////////////////
//packet.cpp 
//
///////////////////////////////////////////////////////////////

#include "packet.h"

#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)

Packet::Packet()
    :writeptr(0),
    readptr(0)
    {
    }

    void Packet::set_msg_size(int size)
    {
	std::cout << "size: " << size << std::endl;
	this->size = size;
	buffer = new unsigned char[size];
    }

    void Packet::swap_bytes(unsigned short &bytes)
    {
     bytes = (bytes >> 8) |
         (bytes << 8);
    }

    void Packet::swap_bytes(unsigned int &bytes)
    {
     bytes = (bytes >> 24) |
         ((bytes & 0x00FF0000) >> 8) |
         ((bytes & 0x0000FF00) << 8) |
         (bytes << 24);
    }

    void Packet::swap_bytes(unsigned long int &bytes)
    {
        bytes = (bytes >> 56) |
          ((bytes & 0x00FF000000000000) >> 40) |
          ((bytes & 0x0000FF0000000000) >> 24) |
          ((bytes & 0x000000FF00000000) >> 8)  |
          ((bytes & 0x00000000FF000000) << 8)  |
          ((bytes & 0x0000000000FF0000) << 24) |
          ((bytes & 0x000000000000FF00) << 40) |
          (bytes << 56);
    }





    //ugly, but will work for now
    void Packet::print()
    {
	int i, j;
	std::cout << "BEGIN packet dump" << std::endl;

	for (i = 0; i < size; i += 16) {	

	    //print hex 
	    for (j = 0; j < 16 && i + j < size; j++) {
		printf("%02X ", (unsigned char)buffer[i + j]);
	     }   
	    printf(" ");

	    for (j = j; j < 16; j++) {
		printf("   ");
	    }

	    //print ascii
	    for (j = 0; j < 16 && i + j < size; j++) {
		if (IS_PRINTABLE(buffer[i + j]))
                printf("%c", buffer[i + j]);
		else
		    printf(".");
	    }
	    printf("\n");
    
	}
	std::cout << "END packet dump" << std::endl;

    }


