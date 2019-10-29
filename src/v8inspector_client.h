#include <v8.h>
#include <v8-inspector.h>
#include "v8inspector_channel.h"

#ifndef V8INSPECTORCLIENTIMPL_H
#define V8INSPECTORCLIENTIMPL_H

class V8InspectorClientImpl final: public v8_inspector::V8InspectorClient {
public:
  V8InspectorClientImpl(const std::unique_ptr<v8::Platform> &platform, const v8::Local<v8::Context>& context, const std::function<void(std::string)> &onResponse);

  void dispatchProtocolMessage(const v8_inspector::StringView &message_view);

private:
  v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

  static const int kContextGroupId = 1;
  v8::Platform* platform_;
  std::unique_ptr<v8_inspector::V8Inspector> inspector_;
  std::unique_ptr<v8_inspector::V8InspectorSession> session_;
  std::unique_ptr<V8InspectorChannelImp> channel_;
  v8::Isolate* isolate_;
  v8::Handle<v8::Context> context_;
};

#endif // V8INSPECTORCLIENTIMPL_H
