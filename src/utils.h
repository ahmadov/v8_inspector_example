#include <v8.h>
#include <v8-inspector.h>

#ifndef UTILS_H
#define UTILS_H

static inline std::string convertToString(v8::Isolate* isolate, const v8_inspector::StringView stringView) {
  int length = static_cast<int>(stringView.length());
  v8::Local<v8::String> message = (
        stringView.is8Bit()
          ? v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t*>(stringView.characters8()), v8::NewStringType::kNormal, length)
          : v8::String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t*>(stringView.characters16()), v8::NewStringType::kNormal, length)
      ).ToLocalChecked();
  v8::String::Utf8Value utf8Value(isolate, message->ToString(isolate));
  std::string res(*utf8Value);
  return res;
}

static inline v8_inspector::StringView convertToStringView(const std::string &str) {
  std::vector<uint8_t> strV(str.begin(), str.end());
  uint8_t *characters = &strV[0];
  v8_inspector::StringView stringView(characters, strV.size());
  return stringView;
}

#endif // UTILS_H
