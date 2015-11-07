///////////////////////////////////////////////////////////////
//tcp.cpp 
//
//
///////////////////////////////////////////////////////////////

#include "tcp.h"
#include "packet.h"


struct acceptConnection {
    uint32_t length;
    uint32_t type;
    uint64_t longPad;
    uint32_t uId;
    uint16_t result;
    uint16_t accSt;
    uint32_t time;
    uint64_t unknown1;
    uint64_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;

    acceptConnection()
    :length(0x000000a8),
    type(0x00000025),
    longPad(0x0000000000000000),
    uId(0x25a65176),
    result(0x0000),
    accSt(0x0003),
    time(0x0021223d),
    unknown1(0x53714754656b3138),
    unknown2(0x544f4144416c4e32),
    unknown3(0x00000001),
    unknown4(0x00000003)
    {
    }


    void serialize(Packet *p)
    {
	p->swap_bytes_write(length);
	p->swap_bytes_write(type);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->swap_bytes_write(uId);
	p->swap_bytes_write(result);
	p->swap_bytes_write(accSt);
	p->swap_bytes_write(time);
	p->write(time);
	p->swap_bytes_write(unknown1);
	p->swap_bytes_write(unknown2);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->write(longPad);
	p->swap_bytes_write(unknown3);
	p->swap_bytes_write(unknown4);
    }

};

/*-----------------------------------------------------------*/
TCPConnection::TCPConnection(boost::shared_ptr<Service> service)
: _service(service), tcp_socket(service->GetIoService()), recv_buffer(512)
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
	 std::cout << __PRETTY_FUNCTION__ << "\n";
    boost::asio::async_read(tcp_socket, boost::asio::buffer(recv_buffer, 94),
    boost::bind(&TCPConnection::ReadHandler, shared_from_this(), boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));

}

void TCPConnection::AsyncWrite(Packet *out)
{
	 std::cout << __PRETTY_FUNCTION__ << "\n";
    boost::asio::async_write(tcp_socket, boost::asio::buffer(out->get_msg(),
    out->get_msg_size()), boost::bind(&TCPConnection::WriteHandler, shared_from_this(),
    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}


void TCPConnection::ReadHandler(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error)
    {

	for (int i = 0; i < 94; i++) {
        std::cout << std::hex << static_cast<unsigned int>(this->recv_buffer[i]) << std::flush;
	}
	std::cout <<  "\n";

	HandleConnectionRequest();
	AsyncRead();
    }
    else {
	     std::cout << error.message() << std::endl;
    }
	
}

void TCPConnection::WriteHandler(const boost::system::error_code &error, size_t bytes)
{
    if (!error) {
	 std::cout << __PRETTY_FUNCTION__ << "\n";
    }

    else {
	     std::cout << error.message() << std::endl;
    }

} 

void TCPConnection::HandleConnectionRequest() 
{
   acceptConnection accept;
   Packet out;
   out.set_msg_size(168);
   accept.serialize(&out);
   out.print();
   AsyncWrite(&out);

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

void TCPServer::Start(const uint16_t &port)
{
    std::cout << "Starting TCP Server" << std::endl;
    Listen(port);
    Accept();
}

void TCPServer::Listen(const uint16_t &port)
{
     std::cout << __PRETTY_FUNCTION__ << "\n";
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 7000);
    _acceptor.open(endpoint.protocol());
    _acceptor.bind(endpoint);
    _acceptor.listen();
}

void TCPServer::Accept()
{
    
     std::cout << __PRETTY_FUNCTION__ << "\n";
    boost::shared_ptr<TCPConnection> connection = boost::make_shared<TCPConnection>(_service);
    std::cout << connection->GetSocket().is_open() << "\n";
    if (connection->GetSocket().is_open()) {
     std::cout << "open" << "\n";
    }
    
    _acceptor.async_accept(connection->GetSocket(), boost::bind(&TCPServer::AcceptHandler, this,
    boost::asio::placeholders::error, connection));

}

void TCPServer::AcceptHandler(const boost::system::error_code &ec,
boost::shared_ptr<TCPConnection> accepted) {
    if (!ec)
    {
	accepted->AsyncRead();
	Accept();
    }
    else 
    {
	     std::cout << ec.message() << std::endl;
    }

}

