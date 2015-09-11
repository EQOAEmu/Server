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
:
_service(service)
{
}

TCPServer::~TCPServer()
{
}

boost::shared_ptr<Service> TCPServer::GetService()
{
    return _service;
}

void TCPServer::Bind()
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string(ip);
    boost::asio::ip::tcp::endpoint endpoint(address, port);
    _acceptor.bind(endpoint);
}

void TCPServer::Accept(boost::shared_ptr<TCPConnection> connection) 
{
    _acceptor.async_accept(accept->Socket(), boost::bind(&TCPServer::AcceptHander, this,
    boost::asio::placeholders::error, connection));

}

void TCPServer::AcceptHandler(const boost::system::error_code &ec,
boost::shared_ptr<TCPConnection> accepted) {
    if (!ec)
    {
//	TCPServer::Accept();
    }
    else 
    {
    }

}

