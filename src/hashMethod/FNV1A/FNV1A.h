#ifndef FNV1A
void mulOverflow(uint64_t *a, uint64_t *b, uint64_t *overflow)
{
    *overflow = (((*a >> 8) * *b) >> 56 & 0x00000000000000FF) |
                (((*a >> 16) * *b) >> 48 & 0x000000000000FF00) |
                (((*a >> 24) * *b) >> 40 & 0x0000000000FF0000) |
                (((*a >> 32) * *b) >> 32 & 0x00000000FF000000) |
                (((*a >> 40) * *b) >> 24 & 0x000000FF00000000) |
                (((*a >> 48) * *b) >> 16 & 0x0000FF0000000000) |
                (((*a >> 56) * *b) >> 8 & 0x00FF000000000000);
}
void addOverflow(uint64_t *a, uint64_t *b, uint64_t *overflow)
{
    *overflow = ((*a + *b) < *a);
}

void mul1024(uint64_t *prime, uint64_t *offset)
{
    uint64_t overlow, intermediate_value;

    offset[0] = offset[0] * prime[15];
    offset[0] += offset[10] * prime[5];
    mulOverflow(&offset[1], &prime[15], &overlow);
    offset[0] += overlow;
    mulOverflow(&offset[11], &prime[5], &overlow);
    offset[0] += overlow;

    offset[1] = offset[1] * prime[15];
    intermediate_value = offset[11] * prime[5];
    addOverflow(&offset[1], &intermediate_value, &overlow);
    offset[0] += overlow;
    offset[1] += intermediate_value;
    mulOverflow(&offset[2], &prime[15], &overlow);
    offset[1] += overlow;
    mulOverflow(&offset[12], &prime[5], &overlow);
    offset[1] += overlow;

    offset[2] = offset[2] * prime[15];
    intermediate_value = offset[12] * prime[5];
    addOverflow(&offset[2], &intermediate_value, &overlow);
    offset[1] += overlow;
    offset[2] += intermediate_value;
    mulOverflow(&offset[3], &prime[15], &overlow);
    offset[2] += overlow;
    mulOverflow(&offset[13], &prime[5], &overlow);
    offset[2] += overlow;

    offset[3] = offset[3] * prime[15];
    intermediate_value = offset[13] * prime[5];
    addOverflow(&offset[3], &intermediate_value, &overlow);
    offset[2] += overlow;
    offset[3] += intermediate_value;
    mulOverflow(&offset[4], &prime[15], &overlow);
    offset[3] += overlow;
    mulOverflow(&offset[14], &prime[5], &overlow);
    offset[3] += overlow;

    offset[4] = offset[4] * prime[15];
    intermediate_value = offset[14] * prime[5];
    addOverflow(&offset[4], &intermediate_value, &overlow);
    offset[3] += overlow;
    offset[4] += intermediate_value;
    mulOverflow(&offset[5], &prime[15], &overlow);
    offset[4] += overlow;
    mulOverflow(&offset[15], &prime[5], &overlow);
    offset[4] += overlow;

    offset[5] = offset[5] * prime[15];
    intermediate_value = offset[15] * prime[5];
    addOverflow(&offset[5], &intermediate_value, &overlow);
    offset[4] += overlow;
    offset[5] += intermediate_value;
    mulOverflow(&offset[6], &prime[15], &overlow);
    offset[5] += overlow;

    offset[6] = offset[6] * prime[15];
    mulOverflow(&offset[7], &prime[15], &overlow);
    offset[6] += overlow;

    offset[7] = offset[7] * prime[15];
    mulOverflow(&offset[8], &prime[15], &overlow);
    offset[7] += overlow;

    offset[8] = offset[8] * prime[15];
    mulOverflow(&offset[9], &prime[15], &overlow);
    offset[8] += overlow;

    offset[9] = offset[9] * prime[15];
    mulOverflow(&offset[10], &prime[15], &overlow);
    offset[9] += overlow;

    offset[10] = offset[10] * prime[15];
    mulOverflow(&offset[11], &prime[15], &overlow);
    offset[10] += overlow;

    offset[11] = offset[11] * prime[15];
    mulOverflow(&offset[12], &prime[15], &overlow);
    offset[11] += overlow;

    offset[12] = offset[12] * prime[15];
    mulOverflow(&offset[13], &prime[15], &overlow);
    offset[12] += overlow;

    offset[13] = offset[13] * prime[15];
    mulOverflow(&offset[14], &prime[15], &overlow);
    offset[13] += overlow;

    offset[14] = offset[14] * prime[15];
    mulOverflow(&offset[15], &prime[15], &overlow);
    offset[14] += overlow;

    offset[15] = offset[15] * prime[15];
}

