///////////////////////////////////////////////////////////////
//tcp.cpp 
//
//
///////////////////////////////////////////////////////////////

#include "tcp.h"
#include "packet.h"
/*-----------------------------------------------------------*/
TCPConnection::TCPConnection(boost::shared_ptr<Service> service)
: _service(service), tcp_socket(service->GetIoService())
{
}

TCPConnection::~TCPConnection()
{
}

boost::asio::ip::tcp::socket& TCPConnection::GetSocket()
{
    return tcp_socket;
}

void TCPConnection::AsyncRead()
{
    boost::asio::async_read(tcp_socket, boost::asio::buffer(recv_buffer),
    boost::bind(&TCPConnection::ReadHandler, this, boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));

}

void TCPConnection::AsyncWrite(Packet out)
{
    boost::asio::async_write(tcp_socket, boost::asio::buffer(out.get_msg(),
    out.get_msg_size()), boost::bind(&TCPConnection::WriteHandler, shared_from_this(),
    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}


void TCPConnection::ReadHandler(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error)
    {
	std::cout << "received a packet" << std::endl;
	//packet code
	AsyncRead();
    }
	
}

void TCPConnection::WriteHandler(const boost::system::error_code &error, size_t bytes)
{

} 

/*-----------------------------------------------------------*/
TCPServer::TCPServer(boost::shared_ptr<Service> service)
: _service(service), _acceptor(service->GetIoService())
{
}

TCPServer::~TCPServer()
{
}

boost::shared_ptr<Service> TCPServer::GetService()
{
    return _service;
}

void TCPServer::Start()
{
    std::cout << "Starting TCP Server" << std::endl;
    //Accept();
}

void TCPServer::Bind(const std::string &ip, uint16_t port)
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string(ip);
    boost::asio::ip::tcp::endpoint endpoint(address, port);
    _acceptor.bind(endpoint);
}

void TCPServer::Accept(boost::shared_ptr<TCPConnection> tcpConnection) 
{
    _acceptor.async_accept(tcpConnection->GetSocket(), boost::bind(&TCPServer::AcceptHandler, this,
    boost::asio::placeholders::error, tcpConnection));

}

void TCPServer::AcceptHandler(const boost::system::error_code &ec,
boost::shared_ptr<TCPConnection> accepted) {
    if (!ec)
    {
	accepted->AsyncRead();
	//Accept();
    }
    else 
    {
    }

}

