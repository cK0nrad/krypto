//https://web.archive.org/web/20140411060613if_/http://labs.calyptix.com:80/files/haval-paper.pdf
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include "HAVAL.hpp"
#include <napi.h>
#include "../../check.hpp"

std::string HAVAL5_MAIN(uint32_t *message, size_t messageLength, uint16_t digestSize)
{

    //Where to add size
    size_t shiftingPosition = (messageLength / 4);
    size_t shiftingAmount = 8 * (messageLength % 4);

    //How many array entry to fill the message
    size_t paddingLength = 944;
    if ((messageLength * 8) % 1024 != 0)
    {
        if (((messageLength * 8) % 1024) > 944)
        {
            paddingLength = 512 - (((messageLength * 8) % 1024) - 944);
        }
        else
        {
            paddingLength = 944 - ((messageLength * 8) % 1024);
        }
    }

    //Total size of the message + padding + shiftingAmount + 2*32 entry for the length
    size_t newMessageLength = (messageLength / 4) + ((paddingLength + shiftingAmount) / 32);
    size_t paddedMessageLength = newMessageLength + 2 + 1 /*For PASS, VERSION and DGSTLENG */;

    //Vector that contain message + padding + length
    std::vector<uint32_t> paddedMessage(paddedMessageLength, 0);
    memcpy(&paddedMessage[0], message, messageLength);

    //Append 0x80 at then end of the message (10000000)
    //Padding
    paddedMessage[shiftingPosition] += (1 << (shiftingAmount));

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength >> 29) & 0xFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength << 3) & 0xFFFFFFFF);

    //Append number of passes
    paddedMessage[paddedMessageLength - 3] = ((1) + (5 << 3) + (digestSize << 6)) << 16;

    //Hash calcation
    //For loop of 32 words per rounds.

    //Initialize register
    uint32_t H0 = 0x243F6A88,
             H1 = 0x85A308D3,
             H2 = 0x13198A2E,
             H3 = 0x03707344,
             H4 = 0xA4093822,
             H5 = 0x299F31D0,
             H6 = 0x082EFA98,
             H7 = 0xEC4E6C89;

    uint32_t T[8];
    uint32_t B[32] = {0};

    for (size_t offset = 0; offset < (paddedMessageLength / 32); offset++)
    {
        //Fill B[] with the 32 currents words
        for (size_t j = 0; j < 32; j++)
            B[j] = paddedMessage[offset * 32 + j];

        //Init
        T[0] = H0,
        T[1] = H1,
        T[2] = H2,
        T[3] = H3,
        T[4] = H4,
        T[5] = H5,
        T[6] = H6,
        T[7] = H7;

        //Didn't found any way to use for loop here so I'll do it step by step
        //See the permution table in the pdf, I use H(n) bcs value will change and will give wrong function return if i use T[n] instead
        //Pass 1:

        T[7] = RIGHTROTATE(HAHVAL_F1(H3, H4, H1, H0, H5, H2, H6), 7) + RIGHTROTATE(H7, 11) + B[0];
        T[6] = RIGHTROTATE(HAHVAL_F1(H2, H3, H0, T[7], H4, H1, H5), 7) + RIGHTROTATE(H6, 11) + B[1];
        T[5] = RIGHTROTATE(HAHVAL_F1(H1, H2, T[7], T[6], H3, H0, H4), 7) + RIGHTROTATE(H5, 11) + B[2];
        T[4] = RIGHTROTATE(HAHVAL_F1(H0, H1, T[6], T[5], H2, T[7], H3), 7) + RIGHTROTATE(H4, 11) + B[3];
        T[3] = RIGHTROTATE(HAHVAL_F1(T[7], H0, T[5], T[4], H1, T[6], H2), 7) + RIGHTROTATE(H3, 11) + B[4];
        T[2] = RIGHTROTATE(HAHVAL_F1(T[6], T[7], T[4], T[3], H0, T[5], H1), 7) + RIGHTROTATE(H2, 11) + B[5];
        T[1] = RIGHTROTATE(HAHVAL_F1(T[5], T[6], T[3], T[2], T[7], T[4], H0), 7) + RIGHTROTATE(H1, 11) + B[6];
        T[0] = RIGHTROTATE(HAHVAL_F1(T[4], T[5], T[2], T[1], T[6], T[3], T[7]), 7) + RIGHTROTATE(H0, 11) + B[7];

        T[7] = RIGHTROTATE(HAHVAL_F1(T[3], T[4], T[1], T[0], T[5], T[2], T[6]), 7) + RIGHTROTATE(T[7], 11) + B[8];
        T[6] = RIGHTROTATE(HAHVAL_F1(T[2], T[3], T[0], T[7], T[4], T[1], T[5]), 7) + RIGHTROTATE(T[6], 11) + B[9];
        T[5] = RIGHTROTATE(HAHVAL_F1(T[1], T[2], T[7], T[6], T[3], T[0], T[4]), 7) + RIGHTROTATE(T[5], 11) + B[10];
        T[4] = RIGHTROTATE(HAHVAL_F1(T[0], T[1], T[6], T[5], T[2], T[7], T[3]), 7) + RIGHTROTATE(T[4], 11) + B[11];
        T[3] = RIGHTROTATE(HAHVAL_F1(T[7], T[0], T[5], T[4], T[1], T[6], T[2]), 7) + RIGHTROTATE(T[3], 11) + B[12];
        T[2] = RIGHTROTATE(HAHVAL_F1(T[6], T[7], T[4], T[3], T[0], T[5], T[1]), 7) + RIGHTROTATE(T[2], 11) + B[13];
        T[1] = RIGHTROTATE(HAHVAL_F1(T[5], T[6], T[3], T[2], T[7], T[4], T[0]), 7) + RIGHTROTATE(T[1], 11) + B[14];
        T[0] = RIGHTROTATE(HAHVAL_F1(T[4], T[5], T[2], T[1], T[6], T[3], T[7]), 7) + RIGHTROTATE(T[0], 11) + B[15];

        T[7] = RIGHTROTATE(HAHVAL_F1(T[3], T[4], T[1], T[0], T[5], T[2], T[6]), 7) + RIGHTROTATE(T[7], 11) + B[16];
        T[6] = RIGHTROTATE(HAHVAL_F1(T[2], T[3], T[0], T[7], T[4], T[1], T[5]), 7) + RIGHTROTATE(T[6], 11) + B[17];
        T[5] = RIGHTROTATE(HAHVAL_F1(T[1], T[2], T[7], T[6], T[3], T[0], T[4]), 7) + RIGHTROTATE(T[5], 11) + B[18];
        T[4] = RIGHTROTATE(HAHVAL_F1(T[0], T[1], T[6], T[5], T[2], T[7], T[3]), 7) + RIGHTROTATE(T[4], 11) + B[19];
        T[3] = RIGHTROTATE(HAHVAL_F1(T[7], T[0], T[5], T[4], T[1], T[6], T[2]), 7) + RIGHTROTATE(T[3], 11) + B[20];
        T[2] = RIGHTROTATE(HAHVAL_F1(T[6], T[7], T[4], T[3], T[0], T[5], T[1]), 7) + RIGHTROTATE(T[2], 11) + B[21];
        T[1] = RIGHTROTATE(HAHVAL_F1(T[5], T[6], T[3], T[2], T[7], T[4], T[0]), 7) + RIGHTROTATE(T[1], 11) + B[22];
        T[0] = RIGHTROTATE(HAHVAL_F1(T[4], T[5], T[2], T[1], T[6], T[3], T[7]), 7) + RIGHTROTATE(T[0], 11) + B[23];

        T[7] = RIGHTROTATE(HAHVAL_F1(T[3], T[4], T[1], T[0], T[5], T[2], T[6]), 7) + RIGHTROTATE(T[7], 11) + B[24];
        T[6] = RIGHTROTATE(HAHVAL_F1(T[2], T[3], T[0], T[7], T[4], T[1], T[5]), 7) + RIGHTROTATE(T[6], 11) + B[25];
        T[5] = RIGHTROTATE(HAHVAL_F1(T[1], T[2], T[7], T[6], T[3], T[0], T[4]), 7) + RIGHTROTATE(T[5], 11) + B[26];
        T[4] = RIGHTROTATE(HAHVAL_F1(T[0], T[1], T[6], T[5], T[2], T[7], T[3]), 7) + RIGHTROTATE(T[4], 11) + B[27];
        T[3] = RIGHTROTATE(HAHVAL_F1(T[7], T[0], T[5], T[4], T[1], T[6], T[2]), 7) + RIGHTROTATE(T[3], 11) + B[28];
        T[2] = RIGHTROTATE(HAHVAL_F1(T[6], T[7], T[4], T[3], T[0], T[5], T[1]), 7) + RIGHTROTATE(T[2], 11) + B[29];
        T[1] = RIGHTROTATE(HAHVAL_F1(T[5], T[6], T[3], T[2], T[7], T[4], T[0]), 7) + RIGHTROTATE(T[1], 11) + B[30];
        T[0] = RIGHTROTATE(HAHVAL_F1(T[4], T[5], T[2], T[1], T[6], T[3], T[7]), 7) + RIGHTROTATE(T[0], 11) + B[31];

        //Pass 2:
        T[7] = RIGHTROTATE(HAHVAL_F2(T[6], T[2], T[1], T[0], T[3], T[4], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[5] + 0x452821E6UL;
        T[6] = RIGHTROTATE(HAHVAL_F2(T[5], T[1], T[0], T[7], T[2], T[3], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[14] + 0x38D01377UL;
        T[5] = RIGHTROTATE(HAHVAL_F2(T[4], T[0], T[7], T[6], T[1], T[2], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[26] + 0xBE5466CFUL;
        T[4] = RIGHTROTATE(HAHVAL_F2(T[3], T[7], T[6], T[5], T[0], T[1], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[18] + 0x34E90C6CUL;
        T[3] = RIGHTROTATE(HAHVAL_F2(T[2], T[6], T[5], T[4], T[7], T[0], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[11] + 0xC0AC29B7UL;
        T[2] = RIGHTROTATE(HAHVAL_F2(T[1], T[5], T[4], T[3], T[6], T[7], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[28] + 0xC97C50DDUL;
        T[1] = RIGHTROTATE(HAHVAL_F2(T[0], T[4], T[3], T[2], T[5], T[6], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[7] + 0x3F84D5B5UL;
        T[0] = RIGHTROTATE(HAHVAL_F2(T[7], T[3], T[2], T[1], T[4], T[5], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[16] + 0xB5470917UL;

        T[7] = RIGHTROTATE(HAHVAL_F2(T[6], T[2], T[1], T[0], T[3], T[4], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[0] + 0x9216D5D9UL;
        T[6] = RIGHTROTATE(HAHVAL_F2(T[5], T[1], T[0], T[7], T[2], T[3], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[23] + 0x8979FB1BUL;
        T[5] = RIGHTROTATE(HAHVAL_F2(T[4], T[0], T[7], T[6], T[1], T[2], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[20] + 0xD1310BA6UL;
        T[4] = RIGHTROTATE(HAHVAL_F2(T[3], T[7], T[6], T[5], T[0], T[1], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[22] + 0x98DFB5ACUL;
        T[3] = RIGHTROTATE(HAHVAL_F2(T[2], T[6], T[5], T[4], T[7], T[0], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[1] + 0x2FFD72DBUL;
        T[2] = RIGHTROTATE(HAHVAL_F2(T[1], T[5], T[4], T[3], T[6], T[7], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[10] + 0xD01ADFB7UL;
        T[1] = RIGHTROTATE(HAHVAL_F2(T[0], T[4], T[3], T[2], T[5], T[6], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[4] + 0xB8E1AFEDUL;
        T[0] = RIGHTROTATE(HAHVAL_F2(T[7], T[3], T[2], T[1], T[4], T[5], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[8] + 0x6A267E96UL;

        T[7] = RIGHTROTATE(HAHVAL_F2(T[6], T[2], T[1], T[0], T[3], T[4], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[30] + 0xBA7C9045UL;
        T[6] = RIGHTROTATE(HAHVAL_F2(T[5], T[1], T[0], T[7], T[2], T[3], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[3] + 0xF12C7F99UL;
        T[5] = RIGHTROTATE(HAHVAL_F2(T[4], T[0], T[7], T[6], T[1], T[2], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[21] + 0x24A19947UL;
        T[4] = RIGHTROTATE(HAHVAL_F2(T[3], T[7], T[6], T[5], T[0], T[1], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[9] + 0xB3916CF7UL;
        T[3] = RIGHTROTATE(HAHVAL_F2(T[2], T[6], T[5], T[4], T[7], T[0], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[17] + 0x0801F2E2UL;
        T[2] = RIGHTROTATE(HAHVAL_F2(T[1], T[5], T[4], T[3], T[6], T[7], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[24] + 0x858EFC16UL;
        T[1] = RIGHTROTATE(HAHVAL_F2(T[0], T[4], T[3], T[2], T[5], T[6], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[29] + 0x636920D8UL;
        T[0] = RIGHTROTATE(HAHVAL_F2(T[7], T[3], T[2], T[1], T[4], T[5], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[6] + 0x71574E69UL;

        T[7] = RIGHTROTATE(HAHVAL_F2(T[6], T[2], T[1], T[0], T[3], T[4], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[19] + 0xA458FEA3UL;
        T[6] = RIGHTROTATE(HAHVAL_F2(T[5], T[1], T[0], T[7], T[2], T[3], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[12] + 0xF4933D7EUL;
        T[5] = RIGHTROTATE(HAHVAL_F2(T[4], T[0], T[7], T[6], T[1], T[2], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[15] + 0x0D95748FUL;
        T[4] = RIGHTROTATE(HAHVAL_F2(T[3], T[7], T[6], T[5], T[0], T[1], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[13] + 0x728EB658UL;
        T[3] = RIGHTROTATE(HAHVAL_F2(T[2], T[6], T[5], T[4], T[7], T[0], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[2] + 0x718BCD58UL;
        T[2] = RIGHTROTATE(HAHVAL_F2(T[1], T[5], T[4], T[3], T[6], T[7], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[25] + 0x82154AEEUL;
        T[1] = RIGHTROTATE(HAHVAL_F2(T[0], T[4], T[3], T[2], T[5], T[6], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[31] + 0x7B54A41DUL;
        T[0] = RIGHTROTATE(HAHVAL_F2(T[7], T[3], T[2], T[1], T[4], T[5], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[27] + 0xC25A59B5UL;

        //Pass 3:
        T[7] = RIGHTROTATE(HAHVAL_F3(T[2], T[6], T[0], T[4], T[3], T[1], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[19] + 0x9C30D539UL;
        T[6] = RIGHTROTATE(HAHVAL_F3(T[1], T[5], T[7], T[3], T[2], T[0], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[9] + 0x2AF26013UL;
        T[5] = RIGHTROTATE(HAHVAL_F3(T[0], T[4], T[6], T[2], T[1], T[7], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[4] + 0xC5D1B023UL;
        T[4] = RIGHTROTATE(HAHVAL_F3(T[7], T[3], T[5], T[1], T[0], T[6], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[20] + 0x286085F0UL;
        T[3] = RIGHTROTATE(HAHVAL_F3(T[6], T[2], T[4], T[0], T[7], T[5], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[28] + 0xCA417918UL;
        T[2] = RIGHTROTATE(HAHVAL_F3(T[5], T[1], T[3], T[7], T[6], T[4], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[17] + 0xB8DB38EFUL;
        T[1] = RIGHTROTATE(HAHVAL_F3(T[4], T[0], T[2], T[6], T[5], T[3], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[8] + 0x8E79DCB0UL;
        T[0] = RIGHTROTATE(HAHVAL_F3(T[3], T[7], T[1], T[5], T[4], T[2], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[22] + 0x603A180EUL;

        T[7] = RIGHTROTATE(HAHVAL_F3(T[2], T[6], T[0], T[4], T[3], T[1], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[29] + 0x6C9E0E8BUL;
        T[6] = RIGHTROTATE(HAHVAL_F3(T[1], T[5], T[7], T[3], T[2], T[0], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[14] + 0xB01E8A3EUL;
        T[5] = RIGHTROTATE(HAHVAL_F3(T[0], T[4], T[6], T[2], T[1], T[7], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[25] + 0xD71577C1UL;
        T[4] = RIGHTROTATE(HAHVAL_F3(T[7], T[3], T[5], T[1], T[0], T[6], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[12] + 0xBD314B27UL;
        T[3] = RIGHTROTATE(HAHVAL_F3(T[6], T[2], T[4], T[0], T[7], T[5], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[24] + 0x78AF2FDAUL;
        T[2] = RIGHTROTATE(HAHVAL_F3(T[5], T[1], T[3], T[7], T[6], T[4], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[30] + 0x55605C60UL;
        T[1] = RIGHTROTATE(HAHVAL_F3(T[4], T[0], T[2], T[6], T[5], T[3], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[16] + 0xE65525F3UL;
        T[0] = RIGHTROTATE(HAHVAL_F3(T[3], T[7], T[1], T[5], T[4], T[2], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[26] + 0xAA55AB94UL;

        T[7] = RIGHTROTATE(HAHVAL_F3(T[2], T[6], T[0], T[4], T[3], T[1], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[31] + 0x57489862UL;
        T[6] = RIGHTROTATE(HAHVAL_F3(T[1], T[5], T[7], T[3], T[2], T[0], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[15] + 0x63E81440UL;
        T[5] = RIGHTROTATE(HAHVAL_F3(T[0], T[4], T[6], T[2], T[1], T[7], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[7] + 0x55CA396AUL;
        T[4] = RIGHTROTATE(HAHVAL_F3(T[7], T[3], T[5], T[1], T[0], T[6], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[3] + 0x2AAB10B6UL;
        T[3] = RIGHTROTATE(HAHVAL_F3(T[6], T[2], T[4], T[0], T[7], T[5], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[1] + 0xB4CC5C34UL;
        T[2] = RIGHTROTATE(HAHVAL_F3(T[5], T[1], T[3], T[7], T[6], T[4], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[0] + 0x1141E8CEUL;
        T[1] = RIGHTROTATE(HAHVAL_F3(T[4], T[0], T[2], T[6], T[5], T[3], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[18] + 0xA15486AFUL;
        T[0] = RIGHTROTATE(HAHVAL_F3(T[3], T[7], T[1], T[5], T[4], T[2], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[27] + 0x7C72E993UL;

        T[7] = RIGHTROTATE(HAHVAL_F3(T[2], T[6], T[0], T[4], T[3], T[1], T[5]), 7) + RIGHTROTATE(T[7], 11) + B[13] + 0xB3EE1411UL;
        T[6] = RIGHTROTATE(HAHVAL_F3(T[1], T[5], T[7], T[3], T[2], T[0], T[4]), 7) + RIGHTROTATE(T[6], 11) + B[6] + 0x636FBC2AUL;
        T[5] = RIGHTROTATE(HAHVAL_F3(T[0], T[4], T[6], T[2], T[1], T[7], T[3]), 7) + RIGHTROTATE(T[5], 11) + B[21] + 0x2BA9C55DUL;
        T[4] = RIGHTROTATE(HAHVAL_F3(T[7], T[3], T[5], T[1], T[0], T[6], T[2]), 7) + RIGHTROTATE(T[4], 11) + B[10] + 0x741831F6UL;
        T[3] = RIGHTROTATE(HAHVAL_F3(T[6], T[2], T[4], T[0], T[7], T[5], T[1]), 7) + RIGHTROTATE(T[3], 11) + B[23] + 0xCE5C3E16UL;
        T[2] = RIGHTROTATE(HAHVAL_F3(T[5], T[1], T[3], T[7], T[6], T[4], T[0]), 7) + RIGHTROTATE(T[2], 11) + B[11] + 0x9B87931EUL;
        T[1] = RIGHTROTATE(HAHVAL_F3(T[4], T[0], T[2], T[6], T[5], T[3], T[7]), 7) + RIGHTROTATE(T[1], 11) + B[5] + 0xAFD6BA33UL;
        T[0] = RIGHTROTATE(HAHVAL_F3(T[3], T[7], T[1], T[5], T[4], T[2], T[6]), 7) + RIGHTROTATE(T[0], 11) + B[2] + 0x6C24CF5CUL;

        //Pass 4:
        T[7] = RIGHTROTATE(HAHVAL_F4(T[1], T[5], T[3], T[2], T[0], T[4], T[6]), 7) + RIGHTROTATE(T[7], 11) + B[24] + 0x7A325381UL;
        T[6] = RIGHTROTATE(HAHVAL_F4(T[0], T[4], T[2], T[1], T[7], T[3], T[5]), 7) + RIGHTROTATE(T[6], 11) + B[4] + 0x28958677UL;
        T[5] = RIGHTROTATE(HAHVAL_F4(T[7], T[3], T[1], T[0], T[6], T[2], T[4]), 7) + RIGHTROTATE(T[5], 11) + B[0] + 0x3B8F4898UL;
        T[4] = RIGHTROTATE(HAHVAL_F4(T[6], T[2], T[0], T[7], T[5], T[1], T[3]), 7) + RIGHTROTATE(T[4], 11) + B[14] + 0x6B4BB9AFUL;
        T[3] = RIGHTROTATE(HAHVAL_F4(T[5], T[1], T[7], T[6], T[4], T[0], T[2]), 7) + RIGHTROTATE(T[3], 11) + B[2] + 0xC4BFE81BUL;
        T[2] = RIGHTROTATE(HAHVAL_F4(T[4], T[0], T[6], T[5], T[3], T[7], T[1]), 7) + RIGHTROTATE(T[2], 11) + B[7] + 0x66282193UL;
        T[1] = RIGHTROTATE(HAHVAL_F4(T[3], T[7], T[5], T[4], T[2], T[6], T[0]), 7) + RIGHTROTATE(T[1], 11) + B[28] + 0x61D809CCUL;
        T[0] = RIGHTROTATE(HAHVAL_F4(T[2], T[6], T[4], T[3], T[1], T[5], T[7]), 7) + RIGHTROTATE(T[0], 11) + B[23] + 0xFB21A991UL;

        T[7] = RIGHTROTATE(HAHVAL_F4(T[1], T[5], T[3], T[2], T[0], T[4], T[6]), 7) + RIGHTROTATE(T[7], 11) + B[26] + 0x487CAC60UL;
        T[6] = RIGHTROTATE(HAHVAL_F4(T[0], T[4], T[2], T[1], T[7], T[3], T[5]), 7) + RIGHTROTATE(T[6], 11) + B[6] + 0x5DEC8032UL;
        T[5] = RIGHTROTATE(HAHVAL_F4(T[7], T[3], T[1], T[0], T[6], T[2], T[4]), 7) + RIGHTROTATE(T[5], 11) + B[30] + 0xEF845D5DUL;
        T[4] = RIGHTROTATE(HAHVAL_F4(T[6], T[2], T[0], T[7], T[5], T[1], T[3]), 7) + RIGHTROTATE(T[4], 11) + B[20] + 0xE98575B1UL;
        T[3] = RIGHTROTATE(HAHVAL_F4(T[5], T[1], T[7], T[6], T[4], T[0], T[2]), 7) + RIGHTROTATE(T[3], 11) + B[18] + 0xDC262302UL;
        T[2] = RIGHTROTATE(HAHVAL_F4(T[4], T[0], T[6], T[5], T[3], T[7], T[1]), 7) + RIGHTROTATE(T[2], 11) + B[25] + 0xEB651B88UL;
        T[1] = RIGHTROTATE(HAHVAL_F4(T[3], T[7], T[5], T[4], T[2], T[6], T[0]), 7) + RIGHTROTATE(T[1], 11) + B[19] + 0x23893E81UL;
        T[0] = RIGHTROTATE(HAHVAL_F4(T[2], T[6], T[4], T[3], T[1], T[5], T[7]), 7) + RIGHTROTATE(T[0], 11) + B[3] + 0xD396ACC5UL;

        T[7] = RIGHTROTATE(HAHVAL_F4(T[1], T[5], T[3], T[2], T[0], T[4], T[6]), 7) + RIGHTROTATE(T[7], 11) + B[22] + 0x0F6D6FF3UL;
        T[6] = RIGHTROTATE(HAHVAL_F4(T[0], T[4], T[2], T[1], T[7], T[3], T[5]), 7) + RIGHTROTATE(T[6], 11) + B[11] + 0x83F44239UL;
        T[5] = RIGHTROTATE(HAHVAL_F4(T[7], T[3], T[1], T[0], T[6], T[2], T[4]), 7) + RIGHTROTATE(T[5], 11) + B[31] + 0x2E0B4482UL;
        T[4] = RIGHTROTATE(HAHVAL_F4(T[6], T[2], T[0], T[7], T[5], T[1], T[3]), 7) + RIGHTROTATE(T[4], 11) + B[21] + 0xA4842004UL;
        T[3] = RIGHTROTATE(HAHVAL_F4(T[5], T[1], T[7], T[6], T[4], T[0], T[2]), 7) + RIGHTROTATE(T[3], 11) + B[8] + 0x69C8F04AUL;
        T[2] = RIGHTROTATE(HAHVAL_F4(T[4], T[0], T[6], T[5], T[3], T[7], T[1]), 7) + RIGHTROTATE(T[2], 11) + B[27] + 0x9E1F9B5EUL;
        T[1] = RIGHTROTATE(HAHVAL_F4(T[3], T[7], T[5], T[4], T[2], T[6], T[0]), 7) + RIGHTROTATE(T[1], 11) + B[12] + 0x21C66842UL;
        T[0] = RIGHTROTATE(HAHVAL_F4(T[2], T[6], T[4], T[3], T[1], T[5], T[7]), 7) + RIGHTROTATE(T[0], 11) + B[9] + 0xF6E96C9AUL;

        T[7] = RIGHTROTATE(HAHVAL_F4(T[1], T[5], T[3], T[2], T[0], T[4], T[6]), 7) + RIGHTROTATE(T[7], 11) + B[1] + 0x670C9C61UL;
        T[6] = RIGHTROTATE(HAHVAL_F4(T[0], T[4], T[2], T[1], T[7], T[3], T[5]), 7) + RIGHTROTATE(T[6], 11) + B[29] + 0xABD388F0UL;
        T[5] = RIGHTROTATE(HAHVAL_F4(T[7], T[3], T[1], T[0], T[6], T[2], T[4]), 7) + RIGHTROTATE(T[5], 11) + B[5] + 0x6A51A0D2UL;
        T[4] = RIGHTROTATE(HAHVAL_F4(T[6], T[2], T[0], T[7], T[5], T[1], T[3]), 7) + RIGHTROTATE(T[4], 11) + B[15] + 0xD8542F68UL;
        T[3] = RIGHTROTATE(HAHVAL_F4(T[5], T[1], T[7], T[6], T[4], T[0], T[2]), 7) + RIGHTROTATE(T[3], 11) + B[17] + 0x960FA728UL;
        T[2] = RIGHTROTATE(HAHVAL_F4(T[4], T[0], T[6], T[5], T[3], T[7], T[1]), 7) + RIGHTROTATE(T[2], 11) + B[10] + 0xAB5133A3UL;
        T[1] = RIGHTROTATE(HAHVAL_F4(T[3], T[7], T[5], T[4], T[2], T[6], T[0]), 7) + RIGHTROTATE(T[1], 11) + B[16] + 0x6EEF0B6CUL;
        T[0] = RIGHTROTATE(HAHVAL_F4(T[2], T[6], T[4], T[3], T[1], T[5], T[7]), 7) + RIGHTROTATE(T[0], 11) + B[13] + 0x137A3BE4UL;

        //Pass 5:
        T[7] = RIGHTROTATE(HAHVAL_F5(T[2], T[5], T[0], T[6], T[4], T[3], T[1]), 7) + RIGHTROTATE(T[7], 11) + B[27] + 0xBA3BF050UL;
        T[6] = RIGHTROTATE(HAHVAL_F5(T[1], T[4], T[7], T[5], T[3], T[2], T[0]), 7) + RIGHTROTATE(T[6], 11) + B[3] + 0x7EFB2A98UL;
        T[5] = RIGHTROTATE(HAHVAL_F5(T[0], T[3], T[6], T[4], T[2], T[1], T[7]), 7) + RIGHTROTATE(T[5], 11) + B[21] + 0xA1F1651DUL;
        T[4] = RIGHTROTATE(HAHVAL_F5(T[7], T[2], T[5], T[3], T[1], T[0], T[6]), 7) + RIGHTROTATE(T[4], 11) + B[26] + 0x39AF0176UL;
        T[3] = RIGHTROTATE(HAHVAL_F5(T[6], T[1], T[4], T[2], T[0], T[7], T[5]), 7) + RIGHTROTATE(T[3], 11) + B[17] + 0x66CA593EUL;
        T[2] = RIGHTROTATE(HAHVAL_F5(T[5], T[0], T[3], T[1], T[7], T[6], T[4]), 7) + RIGHTROTATE(T[2], 11) + B[11] + 0x82430E88UL;
        T[1] = RIGHTROTATE(HAHVAL_F5(T[4], T[7], T[2], T[0], T[6], T[5], T[3]), 7) + RIGHTROTATE(T[1], 11) + B[20] + 0x8CEE8619UL;
        T[0] = RIGHTROTATE(HAHVAL_F5(T[3], T[6], T[1], T[7], T[5], T[4], T[2]), 7) + RIGHTROTATE(T[0], 11) + B[29] + 0x456F9FB4UL;

        T[7] = RIGHTROTATE(HAHVAL_F5(T[2], T[5], T[0], T[6], T[4], T[3], T[1]), 7) + RIGHTROTATE(T[7], 11) + B[19] + 0x7D84A5C3UL;
        T[6] = RIGHTROTATE(HAHVAL_F5(T[1], T[4], T[7], T[5], T[3], T[2], T[0]), 7) + RIGHTROTATE(T[6], 11) + B[0] + 0x3B8B5EBEUL;
        T[5] = RIGHTROTATE(HAHVAL_F5(T[0], T[3], T[6], T[4], T[2], T[1], T[7]), 7) + RIGHTROTATE(T[5], 11) + B[12] + 0xE06F75D8UL;
        T[4] = RIGHTROTATE(HAHVAL_F5(T[7], T[2], T[5], T[3], T[1], T[0], T[6]), 7) + RIGHTROTATE(T[4], 11) + B[7] + 0x85C12073UL;
        T[3] = RIGHTROTATE(HAHVAL_F5(T[6], T[1], T[4], T[2], T[0], T[7], T[5]), 7) + RIGHTROTATE(T[3], 11) + B[13] + 0x401A449FUL;
        T[2] = RIGHTROTATE(HAHVAL_F5(T[5], T[0], T[3], T[1], T[7], T[6], T[4]), 7) + RIGHTROTATE(T[2], 11) + B[8] + 0x56C16AA6UL;
        T[1] = RIGHTROTATE(HAHVAL_F5(T[4], T[7], T[2], T[0], T[6], T[5], T[3]), 7) + RIGHTROTATE(T[1], 11) + B[31] + 0x4ED3AA62UL;
        T[0] = RIGHTROTATE(HAHVAL_F5(T[3], T[6], T[1], T[7], T[5], T[4], T[2]), 7) + RIGHTROTATE(T[0], 11) + B[10] + 0x363F7706UL;

        T[7] = RIGHTROTATE(HAHVAL_F5(T[2], T[5], T[0], T[6], T[4], T[3], T[1]), 7) + RIGHTROTATE(T[7], 11) + B[5] + 0x1BFEDF72UL;
        T[6] = RIGHTROTATE(HAHVAL_F5(T[1], T[4], T[7], T[5], T[3], T[2], T[0]), 7) + RIGHTROTATE(T[6], 11) + B[9] + 0x429B023DUL;
        T[5] = RIGHTROTATE(HAHVAL_F5(T[0], T[3], T[6], T[4], T[2], T[1], T[7]), 7) + RIGHTROTATE(T[5], 11) + B[14] + 0x37D0D724UL;
        T[4] = RIGHTROTATE(HAHVAL_F5(T[7], T[2], T[5], T[3], T[1], T[0], T[6]), 7) + RIGHTROTATE(T[4], 11) + B[30] + 0xD00A1248UL;
        T[3] = RIGHTROTATE(HAHVAL_F5(T[6], T[1], T[4], T[2], T[0], T[7], T[5]), 7) + RIGHTROTATE(T[3], 11) + B[18] + 0xDB0FEAD3UL;
        T[2] = RIGHTROTATE(HAHVAL_F5(T[5], T[0], T[3], T[1], T[7], T[6], T[4]), 7) + RIGHTROTATE(T[2], 11) + B[6] + 0x49F1C09BUL;
        T[1] = RIGHTROTATE(HAHVAL_F5(T[4], T[7], T[2], T[0], T[6], T[5], T[3]), 7) + RIGHTROTATE(T[1], 11) + B[28] + 0x075372C9UL;
        T[0] = RIGHTROTATE(HAHVAL_F5(T[3], T[6], T[1], T[7], T[5], T[4], T[2]), 7) + RIGHTROTATE(T[0], 11) + B[24] + 0x80991B7BUL;

        T[7] = RIGHTROTATE(HAHVAL_F5(T[2], T[5], T[0], T[6], T[4], T[3], T[1]), 7) + RIGHTROTATE(T[7], 11) + B[2] + 0x25D479D8UL;
        T[6] = RIGHTROTATE(HAHVAL_F5(T[1], T[4], T[7], T[5], T[3], T[2], T[0]), 7) + RIGHTROTATE(T[6], 11) + B[23] + 0xF6E8DEF7UL;
        T[5] = RIGHTROTATE(HAHVAL_F5(T[0], T[3], T[6], T[4], T[2], T[1], T[7]), 7) + RIGHTROTATE(T[5], 11) + B[16] + 0xE3FE501AUL;
        T[4] = RIGHTROTATE(HAHVAL_F5(T[7], T[2], T[5], T[3], T[1], T[0], T[6]), 7) + RIGHTROTATE(T[4], 11) + B[22] + 0xB6794C3BUL;
        T[3] = RIGHTROTATE(HAHVAL_F5(T[6], T[1], T[4], T[2], T[0], T[7], T[5]), 7) + RIGHTROTATE(T[3], 11) + B[4] + 0x976CE0BDUL;
        T[2] = RIGHTROTATE(HAHVAL_F5(T[5], T[0], T[3], T[1], T[7], T[6], T[4]), 7) + RIGHTROTATE(T[2], 11) + B[1] + 0x04C006BAUL;
        T[1] = RIGHTROTATE(HAHVAL_F5(T[4], T[7], T[2], T[0], T[6], T[5], T[3]), 7) + RIGHTROTATE(T[1], 11) + B[25] + 0xC1A94FB6UL;
        T[0] = RIGHTROTATE(HAHVAL_F5(T[3], T[6], T[1], T[7], T[5], T[4], T[2]), 7) + RIGHTROTATE(T[0], 11) + B[15] + 0x409F60C4UL;

        //Update register now
        H0 += T[0];
        H1 += T[1];
        H2 += T[2];
        H3 += T[3];
        H4 += T[4];
        H5 += T[5];
        H6 += T[6];
        H7 += T[7];
    }

    std::ostringstream md5Message;

    switch (digestSize)
    {
    case 128:
        H3 += ((H7 & 0xFF000000) | (H6 & 0x00FF0000) | (H5 & 0x0000FF00) | (H4 & 0x000000FF));
        H2 += (((H7 & 0x00FF0000) | (H6 & 0x0000FF00) | (H5 & 0x000000FF)) << 8) | ((H4 & 0xFF000000) >> 24);
        H1 += (((H7 & 0x0000FF00) | (H6 & 0x000000FF)) << 16) | (((H5 & 0xFF000000) | (H4 & 0x00FF0000)) >> 16);
        H0 += (((H6 & 0xFF000000) | (H5 & 0x00FF0000) | (H4 & 0x0000FF00)) >> 8) | ((H7 & 0x000000FF) << 24);
        break;
    case 160:
        H4 += ((H7 & 0xFE000000) | (H6 & 0x01F80000) | (H5 & 0x0007F000)) >> 12;
        H3 += ((H7 & 0x01F80000) | (H6 & 0x0007F000) | (H5 & 0x00000FC0)) >> 6;
        H2 += ((H7 & 0x0007F000) | (H6 & 0x00000FC0) | (H5 & 0x0000003F));
        H1 += RIGHTROTATE(((H7 & 0x00000FC0) | (H6 & 0x0000003F) | (H5 & 0xFE000000)), 25);
        H0 += RIGHTROTATE(((H7 & 0x0000003F) | (H6 & 0xFE000000) | (H5 & 0x01F80000)), 19);
        break;
    case 192:
        H5 += ((H7 & 0xFC000000) | (H6 & 0x03E00000)) >> 21;
        H4 += ((H7 & 0x03E00000) | (H6 & 0x001F0000)) >> 16;
        H3 += ((H7 & 0x001F0000) | (H6 & 0x0000FC00)) >> 10;
        H2 += ((H7 & 0x0000FC00) | (H6 & 0x000003E0)) >> 5;
        H1 += ((H7 & 0x000003E0) | (H6 & 0x0000001F));
        H0 += RIGHTROTATE(((H7 & 0x0000001F) | (H6 & 0xFC000000)), 26);
        break;
    case 224:
        H6 += (H7)&0x0000000F;
        H5 += (H7 >> 4) & 0x0000001F;
        H4 += (H7 >> 9) & 0x0000000F;
        H3 += (H7 >> 13) & 0x0000001F;
        H2 += (H7 >> 18) & 0x0000000F;
        H1 += (H7 >> 22) & 0x0000001F;
        H0 += (H7 >> 27) & 0x0000001F;
        break;
    }

    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H0);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H1);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H2);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H3);

    if (digestSize > 128)
        md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H4);

    if (digestSize > 160)
        md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H5);

    if (digestSize > 196)
        md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H6);

    if (digestSize > 224)
        md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H7);

    return md5Message.str();
}

Napi::Value HAVAL5_128(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, HAVAL5_MAIN((uint32_t *)(arg0.c_str()), arg0.length(), 128));
    return reinterpret_cast<Napi::Value &&>(num);
}

Napi::Value HAVAL5_160(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, HAVAL5_MAIN((uint32_t *)(arg0.c_str()), arg0.length(), 160));
    return reinterpret_cast<Napi::Value &&>(num);
}

Napi::Value HAVAL5_192(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, HAVAL5_MAIN((uint32_t *)(arg0.c_str()), arg0.length(), 192));
    return reinterpret_cast<Napi::Value &&>(num);
}

Napi::Value HAVAL5_224(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, HAVAL5_MAIN((uint32_t *)(arg0.c_str()), arg0.length(), 224));
    return reinterpret_cast<Napi::Value &&>(num);
}
Napi::Value HAVAL5_256(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, HAVAL5_MAIN((uint32_t *)(arg0.c_str()), arg0.length(), 256));
    return reinterpret_cast<Napi::Value &&>(num);
}