void mul512(uint64_t *prime, uint64_t *offset)
{
    uint64_t overlow, intermediate_value;

    offset[0] = offset[0] * prime[7];
    offset[0] += offset[5] * prime[2];
    mulOverflow(&offset[1], &prime[7], &overlow);
    offset[0] += overlow;
    mulOverflow(&offset[6], &prime[2], &overlow);
    offset[0] += overlow;

    offset[1] = offset[1] * prime[7];
    intermediate_value = offset[6] * prime[2];
    addOverflow(&offset[1], &intermediate_value, &overlow);
    offset[0] += overlow;
    offset[1] += intermediate_value;
    mulOverflow(&offset[2], &prime[7], &overlow);
    offset[1] += overlow;
    mulOverflow(&offset[7], &prime[2], &overlow);
    offset[1] += overlow;

    offset[2] = offset[2] * prime[7];
    intermediate_value = offset[7] * prime[2];
    addOverflow(&offset[2], &intermediate_value, &overlow);
    offset[1] += overlow;
    offset[2] += offset[7] * prime[2];
    mulOverflow(&offset[3], &prime[7], &overlow);
    offset[2] += overlow;

    offset[3] = offset[3] * prime[7];
    mulOverflow(&offset[4], &prime[7], &overlow);
    offset[3] += overlow;

    offset[4] = offset[4] * prime[7];
    mulOverflow(&offset[5], &prime[7], &overlow);
    offset[4] += overlow;

    offset[5] = offset[5] * prime[7];
    mulOverflow(&offset[6], &prime[7], &overlow);
    offset[5] += overlow;

    offset[6] = offset[6] * prime[7];
    mulOverflow(&offset[7], &prime[7], &overlow);
    offset[6] += overlow;

    offset[7] = offset[7] * prime[7];
}

void mul256(uint64_t *prime, uint64_t *offset)
{
    uint64_t overlow, intermediate_value;

    //[ABCD]*[EFGH] => offset[0]
    offset[0] = offset[0] * prime[3];
    offset[0] += offset[2] * prime[1];

    mulOverflow(&offset[3], &prime[1], &overlow);
    offset[0] += overlow;

    mulOverflow(&offset[1], &prime[3], &overlow);
    offset[0] += overlow;

    offset[1] = offset[1] * prime[3];
    intermediate_value = offset[3] * prime[1];
    addOverflow(&offset[1], &intermediate_value, &overlow);
    offset[0] += overlow;
    offset[1] += intermediate_value;

    mulOverflow(&offset[2], &prime[3], &overlow);
    offset[1] += overlow;

    offset[2] = (offset[2]) * (prime[3]);
    mulOverflow(&offset[3], &prime[3], &overlow);
    offset[2] += overlow;

    offset[3] = offset[3] * prime[3];
}

void mul128(uint64_t *prime, uint64_t *offset)
{
    *(offset) = (((*prime) * *(offset + 1)) + (*(prime + 1) * (*offset)));

    //Whenever there's overflow, we add it. If no overflow it will be: [] += 0; so np
    *(offset) += (((*(offset + 1) >> 32) * *(prime + 1)) >> 32);
    *(offset + 1) = (*(prime + 1) * *(offset + 1));
}
#define FNV1A
#endif