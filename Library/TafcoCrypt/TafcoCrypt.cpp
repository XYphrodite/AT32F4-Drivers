#include "TafcoCrypt.h"

const uint8_t KeySize = 128;
const uint8_t key[] = {
    154, 143, 51, 170, 93, 148, 6, 165, 168, 104, 239, 251, 157, 105, 185, 42, 190,
    164, 221, 193, 47, 155, 76, 171, 187, 77, 229, 113, 27, 199, 217, 69, 216,
    184, 133, 245, 147, 54, 29, 169, 68, 182, 212, 13, 230, 181, 205, 145, 248,
    2, 158, 3, 96, 132, 175, 60, 40, 124, 5, 191, 4, 61, 70, 224, 23,
    202, 50, 22, 31, 201, 24, 37, 63, 144, 127, 198, 112, 180, 19, 134, 7,
    240, 237, 234, 162, 195, 90, 109, 53, 218, 146, 8, 220, 12, 131, 115, 174,
    18, 75, 88, 9, 122, 210, 14, 20, 64, 231, 32, 203, 71, 173, 95, 56,
    151, 225, 156, 97, 247, 17, 160, 150, 177, 86, 211, 139, 238, 49, 116};
// uint8_t *crypto_buff;
uint8_t plaintext[KeySize]{0};

void decrypt_firmware_block(uint8_t *buff)
{
    for (uint8_t i = 0; i < KeySize; ++i)
    {
        plaintext[i] = buff[i];
        // xor
        plaintext[i] = plaintext[i] ^ key[i];
        // shift
        uint8_t shift = key[i] % 8;
        uint8_t neg_shift = 8 - shift;
        uint8_t to_add = plaintext[i] << neg_shift;
        uint8_t shifted = plaintext[i] >> shift;
        plaintext[i] = shifted + to_add;
        // plus
        plaintext[i] -= key[i];
    }

    memcpy(buff, plaintext, KeySize);
}