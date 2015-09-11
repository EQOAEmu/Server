
///////////////////////////////////////////////////////////////
//service.h 
//
//
///////////////////////////////////////////////////////////////

#ifndef SERVICE_H
#define SERVICE_H_

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class Service : public boost::enable_shared_from_this<Service>
{
public:
    Service();
    virtual ~Service();

    boost::asio::io_service& GetIoService();
    void Run();

private:
    boost::asio::io_service _ioservice;
};

#endif
