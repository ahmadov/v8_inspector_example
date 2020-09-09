#include <v8.h>
#include "v8inspector_listener.h"
#include "utils.h"

#ifndef BASEV8INSPECTORLISTENERIMPL_H
#define BASEV8INSPECTORLISTENERIMPL_H

class WASMV8InspectorListenerImpl final: public V8InspectorListener {
public:
    void onConnected(const v8::Local<v8::Context>& context) override {
        const std::string filePath = "../example/wasm-example.js";
        runScript(context, filePath);
    }
};

#endif //BASEV8INSPECTORLISTENERIMPL_H
