#include "model.h"

Napi::Object Model::Init(Napi::Env env, Napi::Object exports) {
	Napi::HandleScope scope(env);

	Napi::Function func = DefineClass(env, "Model", {
		InstanceMethod("calc", &Model::Calc),
		InstanceMethod("calcMany", &Model::CalcMany)
	});

	exports.Set("Model", func);
	return exports;
}

Model::Model(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Model>(info) {
	Napi::Env env = info.Env();
	Napi::HandleScope scope(env);

	if (info.Length() < 1 || !info[0].IsString()) {
		Napi::TypeError::New(env, "Model path expected").ThrowAsJavaScriptException();
	}

	try
	{
		this->calcer_ = new ModelCalcerWrapper(info[0].ToString().Utf8Value());
	}
	catch(const std::exception& e)
	{
		Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();
	}
}

Napi::Value Model::Calc(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	std::vector<float> floatFeatures;
	std::vector<std::string> catFeatures;

	if (info.Length() < 1 || !info[0].IsArray()) {
		Napi::TypeError::New(env, "Array of features expected").ThrowAsJavaScriptException();
	}

	double result = 0;
	try
	{
		Napi::Array features = info[0].As<Napi::Array>();
		this->FeaturesToVectors(features, floatFeatures, catFeatures);
		result = this->calcer_->Calc(floatFeatures, catFeatures);
	}
	catch(const std::exception& e)
	{
		Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();
	}

	return Napi::Number::New(env, result);
}

Napi::Value Model::CalcMany(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	std::vector<std::vector<float>> allFloatFeatures;
	std::vector<std::vector<std::string>> allCatFeatures;

	if (info.Length() < 1 || !info[0].IsArray()) {
		Napi::TypeError::New(env, "Array of objects expected").ThrowAsJavaScriptException();
	}

	Napi::Array objectsList = info[0].As<Napi::Array>();

	for(size_t i = 0; i < objectsList.Length(); i++) {
		std::vector<float> floatFeatures;
		std::vector<std::string> catFeatures;

		Napi::Value object = objectsList.Get(i);

		if (!object.IsArray()) {
			Napi::TypeError::New(env, "Array of features expected").ThrowAsJavaScriptException();
		}

		try
		{
			Napi::Array features = info[0].As<Napi::Array>();
			this->FeaturesToVectors(features, floatFeatures, catFeatures);
		}
		catch(const std::exception& e)
		{
			Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();
		}

		allFloatFeatures.push_back(floatFeatures);
		allCatFeatures.push_back(catFeatures);
	}

	Napi::Array results = Napi::Array::New(env);
	try
	{
		std::vector<double> resultsVector = this->calcer_->Calc(allFloatFeatures, allCatFeatures);

		for (size_t i = 0; i < resultsVector.size(); i++) {
			results[i] = Napi::Number::New(env, resultsVector[i]);
		}
	}
	catch(const std::exception& e)
	{
		Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();
	}

	return results;
}

void Model::FeaturesToVectors(Napi::Array &featuresList, std::vector<float> &floatVector, std::vector<std::string> &stringVector) {
	for (size_t i = 0; i < featuresList.Length(); i++) {
		Napi::Value feature = featuresList.Get(i);

		if (feature.IsNumber()) {
			floatVector.push_back((float) feature.ToNumber().DoubleValue());
		}
		else if (feature.IsString()) {
			stringVector.push_back(feature.ToString().Utf8Value());
		}
		else {
			throw std::runtime_error("Features must be Number or String");
		}
	}
}
