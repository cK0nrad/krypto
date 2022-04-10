//https://tools.ietf.org/html/rfc1319
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include <napi.h>
#include "../../check.hpp"
#include "MD.hpp"

std::string MD2_MAIN(uint8_t *message, size_t messageLength)
{
    //get amount and what to append to the message
    size_t paddingLength = 16 - (messageLength % 16);
    size_t newMessageLength = messageLength + paddingLength;
    size_t paddedMessageLength = newMessageLength + 16;

    //new array for the new message
    std::vector<uint8_t> paddedMessage(paddedMessageLength);
    memcpy(&paddedMessage[0], message, messageLength);

    //Fill the new array with the padding
    for (size_t i = messageLength; i < newMessageLength; i++)
        paddedMessage[i] = paddingLength;
    ;

    //calculate the checksum
    uint8_t checksum[16] = {0};
    int L = 0;

    //For each block of 16 word
    for (size_t offset = 0; offset < (newMessageLength / 16); offset++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            int c = paddedMessage[offset * 16 + j];
            L = MD2_pi[c ^ L] ^ checksum[j];
            checksum[j] = L;
        }
    }
    //Append the checksum
    for (size_t i = 0; i < 16; i++)
    {
        paddedMessage[newMessageLength + i] = checksum[i];
    }

    //MD Buffer
    uint8_t X[48] = {0};

    for (size_t offset = 0; offset < ((paddedMessageLength) / 16); offset++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            X[16 + j] = paddedMessage[offset * 16 + j];
            X[32 + j] = (X[16 + j] ^ X[j]);
        }

        int t = 0;
        for (size_t j = 0; j < 18; j++)
        {
            for (size_t k = 0; k < 48; k++)
            {
                t = X[k] ^ MD2_pi[t];
                X[k] = t;
            }
            t = (t + j) % 256;
        }
    }
    //Process and output hashed message
    std::ostringstream md2Message;
    for (size_t x = 0; x < 16; x++)
    {
        md2Message << std::hex << std::setfill('0') << std::setw(2) << (int)X[x];
    }
    return md2Message.str();
}

Napi::Value MD2(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, MD2_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
