#include <napi.h>

bool check(const Napi::CallbackInfo& info){
    if (info.Length() < 1) {
        Napi::TypeError::New(info.Env(), "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return false;
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
        return false;
    }

    return true;
}