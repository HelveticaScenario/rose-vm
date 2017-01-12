#pragma once

const char* ToCString(const v8::String::Utf8Value& value);

void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);

v8::Local<v8::Value>  ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
                   v8::Local<v8::Value> name, bool report_exceptions, bool* failed);

void split(const std::string& s, char delim, std::vector<std::string>& elems);

