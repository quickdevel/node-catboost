#ifndef MODEL_H
#define MODEL_H

#include <napi.h>
#include "includes\wrapped_calcer.h"

class Model : public Napi::ObjectWrap<Model> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Model(const Napi::CallbackInfo& info);

 private:
  Napi::Value Calc(const Napi::CallbackInfo& info);
  Napi::Value CalcMany(const Napi::CallbackInfo& info);

  void FeaturesToVectors(Napi::Array &featuresList, std::vector<float> &floatVector, std::vector<std::string> &stringVector);

  ModelCalcerWrapper *calcer_;
};

#endif
