#include <iostream>
#include "v8inspector_client.h"
#include "utils.h"

V8InspectorClientImpl::V8InspectorClientImpl(const v8::Local<v8::Context> &context, const std::function<void(std::string)> &onResponse) {
    isolate_ = context->GetIsolate();
    channel_.reset(new V8InspectorChannelImp(isolate_, onResponse));
    inspector_ = v8_inspector::V8Inspector::create(isolate_, this);
    session_ = inspector_->connect(kContextGroupId, channel_.get(), v8_inspector::StringView());
    context->SetAlignedPointerInEmbedderData(1, this);

    v8_inspector::StringView contextName = convertToStringView("inspector");
    inspector_->contextCreated(v8_inspector::V8ContextInfo(context, kContextGroupId, contextName));
}

void V8InspectorClientImpl::dispatchProtocolMessage(const v8_inspector::StringView &message_view) {
    session_->dispatchProtocolMessage(message_view);
}
