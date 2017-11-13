#include <nan.h>
#include "src/js/js.h"

void InitAll(v8::Local<v8::Object> exports) {
    RosebudJS::Init(exports);
}
  
NODE_MODULE(rosebud, InitAll)

