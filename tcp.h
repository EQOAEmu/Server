#ifndef TCP_H_
#define TCP_H_

#include "service.h"
#include "packet.h"

/**
 * TCPConnection Class
 *
 * Manages TCP connections
 *
 */
class TCPConnection;

/* TCPServer Class
*
* Manages the TCP server that listens and accepts connections.
* Once a connection is established, it is handed off to the TCPConnection class.
*
*/
class TCPServer;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:
  typedef boost::shared_ptr<TCPConnection> Ptr;
  TCPConnection(boost::shared_ptr<Service> service);

  virtual ~TCPConnection();

  /**
   * Returns the TCP socket
   *
   * @return tcp_socket
   */
  boost::asio::ip::tcp::socket& GetSocket();

  /**
   * Wrapper for AsyncRead
   *
   */
  void Session() {
    AsyncRead();
  }

  /**
   * Ends any pending or current operations
   * Does not close the socket
   *
   */
  void Stop() {
    tcp_socket.cancel();
  }

  boost::asio::ip::tcp::socket tcp_socket;

  void Listen(uint16_t port);

  /**
   * Starts an asynchronous read from the socket. Data is read into recv_buffer
   * Upon completion, a call is made to TCPConnection::ReadHandler
   *
   */
  void AsyncRead();


  /**
   * Completion handler for AsyncRead()
   *
   * @param error The result of the read operation
   * @param bytes The number of bytes transferred
   *
   */
  void ReadHandler(const boost::system::error_code& error, std::size_t bytes);

  /**
   * Starts an asynchronous write to the socket. Data is written from a Packet
   * Upon completion, a call is made to TCPConnection::WriteHandler
   *
   * @param p A pointer to the Packet that will be written to the stream
   *
   */
  void AsyncWrite(Packet *p);

  /**
   * Completion handler for AsyncWrite()
   *
   * @param error The result of the write operation
   * @param bytes The number of bytes written
   *
   */
  void WriteHandler(const boost::system::error_code &error, size_t bytes);

  /**
   * Creates the response to the TCP packet connection request from the client
   * Uses AsyncWrite() to send the response to the client
   *
   */
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

  /**
   * Returns _service
   *
   * @return _service The shared pointer for our io_service
   */
  boost::shared_ptr<Service> GetService();

  /**
   * Wrapper for Listen() and Accept()
   *
   * @param port The port to start our TCP Server
   *
   */
  void Start(const uint16_t &port);

  /**
   * Begin listening on the specified port for incoming connections
   *
   * @param port The port to start listening on
   */
  void Listen(const uint16_t &port);


  /**
   * Begin listening on the specified ip and port for incoming connections
   *
   * @param ip The ip to start listening on
   * @param port The port to start listening on
   */
  void Listen(const std::string &ip, const uint16_t &port);

  /**
   * Starts asynchronous accept
   *
   */
  void Accept();

  /**
   * Completion handler for Accept()
   *
   * @param ec The result of async_accept
   * @param tcpConnection The tcpConnection to begin AsyncRead() on
   */
  void AcceptHandler(const boost::system::error_code &ec, boost::shared_ptr<TCPConnection>
                     tcpConnection);

private:
  boost::shared_ptr<Service> _service;
  boost::asio::ip::tcp::acceptor _acceptor;


};

#endif
