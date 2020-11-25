#include <string>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <iomanip>

#define F(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))

#define LEFTROTATE(A, N) (((A) << (N)) | ((A) >> (32 - (N))))

#define r1(a, b, c, d, x, s) \
    a += F(b, c, d) + x;     \
    a = LEFTROTATE(a, s);
#define r2(a, b, c, d, x, s)                    \
    a += G(b, c, d) + x + (uint32_t)0x5A827999; \
    a = LEFTROTATE(a, s);
#define r3(a, b, c, d, x, s)                    \
    a += H(b, c, d) + x + (uint32_t)0x6ED9EBA1; \
    a = LEFTROTATE(a, s);

#define endia(X) ((((X)&0xFF) << 24) | (((X)&0xFF00) << 8) | (((X)&0xFF0000) >> 8) | (((X)&0xFF000000) >> 24))

std::string MD4_MAIN(uint32_t *message, size_t messageLength)
{
    //Where to add size
    size_t shiftingPosition = (messageLength / 4);
    size_t shiftingAmount = 8 * (messageLength % 4);

    //Let x=MessageLength % 64 (modulo 64), If x=0, we pad with 64 0 bytes. If x !=0, then we check if x > 56. If it's the case we pad with n=64-(x-56) or if it's smaller than 56 n = 56-x
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

    //Add the shifting to the padding to get full 32bit size, bcs we will add 0x80 in the same last one if the message length%32!=0
    size_t newMessageLength = (messageLength / 4) + ((paddingLength + shiftingAmount) / 32);
    size_t paddedMessageLength = newMessageLength + 2;

    //new array for the new message
    std::vector<uint32_t> paddedMessage(paddedMessageLength, 0);
    memcpy(&paddedMessage[0], message, messageLength);

    //Padding
    paddedMessage[shiftingPosition] += (0x80 << shiftingAmount);

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength >> 29) & 0xFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength << 3) & 0xFFFFFFFF);

    //MD Buffer
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;
    uint32_t AA, BB, CC, DD;

    //Define X[16]
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
        //Round 1
        for (int i = 0; i < 4; i++)
        {
            r1(A, B, C, D, X[0 + 4 * i], 3);
            r1(D, A, B, C, X[1 + 4 * i], 7);
            r1(C, D, A, B, X[2 + 4 * i], 11);
            r1(B, C, D, A, X[3 + 4 * i], 19);
        }
        //Round 2
        for (int i = 0; i < 4; i++)
        {
            r2(A, B, C, D, X[0 + i], 3);
            r2(D, A, B, C, X[4 + i], 5);
            r2(C, D, A, B, X[8 + i], 9);
            r2(B, C, D, A, X[12 + i], 13);
        }
        //Round 3
        r3(A, B, C, D, X[0], 3);
        r3(D, A, B, C, X[8], 9);
        r3(C, D, A, B, X[4], 11);
        r3(B, C, D, A, X[12], 15);
        r3(A, B, C, D, X[2], 3);
        r3(D, A, B, C, X[10], 9);
        r3(C, D, A, B, X[6], 11);
        r3(B, C, D, A, X[14], 15);
        r3(A, B, C, D, X[1], 3);
        r3(D, A, B, C, X[9], 9);
        r3(C, D, A, B, X[5], 11);
        r3(B, C, D, A, X[13], 15);
        r3(A, B, C, D, X[3], 3);
        r3(D, A, B, C, X[11], 9);
        r3(C, D, A, B, X[7], 11);
        r3(B, C, D, A, X[15], 15);

        //Update register
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    //Process and output hashed message
    std::ostringstream md4Message;
    md4Message << std::hex << std::setfill('0') << std::setw(8) << endia(A);
    md4Message << std::hex << std::setfill('0') << std::setw(8) << endia(B);
    md4Message << std::hex << std::setfill('0') << std::setw(8) << endia(C);
    md4Message << std::hex << std::setfill('0') << std::setw(8) << endia(D);
    return md4Message.str();
}

Napi::Value MD4(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, MD4_MAIN((uint32_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
