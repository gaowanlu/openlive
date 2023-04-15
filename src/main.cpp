#include <napi.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include "capture_thread/capture_thread.h"
#include "encode_thread/encode_thread.h"

namespace Demo
{
    capture_thread::CaptureThread capture_thread;
    encode_thread::EncodeThread encode_thread;
    std::string mat;

    Napi::Value start(const Napi::CallbackInfo &info)
    {
        capture_thread.open(0); // default dev
        capture_thread.start();
        encode_thread.start(&capture_thread);
        return Napi::Boolean::New(info.Env(), true);
    }

    Napi::Value get_mat(const Napi::CallbackInfo &info)
    {
        mat = encode_thread.getFrame();
        return Napi::String::New(info.Env(), mat.c_str());
    }
};

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "start"), Napi::Function::New<Demo::start>(env));
    exports.Set(Napi::String::New(env, "get_mat"), Napi::Function::New<Demo::get_mat>(env));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
