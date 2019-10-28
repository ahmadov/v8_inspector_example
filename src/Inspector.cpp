#include <iostream>
#include <functional>
#include "Inspector.h"

Inspector::Inspector(const v8::Local<v8::Context> &context, const int webSocketPort) {
    context_ = context;
    websocket_server_ = std::unique_ptr<WebSocketServer>(new WebSocketServer(webSocketPort, std::bind(&Inspector::onMessage, this, std::placeholders::_1)));
    inspector_client_ = std::unique_ptr<V8InspectorClientImpl>(new V8InspectorClientImpl(context_, std::bind(&Inspector::sendMessage, this, std::placeholders::_1)));
}

void Inspector::onMessage(const std::string& message) {
    std::cout << "WebSocket on message: " << message << std::endl;
    std::vector<uint8_t> str(message.begin(), message.end());
    uint8_t *characters = &str[0];
    v8_inspector::StringView protocolMessage(characters, str.size());
    inspector_client_->dispatchProtocolMessage(protocolMessage);
}

void Inspector::sendMessage(const std::string& message) {
    std::cout << "WebSocket send message: " << message << std::endl;
    websocket_server_->sendMessage(message);
}

void Inspector::startAgent() {
    websocket_server_->run();
}
