#include <string>
#include <napi.h>
#include "MD.hpp"
#include "../../check.hpp"

std::string MD5X2_MAIN(std::string message)
{
    return MD5_MAIN(((uint32_t *)MD5_MAIN((uint32_t *)message.c_str(), message.length()).c_str()), 32);
}
std::string MD5X3_MAIN(std::string message)
{
    return MD5_MAIN((uint32_t *)MD5_MAIN(((uint32_t *)MD5_MAIN((uint32_t *)message.c_str(), message.length()).c_str()), 32).c_str(), 32);
}
std::string MD5X4_MAIN(std::string message)
{
    return MD5_MAIN((uint32_t *)MD5_MAIN((uint32_t *)MD5_MAIN(((uint32_t *)MD5_MAIN((uint32_t *)message.c_str(), message.length()).c_str()), 32).c_str(), 32).c_str(), 32);
}
std::string MD5X5_MAIN(std::string message)
{
    return MD5_MAIN((uint32_t *)(uint32_t *)MD5_MAIN((uint32_t *)MD5_MAIN((uint32_t *)MD5_MAIN(((uint32_t *)MD5_MAIN((uint32_t *)message.c_str(), message.length()).c_str()), 32).c_str(), 32).c_str(), 32).c_str(), 32);
}

Napi::Value MD5X2(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, MD5X2_MAIN(arg0));
    return reinterpret_cast<Napi::Value &&>(num);
}

Napi::Value MD5X3(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, MD5X3_MAIN(arg0));
    return reinterpret_cast<Napi::Value &&>(num);
}

Napi::Value MD5X4(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, MD5X4_MAIN(arg0));
    return reinterpret_cast<Napi::Value &&>(num);
}

Napi::Value MD5X5(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, MD5X5_MAIN(arg0));
    return reinterpret_cast<Napi::Value &&>(num);
}