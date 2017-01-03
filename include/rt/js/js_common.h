//
// Created by Dan Lewis on 1/2/17.
//

#ifndef ROSEBUD_JS_COMMON_H
#define ROSEBUD_JS_COMMON_H

#include "config.h"

const char* ToCString(const v8::String::Utf8Value& value);

void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);

bool ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
                   v8::Local<v8::Value> name, bool report_exceptions);

#endif //ROSEBUD_JS_COMMON_H
