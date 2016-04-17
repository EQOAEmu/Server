/**
 * Service Class
 *
 */
#include "service.h"

Service::Service()
{
}

Service::~Service()
{
}

boost::asio::io_service& Service::GetIoService()
{
  return _ioservice;
}

void Service::Run()
{
  _ioservice.run();
}

