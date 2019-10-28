#include <functional>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

class WebSocketServer
{
public:
  WebSocketServer(int port, std::function<void(std::string)> onMessage);

  void run();
  void sendMessage(const std::string &message);
private:
  void startListening();
  void printListeningNotification();
  void onMessage(websocketpp::connection_hdl hdl, const message_ptr& msg);

  int port_;
  std::function<void(std::string)> onMessage_;
  std::unique_ptr<server> server_ = nullptr;
  websocketpp::connection_hdl hdl_;
};

#endif // WEBSOCKETSERVER_H
