#include "websocket_server.h"
#include "v8inspector_client.h"

#ifndef V8_INSPECTOR_EXAMPLE_INSPECTOR_H
#define V8_INSPECTOR_EXAMPLE_INSPECTOR_H


class Inspector {
public:
    Inspector(const v8::Local<v8::Context> &context, const int webSocketPort);
    void startAgent();
private:
    void onMessage(const std::string& message);
    void sendMessage(const std::string& message);

    v8::Handle<v8::Context> context_;
    std::unique_ptr<WebSocketServer> websocket_server_;
    std::unique_ptr<V8InspectorClientImpl> inspector_client_;
};


#endif //V8_INSPECTOR_EXAMPLE_INSPECTOR_H
