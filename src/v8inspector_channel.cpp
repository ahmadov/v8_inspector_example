#include <functional>
#include "v8inspector_channel.h"
#include "utils.h"

V8InspectorChannelImp::V8InspectorChannelImp(v8::Isolate *isolate, const std::function<void(std::string)> &onResponse) {
    isolate_ = isolate;
    onResponse_ = onResponse;
}

void V8InspectorChannelImp::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) {
    const std::string response = convertToString(isolate_, message->string());
    onResponse_(response);
}

void V8InspectorChannelImp::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) {
    const std::string notification = convertToString(isolate_, message->string());
    onResponse_(notification);
}

void V8InspectorChannelImp::flushProtocolNotifications() {
    // flush protocol notification
}
