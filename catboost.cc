#include <napi.h>
#include "model.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return Model::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
