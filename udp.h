///////////////////////////////////////////////////////////////
//udp.h 
//
//
///////////////////////////////////////////////////////////////

#ifndef UDP_H_
#define UDP_H_

#include "packet.h"
#include "crc.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>




class UDP {
public:
    explicit UDP(boost::asio::io_service& io_service);
    
    std::string get_ip() { return ip; }
    int get_port() { return port; }

    void set_ip(char const* n);
    void set_port(int n);

    void start_recv();
    void handle_recv(const boost::system::error_code& error, std::size_t bytes);

    void send(Packet p);
    void handle_send(const boost::system::error_code& error, size_t bytes);

    void handle_session_request(Packet p);
    
private:
    boost::asio::io_service& io_service_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::string ip;
    int port; 
    //  enum { max_length = 512 };
    //char data_[max_length];
    //boost::array<uint8_t, 512> recv_buffer_;
    char recv_buffer_[512];
};
    
#endif
