#include <functional>
#include <v8.h>
#include <libplatform/libplatform.h>
#include "Inspector.h"
#include "v8_inspector_listener_impl.h"

const static int port = 9005;
std::unique_ptr<v8::Platform> v8Platform = nullptr;
std::unique_ptr<Inspector> inspector = nullptr;

int main()
{
    v8::V8::InitializeICU();
    v8Platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(v8Platform.get());
    v8::V8::Initialize();

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    {
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope scope(isolate);
        v8::Handle<v8::Context> context = v8::Context::New(isolate);
        v8::Context::Scope context_scope(context);

        inspector = std::unique_ptr<Inspector>(new Inspector(v8Platform, context, port));

        inspector->addListener(new BaseV8InspectorListenerImpl());
        inspector->startAgent();

    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;

    return 0;
}
