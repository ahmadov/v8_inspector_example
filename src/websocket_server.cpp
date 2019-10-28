#include <functional>
#include "websocket_server.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

WebSocketServer::WebSocketServer(int port, std::function<void(std::string)> onMessage)
{
    port_ = port;
    onMessage_ = std::move(onMessage);
}

void WebSocketServer::run() {
    server_ = std::unique_ptr<server>(new server());
    server_->set_access_channels(websocketpp::log::alevel::all);
    server_->clear_access_channels(websocketpp::log::alevel::frame_payload);
    server_->init_asio();
    server_->set_message_handler(bind(&WebSocketServer::onMessage, this, ::_1, ::_2));
    startListening();
}

void WebSocketServer::sendMessage(const std::string &message)
{
    try {
        websocketpp::lib::error_code ec;
        server_->send(hdl_, message, websocketpp::frame::opcode::text, ec);
    } catch (websocketpp::exception const &e) {
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}

void WebSocketServer::startListening()
{
    try {
        server_->listen(port_);
        printListeningNotification();
        server_->start_accept();
        server_->run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
    std::cerr << "WebSocket server could not start listening on port: " << port_ << std::endl;
}

void WebSocketServer::onMessage(websocketpp::connection_hdl hdl, const message_ptr& msg)
{
    const std::string message = msg->get_payload();
    hdl_ = std::move(hdl);
    onMessage_(message);
}

void WebSocketServer::printListeningNotification() {
    std::cout << "WebSocket based Inspector Agent started" << std::endl;
    std::cout << "Open the following link in your Chrome/Chromium browser: chrome-devtools://devtools/bundled/inspector.html?experiments=true&v8only=true&ws=127.0.0.1:" << port_ << std::endl;
}
