/*
http://www.cs.technion.ac.il/~biham/Reports/Tiger/tiger/node3.html
*/
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include <bitset>
#include "TIGER.h"

std::string TIGER3_MAIN(uint8_t *message, size_t messageLength, size_t digestSize)
{
    //Where to add size
    size_t shiftingPosition = (messageLength / 8);
    size_t shiftingAmount = 8 * (messageLength % 8);

    //
    size_t paddingLength = 448;
    if ((messageLength * 8) % 512 != 0)
    {
        if (((messageLength * 8) % 512) > 448)
        {
            paddingLength = 512 - (((messageLength * 8) % 512) - 448);
        }
        else
        {
            paddingLength = 448 - ((messageLength * 8) % 512);
        }
    }

    //Add the shifting to the padding to get full 64bit size, bcs we will add 0x80 in the same last one if the message length%64!=0
    size_t newMessageLength = (messageLength / 8) + ((paddingLength + shiftingAmount) / 64);
    size_t paddedMessageLength = newMessageLength + 1;

    //new array for the new message
    std::vector<uint64_t> paddedMessage(paddedMessageLength, 0);
    memcpy(&paddedMessage[0], message, messageLength);

    //Append 0x80 at then end of the message (10000000)
    //Padding
    paddedMessage[shiftingPosition] += (((uint64_t)1) << (shiftingAmount));

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength << 3) & 0xFFFFFFFFFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength >> 61) & 0xFFFFFFFFFFFFFFFF);

    uint64_t a = 0x0123456789ABCDEF,
             b = 0xFEDCBA9876543210,
             c = 0xF096A5B4C3B2E187,
             aa, bb, cc;
    uint64_t W[8] = {0};

    //For each block of 512 bits (8*[64bits words])

    for (size_t offset = 0; offset < ((paddedMessageLength) / 8); offset++)
    {
        for (size_t i = 0; i < 8; i++)
        {
            W[i] = (paddedMessage[offset * 8 + i]);
        }

        //Save ABC
        aa = a,
        bb = b,
        cc = c;

        //Passes
        TRIGER_ROUND(a, b, c, W[0], 5);
        TRIGER_ROUND(b, c, a, W[1], 5);
        TRIGER_ROUND(c, a, b, W[2], 5);
        TRIGER_ROUND(a, b, c, W[3], 5);
        TRIGER_ROUND(b, c, a, W[4], 5);
        TRIGER_ROUND(c, a, b, W[5], 5);
        TRIGER_ROUND(a, b, c, W[6], 5);
        TRIGER_ROUND(b, c, a, W[7], 5);

        //Key schedule
        W[0] -= W[7] ^ 0xA5A5A5A5A5A5A5A5;
        W[1] ^= W[0];
        W[2] += W[1];
        W[3] -= W[2] ^ ((~W[1]) << 19);
        W[4] ^= W[3];
        W[5] += W[4];
        W[6] -= W[5] ^ ((~W[4]) >> 23);
        W[7] ^= W[6];
        W[0] += W[7];
        W[1] -= W[0] ^ ((~W[7]) << 19);
        W[2] ^= W[1];
        W[3] += W[2];
        W[4] -= W[3] ^ ((~W[2]) >> 23);
        W[5] ^= W[4];
        W[6] += W[5];
        W[7] -= W[6] ^ 0x0123456789ABCDEF;

        //Pass :    a->c,
        //          b->a,
        //          c->b
        TRIGER_ROUND(c, a, b, W[0], 7);
        TRIGER_ROUND(a, b, c, W[1], 7);
        TRIGER_ROUND(b, c, a, W[2], 7);
        TRIGER_ROUND(c, a, b, W[3], 7);
        TRIGER_ROUND(a, b, c, W[4], 7);
        TRIGER_ROUND(b, c, a, W[5], 7);
        TRIGER_ROUND(c, a, b, W[6], 7);
        TRIGER_ROUND(a, b, c, W[7], 7);

        //Key schedule

        W[0] -= W[7] ^ 0xA5A5A5A5A5A5A5A5;
        W[1] ^= W[0];
        W[2] += W[1];
        W[3] -= W[2] ^ ((~W[1]) << 19);
        W[4] ^= W[3];
        W[5] += W[4];
        W[6] -= W[5] ^ ((~W[4]) >> 23);
        W[7] ^= W[6];
        W[0] += W[7];
        W[1] -= W[0] ^ ((~W[7]) << 19);
        W[2] ^= W[1];
        W[3] += W[2];
        W[4] -= W[3] ^ ((~W[2]) >> 23);
        W[5] ^= W[4];
        W[6] += W[5];
        W[7] -= W[6] ^ 0x0123456789ABCDEF;

        //Pass :    a->c,
        //          b->a,
        //          c->b

        TRIGER_ROUND(b, c, a, W[0], 9);
        TRIGER_ROUND(c, a, b, W[1], 9);
        TRIGER_ROUND(a, b, c, W[2], 9);
        TRIGER_ROUND(b, c, a, W[3], 9);
        TRIGER_ROUND(c, a, b, W[4], 9);
        TRIGER_ROUND(a, b, c, W[5], 9);
        TRIGER_ROUND(b, c, a, W[6], 9);
        TRIGER_ROUND(c, a, b, W[7], 9);

        //feedforward
        a ^= (uint64_t)aa;
        b -= (uint64_t)bb;
        c += (uint64_t)cc;
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(16) << (((uint64_t)ENDIAN((uint32_t)a) << 32) | ((ENDIAN((uint32_t)(a >> 32)))));
    md5Message << std::hex << std::setfill('0') << std::setw(16) << (((uint64_t)ENDIAN((uint32_t)b) << 32) | ((ENDIAN((uint32_t)(b >> 32)))));
    switch (digestSize)
    {
    case 160:
        md5Message << std::hex << std::setfill('0') << std::setw(8) << (ENDIAN((uint32_t)c));
        break;
    case 192:
        md5Message << std::hex << std::setfill('0') << std::setw(16) << (((uint64_t)ENDIAN((uint32_t)c) << 32) | ((ENDIAN((uint32_t)(c >> 32)))));
        break;
    default:
        break;
    }
    return md5Message.str();
}

Napi::Value TIGER3_128(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, TIGER3_MAIN((uint8_t *)(arg0.c_str()), arg0.length(), 128));
    return reinterpret_cast<Napi::Value &&>(num);
}
Napi::Value TIGER3_160(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, TIGER3_MAIN((uint8_t *)(arg0.c_str()), arg0.length(), 160));
    return reinterpret_cast<Napi::Value &&>(num);
}
Napi::Value TIGER3_192(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, TIGER3_MAIN((uint8_t *)(arg0.c_str()), arg0.length(), 192));
    return reinterpret_cast<Napi::Value &&>(num);
}
