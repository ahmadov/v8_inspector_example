#include <functional>
#include <v8.h>
#include <libplatform/libplatform.h>
#include "Inspector.h"
#include "v8_inspector_listener_wasm_impl.h"

const static int port = 9005;
std::unique_ptr<v8::Platform> v8Platform = nullptr;
std::unique_ptr<Inspector> inspector = nullptr;

void FetchWASM(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::String::Utf8Value filename(args.GetIsolate(), args[0]);
    if (*filename == nullptr) {
        std::cerr << "Error loading file" << std::endl;
        return;
    }

    std::ifstream file(*filename, std::ios::binary);
    if (!file.is_open()) {
        std:: cerr << "Could not open the file: " << *filename << std::endl;
        return;
    }

    file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);
    char * buffer = new char[length];
    // read data as a block:
    file.read (buffer, length);
    if (file)
        std::cout << "all characters read successfully.";
    else
        std::cout << "error: only " << file.gcount() << " could be read";
    file.close();
    v8::Local<v8::ArrayBuffer> binary = v8::ArrayBuffer::New(args.GetIsolate(), buffer, length);
    args.GetReturnValue().Set(binary);
}

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

        v8::Local<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New(isolate);
        global_template->Set(
                isolate,
                "fetchWASM",
                v8::FunctionTemplate::New(isolate, FetchWASM)
        );

        v8::Handle<v8::Context> context = v8::Context::New(isolate, nullptr, global_template);
        v8::Context::Scope context_scope(context);

        inspector = std::unique_ptr<Inspector>(new Inspector(v8Platform, context, port));

        inspector->addListener(new WASMV8InspectorListenerImpl());
        inspector->startAgent();

    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;

    return 0;
}
