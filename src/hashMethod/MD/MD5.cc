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

#ifndef endia
#define endia(X) ((((X)&0xFF) << 24) | (((X)&0xFF00) << 8) | (((X)&0xFF0000) >> 8) | (((X)&0xFF000000) >> 24))
#endif

// Constants are the integer part of the sines of integers (in radians) * 2^32.
const uint32_t k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

// r specifies the per-round shift amounts
const uint32_t r[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

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
                fghi = (A + fghi + k[i] + X[i]);
                fghi = B + ((fghi << r[i]) | fghi >> (32 - r[i]));
            }
            else if (i < 32)
            {
                //Round 2
                fghi = (D & B) | ((~D) & C);
                fghi = (A + fghi + k[i] + X[(5 * i + 1) % 16]);
                fghi = B + ((fghi << r[i]) | fghi >> (32 - r[i]));
            }
            else if (i < 48)
            {
                //Round 3
                fghi = B ^ C ^ D;
                fghi = (A + fghi + k[i] + X[(3 * i + 5) % 16]);
                fghi = B + ((fghi << r[i]) | fghi >> (32 - r[i]));
            }
            else
            {
                //Round 4
                fghi = C ^ (B | (~D));
                fghi = (A + fghi + k[i] + X[(7 * i) % 16]);
                fghi = B + ((fghi << r[i]) | fghi >> (32 - r[i]));
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
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(A);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(B);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(C);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(D);
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