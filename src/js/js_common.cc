#include "js_common.h"

const char* ToCString(const Nan::Utf8String& value) {
    return *value ? *value : "<string conversion failed>";
}

// void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch, std::function<void(string)> error_cb) {
//     v8::HandleScope handle_scope(isolate);
//     Nan::Utf8String exception(try_catch->Exception());
//     const char* exception_string = ToCString(exception);
//     v8::Local<v8::Message> message = try_catch->Message();
//     string res_string = "";
//     if (message.IsEmpty()) {
//         // V8 didn't provide any extra information about this error; just
//         // print the exception.
//         res_string += exception_string;
//         res_string += "\n";
//     } else {
//         // Print (filename):(line number): (message).
//         Nan::Utf8String filename(message->GetScriptOrigin().ResourceName());
//         v8::Local<v8::Context> context(isolate->GetCurrentContext());
//         const char* filename_string = ToCString(filename);
//         int linenum = message->GetLineNumber(context).FromJust();
//         res_string += filename_string;
//         res_string += ":";
//         res_string += std::to_string(linenum);
//         res_string += ": ";
//         res_string += exception_string;
//         res_string += "\n";
//         // Print line of source code.
//         Nan::Utf8String sourceline(
//                 message->GetSourceLine(context).ToLocalChecked());
//         const char* sourceline_string = ToCString(sourceline);
//         res_string += sourceline_string;
//         res_string += "\n";
//         // Print wavy underline (GetUnderline is deprecated).
//         int start = message->GetStartColumn(context).FromJust();
//         for (int i = 0; i < start; i++) {
//             res_string += " ";
//         }
//         int end = message->GetEndColumn(context).FromJust();
//         for (int i = start; i < end; i++) {
//             res_string += "^";
//         }
//         res_string += "\n";
//         v8::Local<v8::Value> stack_trace_string;
//         if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
//             stack_trace_string->IsString() &&
//             v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0) {
//             Nan::Utf8String stack_trace(stack_trace_string);
//             res_string += ToCString(stack_trace);
//             res_string += "\n";
//         }
//     }
//     error_cb(res_string);
// }



// // Executes a string within the current v8 context.
// v8::Local<v8::Value> ExecuteString(v8::Isolate* isolate, v8::Local<v8::String> source,
//                    v8::Local<v8::Value> name, bool report_exceptions, bool* failed, std::function<void(string)> error_cb) {
//     v8::Isolate::Scope isolate_scope(isolate);
//     v8::EscapableHandleScope handle_scope(isolate);
//     v8::TryCatch try_catch(isolate);
//     v8::ScriptOrigin origin(name);
//     v8::Local<v8::Context> context(isolate->GetCurrentContext());
//     v8::Local<v8::Script> script;
//     if (!v8::Script::Compile(context, source, &origin).ToLocal(&script)) {
//         Nan::Utf8String str(try_catch.Exception());
//         // Print errors that happened during compilation.
//         if (report_exceptions)
//             ReportException(isolate, &try_catch, error_cb);
//         *failed = true;
//         return handle_scope.Escape(Null(isolate));
//     } else {
//         v8::Local<v8::Value> result;
//         if (!script->Run(context).ToLocal(&result)) {
//             assert(try_catch.HasCaught());
//             // Print errors that happened during execution.
//             if (report_exceptions)
//                 ReportException(isolate, &try_catch, error_cb);
//             *failed = true;
//             return handle_scope.Escape(Null(isolate));
//         } else {
//             assert(!try_catch.HasCaught());
//             *failed = false;
//             return handle_scope.Escape(result);
//         }
//     }
// }

// void split(const std::string &s, char delim, std::vector<std::string> &elems) {
//     std::stringstream ss;
//     ss.str(s);
//     std::string item;
//     while (std::getline(ss, item, delim)) {
//         elems.push_back(item);
//     }
// }