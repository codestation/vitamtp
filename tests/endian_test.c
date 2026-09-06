#include <string.h>
#include "gphoto2-endian.h"

int main(void)
{
    /* Use char (which can be signed) and an unaligned address. */
    char buffer[10] = {0};
    const unsigned char le[] = {0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x81};
    const unsigned char be[] = {0x81, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    uint64_t value = UINT64_C(0x8123456789abcdef);
    htole64a(buffer + 1, value);
    if (memcmp(buffer + 1, le, 8) || le64atoh(buffer + 1) != value)
        return 1;
    htobe64a(buffer + 1, value);
    if (memcmp(buffer + 1, be, 8) || be64atoh(buffer + 1) != value)
        return 2;
    /* The old macros shifted a 32-bit input by up to 56 bits. */
    htole64a(buffer + 1, UINT32_C(0x89abcdef));
    if (le64atoh(buffer + 1) != UINT64_C(0x89abcdef))
        return 3;
    htole32a(buffer + 1, UINT32_C(0x89abcdef));
    if (le32atoh(buffer + 1) != UINT32_C(0x89abcdef))
        return 4;
    htobe32a(buffer + 1, UINT32_C(0x89abcdef));
    if (be32atoh(buffer + 1) != UINT32_C(0x89abcdef))
        return 5;
    htole16a(buffer + 1, UINT16_C(0xabcd));
    if (le16atoh(buffer + 1) != UINT16_C(0xabcd))
        return 6;
    htobe16a(buffer + 1, UINT16_C(0xabcd));
    return be16atoh(buffer + 1) != UINT16_C(0xabcd);
}
