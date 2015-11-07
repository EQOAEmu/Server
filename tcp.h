///////////////////////////////////////////////////////////////
//tcp.h 
//
//
///////////////////////////////////////////////////////////////

#ifndef TCP_H_
#define TCP_H_

#include "service.h"
#include "packet.h"

class TCPConnection;
class TCPServer;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection> 
{
public:
    typedef boost::shared_ptr<TCPConnection> Ptr;
    TCPConnection(boost::shared_ptr<Service> service);

    virtual ~TCPConnection();
	
    boost::asio::ip::tcp::socket& GetSocket();

    void Session()
    {
	AsyncRead();
    }
   
    void Stop()
    {
       tcp_socket.cancel();
    }

    boost::asio::ip::tcp::socket tcp_socket;

    void Listen(uint16_t port);
    
    void AsyncRead();
    void ReadHandler(const boost::system::error_code& error, std::size_t bytes);

    void AsyncWrite(Packet *p);
    void WriteHandler(const boost::system::error_code &error, size_t bytes);

    void HandleConnectionRequest();

private:
    std::vector<uint8_t> recv_buffer;
    boost::shared_ptr<Service> _service;

};

class TCPServer : public boost::enable_shared_from_this<TCPServer>
{
public:
    TCPServer(boost::shared_ptr<Service> service);
    virtual ~TCPServer();

    boost::shared_ptr<Service> GetService();
    
    void Start(const uint16_t &port);
    void Listen(const uint16_t &port);
    void Listen(const std::string &ip, const uint16_t &port);
    void Accept();
    void AcceptHandler(const boost::system::error_code &ec, boost::shared_ptr<TCPConnection>
    tcpConnection);

private:
    boost::shared_ptr<Service> _service;
    boost::asio::ip::tcp::acceptor _acceptor;
    
    
};

#endif
