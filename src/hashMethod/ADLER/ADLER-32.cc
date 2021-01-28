#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>

std::string ADLER32_MAIN(uint8_t *message, size_t messageLength)
{
    uint32_t a = 1;
    uint32_t b = 0;

    for (size_t i = 0; i < messageLength; ++i)
    {
        a = (a + message[i]) % 65521;
        b = (b + a) % 65521;
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ((b << 16) | a);
    return md5Message.str();
}

Napi::Value ADLER32(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, ADLER32_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}