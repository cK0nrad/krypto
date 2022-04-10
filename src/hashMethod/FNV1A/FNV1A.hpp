#ifndef FNV1A
void mulOverflow(uint64_t *a, uint64_t *b, uint64_t *overflow);
void addOverflow(uint64_t *a, uint64_t *b, uint64_t *overflow);

void mul128(uint64_t *prime, uint64_t *offset);
void mul256(uint64_t *prime, uint64_t *offset);
void mul512(uint64_t *prime, uint64_t *offset);
void mul1024(uint64_t *prime, uint64_t *offset);

#define FNV1A
#endif