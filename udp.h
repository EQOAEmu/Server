///////////////////////////////////////////////////////////////
//udp.h 
//
//
///////////////////////////////////////////////////////////////

#ifndef UDPServer_H_
#define UDPServer_H_

#include "packet.h"
#include "service.h"
#include "opcodes.h"
#include "crc.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>




class UDPServer {
public:
    explicit UDPServer(boost::shared_ptr<Service> service);
    ~UDPServer();
    
    boost::shared_ptr<Service> GetService();

    std::string GetIp() { return ip; }
    int GetPort() { return port; }

    void SetIp(char const* n);

    void Start(const uint16_t port);
    void StartRecv();
    void ReadHandler(const boost::system::error_code& error, std::size_t bytes);

    void AsyncSendTo(Packet p);
    void SendToHandler(const boost::system::error_code& error, size_t bytes);

    void HandleSessionRequest(Packet *p);
    void HandleServerRequest(Packet *p);
   
private:
    boost::shared_ptr<Service> _service;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::string ip;
    int port; 
    char recv_buffer_[512];
};
    
#endif
