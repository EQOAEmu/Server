///////////////////////////////////////////////////////////////
//main.cpp
//
//
///////////////////////////////////////////////////////////////

#include <iostream>
#include "udp.h"
#include "tcp.h"


int main(int argc, char *argv[]) {
    try
    {
	boost::shared_ptr<Service> service(new Service());
	boost::shared_ptr<TCPServer> tcpServer(new TCPServer(service));
	UDPServer udpServer(service);
	tcpServer->Start(7000);
	udpServer.Start(7001);
	service->Run();
    }
    catch (std::exception& e)
    {
	std::cerr << e.what() << std::endl;
    }


	return 0;
    }


