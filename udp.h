#ifndef UDPServer_H_
#define UDPServer_H_

#include "packet.h"
#include "service.h"
#include "opcodes.h"
#include "crc.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>


/**
 * UDPServer Class
 *
 * Manages the UDP server by aynchronously receving and sending UDP packets
 */

class UDPServer
{
public:
  explicit UDPServer(boost::shared_ptr<Service> service);
  ~UDPServer();

  /**
   * Returns _service
   *
   * @return _service The shared pointer for our io_service
   */
  boost::shared_ptr<Service> GetService();

  std::string GetIp() {
    return ip;
  }
  int GetPort() {
    return port;
  }

  /**
   * Sets the IP address of the UDP server
   *
   * @param ip The desired IP address of the server
   */
  void SetIp(char const* ip);

  /**
   * Starts the UDP server
   *
   * @param port The port for the UDP server
   */
  void Start(const uint16_t port);

  /**
   * Begins an asynchronous receive with async_receive_from. The payload
   * of the incoming datagram is stored in the _recv_buffer. As soon as
   * the receive operation is complete, a call is made to the handler,
   * UDPServer::ReadHandler
   */
  void StartRecv();

  /**
   * Completion handler for StartRecv(). The received bytes are put into a new Packet
   * which is then checked to see the type of packet we received from the client.
   * Based on the type of packet, it is then handed off to the appropriate response function
   *
   * @param error The result of async_receive_from
   * @param bytes The number of bytes received
   */
  void ReadHandler(const boost::system::error_code& error, std::size_t bytes);

  /**
   * Asynchronous send using async_send_to. Upon completion, a call is made to
   * the handler, SendToHandler.
   *
   * @param p Packet containing the payload to be sent
   *
   */
  void AsyncSendTo(Packet p);

  /**
   * Completion handler for AsyncSendTo.
   *
   * @param error The result of the async_send_to operation
   * @param bytes The number of bytes sent
   */
  void SendToHandler(const boost::system::error_code& error, size_t bytes);

  /**
   * Below functions construct responses to corresponding requests
   * Deserializes the incoming packet to obtain client information
   * Serializes the response information into a new Packet
   * Sends this new Packet back to the client using AsyncSendTo
   *
   * @param p Packet containing the payload from the datagram sent by the client
   */
  void HandleSessionRequest(Packet *p);
  void HandleServerRequest(Packet *p);

private:
  boost::shared_ptr<Service> _service;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
  std::string ip;
  int port;
  char _recv_buffer[512];
};

#endif
