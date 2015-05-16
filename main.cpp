///////////////////////////////////////////////////////////////
//main.cpp
//
//
///////////////////////////////////////////////////////////////

#include <iostream>
#include "udp.h"


int main(int argc, char *argv[]) {
    try
    {
	boost::asio::io_service io_service;
	UDP udpserver(io_service);
	std::cout << udpserver.get_ip() << std::endl;
	std::cout << udpserver.get_port() << std::endl;
	io_service.run();
    }
    catch (std::exception& e)
    {
	std::cerr << e.what() << std::endl;
    }


	return 0;
    }


