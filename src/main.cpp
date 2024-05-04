#include <napi.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <vector>
#include <cstdio>
#include "conf/conf.h"
#include "capture_thread/capture_thread.h"
#include "encode_thread/encode_thread.h"

using std::string;
using std::vector;

namespace OpenLive
{
    capture_thread::CaptureThread capture_thread;
    encode_thread::EncodeThread encode_thread;
    std::string mat;

    class MyAsyncWorker : public Napi::AsyncWorker
    {
    public:
        MyAsyncWorker(Napi::Function &callback) : Napi::AsyncWorker(callback)
        {
        }
        ~MyAsyncWorker()
        {
            // std::cout << "~MyAsyncWorker()" << std::endl;
        }
        void Execute() override
        {
            mat = encode_thread.getFrame(); // update frame
        }
        void OnOK() override
        {
            Napi::HandleScope scope(Napi::AsyncWorker::Env());
	    Napi::String str2js = Napi::String::New(Napi::AsyncWorker::Env(), mat.c_str(), mat.size());
            Callback().Call({str2js});
        }
        void OnError(const Napi::Error& e) override
	{
	    Napi::HandleScope scope(Napi::AsyncWorker::Env());
	    Napi::String str2js = Napi::String::New(Napi::AsyncWorker::Env(), "", 0);
	    Callback().Call({str2js});
        }
    };

    Napi::Value start(const Napi::CallbackInfo &info)
    {
        bool isDev = true;
        try
        {
            std::stoi(Conf::Conf::getPath());
        }
        catch (const std::invalid_argument &e)
        {
            isDev = false;
        }
        if (isDev)
        {
            capture_thread.open(std::stoi(Conf::Conf::getPath()));
        }
        else
        {
            capture_thread.open(Conf::Conf::getPath());
        }
        capture_thread.start();
        encode_thread.start(&capture_thread);
        return Napi::Boolean::New(info.Env(), true);
    }

    Napi::Value getMat(const Napi::CallbackInfo &info)
    {
        Napi::Env env = info.Env();
        Napi::Function cb = info[0].As<Napi::Function>();
        MyAsyncWorker *worker = new MyAsyncWorker(cb);
        worker->Queue();
        return env.Undefined();
    }

    Napi::Value setConf(const Napi::CallbackInfo &info)
    {
        Napi::Env env = info.Env();
        if (info.Length() < 1)
        {
            Napi::TypeError::New(env, "info.Length() < 1").ThrowAsJavaScriptException();
            return Napi::Boolean::New(info.Env(), false);
        }
        if (!info[0].IsObject())
        {
            Napi::TypeError::New(env, "!info[0].IsObject()").ThrowAsJavaScriptException();
            return Napi::Boolean::New(info.Env(), false);
        }
        Napi::Object obj = info[0].As<Napi::Object>();
        vector<string> keys;
        const Napi::Array &arr = obj.GetPropertyNames();
        for (uint32_t i = 0; i < arr.Length(); i++)
        {
            Napi::Value value = arr[i];
            std::string str = value.As<Napi::String>();
            keys.push_back(str);
        }
        for (const auto &key : keys)
        {
            Napi::Value val = obj.Get(key);
            if (val.IsNumber())
            {
                int64_t num = val.As<Napi::Number>().Int64Value();
                Conf::Conf::setConf(key, num);
            }
            else if (val.IsString())
            {
                std::string str = val.As<Napi::String>().Utf8Value();
                Conf::Conf::setConf(key, str);
            }
            else
            {
                printf("%s is not a number or string\n", key.c_str());
            }
        }
        return Napi::Boolean::New(info.Env(), true);
    }
};

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "start"), Napi::Function::New<OpenLive::start>(env));
    exports.Set(Napi::String::New(env, "getMat"), Napi::Function::New<OpenLive::getMat>(env));
    exports.Set(Napi::String::New(env, "setConf"), Napi::Function::New<OpenLive::setConf>(env));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
