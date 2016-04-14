/**
 * Service Class
 *
 * A class used for managing the boost::asio io_service
 */

#ifndef SERVICE_H
#define SERVICE_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

class Service : public boost::enable_shared_from_this<Service>
{
public:
  Service();
  virtual ~Service();

  /**
   * Returns the io_service
   *
   * @return _ioservice The boost::asio io_service
   */
  boost::asio::io_service& GetIoService();

  /**
   * Begins the io_service event processing loop
   *
   */
  void Run();

private:
  boost::asio::io_service _ioservice;
};

#endif
