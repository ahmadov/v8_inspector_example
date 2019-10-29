#include "websocket_server.h"
#include "v8inspector_client.h"

#ifndef V8_INSPECTOR_EXAMPLE_INSPECTOR_H
#define V8_INSPECTOR_EXAMPLE_INSPECTOR_H


class Inspector {
public:
    Inspector(const std::unique_ptr<v8::Platform> &platform, const v8::Local<v8::Context> &context, int webSocketPort);
    void startAgent();
    void addFileForInspection(const std::string &filePath);
private:
    void onMessage(const std::string& message);
    void sendMessage(const std::string& message);
    bool compileScript(const v8::Local<v8::String> &source, const std::string &filename, v8::Local<v8::Script> &script, const v8::TryCatch &tryCatch);
    bool executeScript(const v8::Local<v8::Script> &script, const v8::TryCatch &tryCatch);

    v8::Handle<v8::Context> context_;
    std::unique_ptr<WebSocketServer> websocket_server_;
    std::unique_ptr<V8InspectorClientImpl> inspector_client_;
};


#endif //V8_INSPECTOR_EXAMPLE_INSPECTOR_H
