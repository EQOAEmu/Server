///////////////////////////////////////////////////////////////
//udp.cpp 
//
//
///////////////////////////////////////////////////////////////

#include "udp.h"
#include "packet.h"
#include "CRCsupport.h"



using boost::asio::ip::udp;
using boost::asio::buffer;


struct SessionRequest {
    uint16_t clientcode;
    uint16_t sessioncode;

    SessionRequest()
    {
    }

    void serialize(Packet *p)
    {
	
    }

    void deserialize(Packet *p)
    {
	clientcode = p->read<uint16_t>();
	p->seek<uint32_t>();
	p->seek<uint16_t>();
	p->seek<uint8_t>();
	sessioncode = p->read<uint16_t>();

    }

};

struct SessionResponse {
    uint16_t clientcode;
    uint16_t servercode;
    uint16_t sessioncode;
    uint8_t length;
    uint8_t opcode;
    uint8_t unknown1;
    uint16_t seqno;
    uint32_t unknown2;
    uint64_t unknown3;
    uint16_t unknown4;

    SessionResponse()
    {
    }

    SessionResponse(uint16_t c, uint16_t s)
	:clientcode(c),
	sessioncode(s),
	servercode(0xb073),
	length(0x95),
	opcode(0x60),
	seqno(0x0100),
	unknown1(0x63),
	unknown2(0x01000200),
	unknown3(0xFB06010000002500),
	unknown4(0x00)
	{
	}

    void serialize(Packet *p)
    {	
	p->swap_bytes_write(servercode);
	p->write(clientcode);
	p->write(length);
	p->write(opcode);
	p->write(clientcode);
	p->write(sessioncode);
	p->write(unknown1);
	p->write(clientcode);
	p->write(sessioncode);
	p->swap_bytes_write(seqno);
	p->swap_bytes_write(unknown2);
	p->swap_bytes_write(unknown3);
	p->write(unknown4);
    }

    void deserialize(Packet *p)
    { 

    }

};


UDP::UDP(boost::asio::io_service& io_service)
: io_service_(io_service), socket_(io_service)
{
    set_port(7000);
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), port);
   // boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);
    socket_.open(boost::asio::ip::udp::v4());
    socket_.bind(endpoint);
    UDP::start_recv();
}

void UDP::set_ip(char const* n) 
{
    // boost::asio::ip::address::from_string(n);
}

void UDP::set_port(int n)
{
    this->port = n;
}

void UDP::start_recv()
{
    
    socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
    boost::bind(&UDP::handle_recv, this, boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));

}

void UDP::handle_recv(const boost::system::error_code& error, std::size_t bytes)
{
    std::cout << "Packet received" << std::endl;
    Packet p;
    p.set_msg_size(bytes);
    p.write(recv_buffer_);
    p.print();
    //determine packet type here
    //
    //
    //
   
    //for now just assume sessionrequset
    UDP::handle_session_request(&p);
    start_recv();
}

void UDP::send(Packet p)
{
    socket_.async_send_to(boost::asio::buffer(p.get_msg(), p.get_msg_size()),
    remote_endpoint_, boost::bind(&UDP::handle_send, this, boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

void UDP::handle_send(const boost::system::error_code& error, std::size_t bytes)
{
}


void UDP::handle_session_request(Packet *in)
{
    SessionRequest request;
    request.deserialize(in);
    SessionResponse response(request.clientcode, request.sessioncode);
    Packet out;
    
    //TODO  better way to determine size
    out.set_msg_size(35);

    response.serialize(&out);
    out.print();

    crc_t crc, crc_init;
    crc_init = 0x48e05191;
    crc = crc_update(crc_init, out.get_msg(), out.get_msg_size() - 4);
    printf("> Newly Calculated Packet CRC: 0x%08X\n",htonl(crc));
    out.write(crc);

    out.print();

    send(out);
    

}
