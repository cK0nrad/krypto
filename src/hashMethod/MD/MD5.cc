/*
inspired highly by:
https://github.com/pod32g/MD5/blob/master/md5.c
*/
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include "MD.hpp"
#include <napi.h>
#include "../../check.hpp"

std::string MD5_MAIN(uint32_t *message, size_t messageLength)
{
    //Where to add size
    size_t shiftingPosition = (messageLength / 4);
    size_t shiftingAmount = 8 * (messageLength % 4);
    //How many array entry to fill the message
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
    //Total size of the message + padding + shiftingAmount + 2*32 entry for the length
    size_t newMessageLength = (messageLength / 4) + ((paddingLength + shiftingAmount) / 32);
    size_t paddedMessageLength = newMessageLength + 2;

    //Vector that contain message + padding + length
    std::vector<uint32_t> paddedMessage(paddedMessageLength, 0);
    memcpy(&paddedMessage[0], message, messageLength);

    //Append 0x80 at then end of the message (10000000)
    //Padding
    paddedMessage[shiftingPosition] += (0x80 << shiftingAmount);

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength >> 29) & 0xFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength << 3) & 0xFFFFFFFF);

    //Hash calculation
    //For loop of 16 words per rounds.
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;
    uint32_t AA, BB, CC, DD, fghi, tempa;
    uint32_t X[16] = {0};
    for (size_t offset = 0; offset < (paddedMessageLength / 16); offset++)
    {
        //Fill X[] with the 16 currents words
        for (size_t j = 0; j < 16; j++)
        {
            X[j] = paddedMessage[offset * 16 + j];
        }
        //Save A B C & D
        AA = A;
        BB = B;
        CC = C;
        DD = D;
        //There's 64 round so we will loop through these 64 round and check where we are

        for (size_t i = 0; i < 64; i++)
        {

            //If i < 16 => Round 1; 16 < i < 32 => Round 2; 32 < i < 48 => Round 3; 48 < i < 64 => Round 4;
            if (i < 16)
            {
                //Round 1
                fghi = (B & C) | ((~B) & D);
                fghi = (A + fghi + MD5_k[i] + X[i]);
                fghi = B + ((fghi << MD5_r[i]) | fghi >> (32 - MD5_r[i]));
            }
            else if (i < 32)
            {
                //Round 2
                fghi = (D & B) | ((~D) & C);
                fghi = (A + fghi + MD5_k[i] + X[(5 * i + 1) % 16]);
                fghi = B + ((fghi << MD5_r[i]) | fghi >> (32 - MD5_r[i]));
            }
            else if (i < 48)
            {
                //Round 3
                fghi = B ^ C ^ D;
                fghi = (A + fghi + MD5_k[i] + X[(3 * i + 5) % 16]);
                fghi = B + ((fghi << MD5_r[i]) | fghi >> (32 - MD5_r[i]));
            }
            else
            {
                //Round 4
                fghi = C ^ (B | (~D));
                fghi = (A + fghi + MD5_k[i] + X[(7 * i) % 16]);
                fghi = B + ((fghi << MD5_r[i]) | fghi >> (32 - MD5_r[i]));
            }
            //Set a,b,c,d for the next loop
            tempa = D;
            D = C;
            C = B;
            B = fghi;
            A = tempa;
        }
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }
    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(A);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(B);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(C);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(D);
    return md5Message.str();
}

Napi::Value MD5(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, MD5_MAIN((uint32_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}