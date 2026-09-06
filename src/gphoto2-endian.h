/* Portable byte-order helpers used by the bundled gphoto PTP code. */

#ifndef __BYTEORDER_H
#define __BYTEORDER_H

#include <stdint.h>
#ifdef _WIN32
# include <winsock2.h>
# include <stdlib.h>
# define swap16(x) _byteswap_ushort((uint16_t)(x))
# define swap32(x) _byteswap_ulong((uint32_t)(x))
# define swap64(x) _byteswap_uint64((uint64_t)(x))
#elif defined(__APPLE__)
# include <arpa/inet.h>
# include <libkern/OSByteOrder.h>
# define swap16(x) OSSwapInt16((uint16_t)(x))
# define swap32(x) OSSwapInt32((uint32_t)(x))
# define swap64(x) OSSwapInt64((uint64_t)(x))
#else
# include <arpa/inet.h>
# include <byteswap.h>
# define swap16(x) bswap_16(x)
# define swap32(x) bswap_32(x)
# define swap64(x) bswap_64(x)
#endif

/* The byte swapping macros have the form: */
/*   EENN[a]toh or htoEENN[a] where EE is be (big endian) or */
/* le (little-endian), NN is 16 or 32 (number of bits) and a, */
/* if present, indicates that the endian side is a pointer to an */
/* array of uint8_t bytes instead of an integer of the specified length. */
/* h refers to the host's ordering method. */

/* So, to convert a 32-bit integer stored in a buffer in little-endian */
/* format into a uint32_t usable on this machine, you could use: */
/*   uint32_t value = le32atoh(&buf[3]); */
/* To put that value back into the buffer, you could use: */
/*   htole32a(&buf[3], value); */

/* Define aliases for the standard byte swapping macros */
/* Arguments to these macros must be properly aligned on natural word */
/* boundaries in order to work properly on all architectures */
#ifndef htobe16
# define htobe16(x) htons(x)
#endif
#ifndef htobe32
# define htobe32(x) htonl(x)
#endif
#ifndef be16toh
# define be16toh(x) ntohs(x)
#endif
#ifndef be32toh
# define be32toh(x) ntohl(x)
#endif

#define HTOBE16(x) (x) = htobe16(x)
#define HTOBE32(x) (x) = htobe32(x)
#define BE32TOH(x) (x) = be32toh(x)
#define BE16TOH(x) (x) = be16toh(x)

/* On little endian machines, these macros are null */
#ifndef htole16
# define htole16(x)      (x)
#endif
#ifndef htole32
# define htole32(x)      (x)
#endif
#ifndef htole64
# define htole64(x)      (x)
#endif
#ifndef le16toh
# define le16toh(x)      (x)
#endif
#ifndef le32toh
# define le32toh(x)      (x)
#endif
#ifndef le64toh
# define le64toh(x)      (x)
#endif

#define HTOLE16(x)      (void) (x)
#define HTOLE32(x)      (void) (x)
#define HTOLE64(x)      (void) (x)
#define LE16TOH(x)      (void) (x)
#define LE32TOH(x)      (void) (x)
#define LE64TOH(x)      (void) (x)

/* These don't have standard aliases */
#ifndef htobe64
# define htobe64(x)      swap64(x)
#endif
#ifndef be64toh
# define be64toh(x)      swap64(x)
#endif

#define HTOBE64(x)      (x) = htobe64(x)
#define BE64TOH(x)      (x) = be64toh(x)

/* Here are some macros to create integers from a byte array */
/* These are used to get and put integers from/into a uint8_t array */
/* with a specific endianness.  This is the most portable way to generate */
/* and read messages to a network or serial device.  Each member of a */
/* packet structure must be handled separately. */

/* Byte access also works for unaligned buffers on ARM. */
static inline uint16_t be16atoh(const void *data)
{
    const uint8_t *p = (const uint8_t *)data;
    return (uint16_t)(((uint16_t)p[0] << 8) | ((uint16_t)p[1] << 0));
}
static inline void htobe16a(void *data, uint16_t value)
{
    uint8_t *p = (uint8_t *)data;
    p[0] = (uint8_t)(value >> 8);
    p[1] = (uint8_t)(value >> 0);
}

/* Byte access also works for unaligned buffers on ARM. */
static inline uint32_t be32atoh(const void *data)
{
    const uint8_t *p = (const uint8_t *)data;
    return (uint32_t)(((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | ((uint32_t)p[3] << 0));
}
static inline void htobe32a(void *data, uint32_t value)
{
    uint8_t *p = (uint8_t *)data;
    p[0] = (uint8_t)(value >> 24);
    p[1] = (uint8_t)(value >> 16);
    p[2] = (uint8_t)(value >> 8);
    p[3] = (uint8_t)(value >> 0);
}

/* Byte access also works for unaligned buffers on ARM. */
static inline uint64_t be64atoh(const void *data)
{
    const uint8_t *p = (const uint8_t *)data;
    return (uint64_t)(((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40) | ((uint64_t)p[3] << 32) | ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) | ((uint64_t)p[6] << 8) | ((uint64_t)p[7] << 0));
}
static inline void htobe64a(void *data, uint64_t value)
{
    uint8_t *p = (uint8_t *)data;
    p[0] = (uint8_t)(value >> 56);
    p[1] = (uint8_t)(value >> 48);
    p[2] = (uint8_t)(value >> 40);
    p[3] = (uint8_t)(value >> 32);
    p[4] = (uint8_t)(value >> 24);
    p[5] = (uint8_t)(value >> 16);
    p[6] = (uint8_t)(value >> 8);
    p[7] = (uint8_t)(value >> 0);
}

/* Byte access also works for unaligned buffers on ARM. */
static inline uint16_t le16atoh(const void *data)
{
    const uint8_t *p = (const uint8_t *)data;
    return (uint16_t)(((uint16_t)p[0] << 0) | ((uint16_t)p[1] << 8));
}
static inline void htole16a(void *data, uint16_t value)
{
    uint8_t *p = (uint8_t *)data;
    p[0] = (uint8_t)(value >> 0);
    p[1] = (uint8_t)(value >> 8);
}

/* Byte access also works for unaligned buffers on ARM. */
static inline uint32_t le32atoh(const void *data)
{
    const uint8_t *p = (const uint8_t *)data;
    return (uint32_t)(((uint32_t)p[0] << 0) | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24));
}
static inline void htole32a(void *data, uint32_t value)
{
    uint8_t *p = (uint8_t *)data;
    p[0] = (uint8_t)(value >> 0);
    p[1] = (uint8_t)(value >> 8);
    p[2] = (uint8_t)(value >> 16);
    p[3] = (uint8_t)(value >> 24);
}

/* Byte access also works for unaligned buffers on ARM. */
static inline uint64_t le64atoh(const void *data)
{
    const uint8_t *p = (const uint8_t *)data;
    return (uint64_t)(((uint64_t)p[0] << 0) | ((uint64_t)p[1] << 8) | ((uint64_t)p[2] << 16) | ((uint64_t)p[3] << 24) | ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) | ((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56));
}
static inline void htole64a(void *data, uint64_t value)
{
    uint8_t *p = (uint8_t *)data;
    p[0] = (uint8_t)(value >> 0);
    p[1] = (uint8_t)(value >> 8);
    p[2] = (uint8_t)(value >> 16);
    p[3] = (uint8_t)(value >> 24);
    p[4] = (uint8_t)(value >> 32);
    p[5] = (uint8_t)(value >> 40);
    p[6] = (uint8_t)(value >> 48);
    p[7] = (uint8_t)(value >> 56);
}

#endif /* __BYTEORDER_H */
