#include <iostream>
#include <functional>
#include "Inspector.h"
#include "utils.h"

Inspector::Inspector(const std::unique_ptr<v8::Platform> &platform, const v8::Local<v8::Context> &context, const int webSocketPort) {
    context_ = context;
    websocket_server_ = std::unique_ptr<WebSocketServer>(new WebSocketServer(webSocketPort, std::bind(&Inspector::onMessage, this, std::placeholders::_1)));
    inspector_client_ = std::unique_ptr<V8InspectorClientImpl>(new V8InspectorClientImpl(platform, context_, std::bind(&Inspector::sendMessage, this, std::placeholders::_1)));
}

void Inspector::onMessage(const std::string& message) {
    std::cout << "CDT message: " << message << std::endl;
    v8_inspector::StringView protocolMessage = convertToStringView(message);
    inspector_client_->dispatchProtocolMessage(protocolMessage);
    v8::Local<v8::Object> jsonObject = parseJson(context_, message);
    if (!jsonObject.IsEmpty()) {
        std::string method = getPropertyFromJson(context_->GetIsolate(), jsonObject, "method");
        std::cout << "Method: " << method << std::endl;
    }
}

void Inspector::sendMessage(const std::string& message) {
    std::cout << "Message to frontend: " << message << std::endl;
    websocket_server_->sendMessage(message);
}

void Inspector::startAgent() {
    websocket_server_->run();
}

void Inspector::addFileForInspection(const std::string &filePath) {
    std::string fileContent = readFileContent(filePath);

    if (fileContent.length() == 0) {
        exit(1);
    }
    const auto isolate_ = context_->GetIsolate();
    v8::Local<v8::Script> script;
    v8::TryCatch tryCatch(isolate_);
    v8::Local<v8::String> source = v8::String::NewFromUtf8(
        isolate_,
        fileContent.c_str(),
        v8::NewStringType::kNormal
    ).ToLocalChecked();

    if (!compileScript(source, filePath, script, tryCatch)) {
        std::cerr << "could not compile the script" << std::endl;
        std::cerr << "Exception: " << getExceptionMessage(isolate_, tryCatch.Exception()) << std::endl;
        exit(1);
    }
    if (!executeScript(script, tryCatch)) {
        std::cerr << "could not execute the script" << std::endl;
        std::cerr << "Exception: " << getExceptionMessage(isolate_, tryCatch.Exception()) << std::endl;
        exit(1);
    }
}

bool Inspector::compileScript(const v8::Local<v8::String> &source, const std::string &filePath, v8::Local<v8::Script> &script, const v8::TryCatch &tryCatch) {
    const auto isolate_ = context_->GetIsolate();
    v8::ScriptOrigin scriptOrigin = v8::ScriptOrigin(
            v8::String::NewFromUtf8(
                    isolate_,
                    ("file://" + filePath).c_str(),
                    v8::NewStringType::kNormal
            ).ToLocalChecked()
    );
    v8::MaybeLocal<v8::Script> script_ = v8::Script::Compile(context_, source, &scriptOrigin);
    if (!script_.IsEmpty()) {
        script = script_.ToLocalChecked();
    }
    return !tryCatch.HasCaught();
}

bool Inspector::executeScript(const v8::Local<v8::Script> &script, const v8::TryCatch &tryCatch) {
    script->Run(context_);
    return !tryCatch.HasCaught();
}