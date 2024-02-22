#include <iostream>
#include <libplatform/libplatform.h>
#include <v8.h>

const char *javascript_code = R"(
    // Your JavaScript code here
    let message = "Hello, from JavaScript!";
    message;
)";

int main()
{
  auto PLATFORM = v8::platform::NewDefaultPlatform();
  v8::V8::InitializeICU();
  v8::V8::InitializeExternalStartupData("");
  PLATFORM = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(PLATFORM.get());
  v8::V8::Initialize();

  v8::Isolate::CreateParams create_params;
  v8::Isolate *isolate = v8::Isolate::New(create_params);

  {
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript_code, v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    v8::String::Utf8Value utf8(isolate, result);
    std::cout << "Result: " << *utf8 << std::endl;
  }

  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::DisposePlatform();

  return 0;
}
