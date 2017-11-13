#pragma once
#include <nan.h>

const char* ToCString(const Nan::Utf8String& value);

// void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch, std::function<void(string)> error_cb);

// v8::Local<v8::Value>  ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
//                    v8::Local<v8::Value> name, bool report_exceptions, bool* failed,std::function<void(string)> error_cb);

// void split(const std::string& s, char delim, std::vector<std::string>& elems);

