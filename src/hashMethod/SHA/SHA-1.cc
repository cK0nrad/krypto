/*
https://www.ietf.org/rfc/rfc3174.txt
*/
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include <bitset>

std::string SHA1_MAIN(uint32_t *message, size_t messageLength)
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

    // memcpy(&paddedMessage[0], message, messageLength);
    for (int i = 0; i <= (messageLength + (messageLength % 4)) / 4; i++)
    {
        paddedMessage[i] = (message[i] & 0xFF000000) >> 24;
        paddedMessage[i] |= (message[i] & 0x00FF0000) >> 8;
        paddedMessage[i] |= (message[i] & 0x0000FF00) << 8;
        paddedMessage[i] |= (message[i] & 0x000000FF) << 24;
    }
    /* for (std::vector<uint8_t>::const_iterator i = paddedMessage.begin(); i != paddedMessage.end(); ++i)
        std::cout << std::bitset<8>(*i) << ' ';*/
    //Append 0x80 at then end of the message (10000000)
    //Padding
    paddedMessage[shiftingPosition] += (0x80 << (24 - shiftingAmount));

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength << 3) & 0xFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength >> 29) & 0xFFFFFFFF);

    //Hash calculation
    //For loop of 16 words per rounds.

    //Initialize register
    uint32_t H0 = 0x67452301;
    uint32_t H1 = 0xefcdab89;
    uint32_t H2 = 0x98badcfe;
    uint32_t H3 = 0x10325476;
    uint32_t H4 = 0xC3D2E1F0;

    uint32_t A, B, C, D, E, K, temp, f;
    uint32_t W[80];
    size_t i;
    for (size_t offset = 0; offset < (paddedMessageLength / 16); offset++)
    {
        //Original message
        for (i = 0; i < 16; i++)
        {
            W[i] = (paddedMessage[offset * 16 + i]);
        }

        //Expension
        for (i = 16; i < 80; i++)
        {
            W[i] = (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]);
            W[i] = ((W[i] << 1) | (W[i] >> (32 - 1)));
        }
        A = H0;
        B = H1;
        C = H2;
        D = H3;
        E = H4;

        for (i = 0; i < 80; i++)
        {
            if (i < 20)
            {
                //IF Function
                K = 0x5A827999;
                f = ((B & C) | ((~B) & D));
            }
            else if (i < 40)
            {
                //XOR Function
                K = 0x6ED9EBA1;
                f = B ^ C ^ D;
            }
            else if (i < 60)
            {
                //MAJ Function
                K = 0x8F1BBCDC;
                f = (B & C) | (B & D) | (C & D);
            }
            else if (i < 80)
            {
                //XOR Function
                K = 0xCA62C1D6;
                f = B ^ C ^ D;
            }
            temp = ((A << 5) | A >> (32 - 5)) + f + E + W[i] + K;
            E = D;
            D = C;
            C = ((B << 30) | B >> (32 - 30));
            B = A;
            A = temp;
        }

        H0 += A;
        H1 += B;
        H2 += C;
        H3 += D;
        H4 += E;
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H0);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H1);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H2);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H3);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H4);
    return md5Message.str();
}

Napi::Value SHA1(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, SHA1_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
