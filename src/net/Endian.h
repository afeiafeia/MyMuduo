#ifndef ENDIAN_H
#define ENDIAN_H
#include <endian.h>
namespace afa
{
    class Endian
    {
    public:

        static uint16_t HostToNetwork16(uint16_t num)
        {
            return be16toh(num);
        }

        static uint16_t NetworkToHost16(uint16_t num)
        {
            return htobe16(num);
        }

        static uint32_t HostToNetwork32(uint32_t num)
        {
            return be32toh(num);
        }

        static uint32_t NetworkToHost32(uint32_t num)
        {
            return htobe32(num);
        }

        static uint64_t HostToNetwork64(uint64_t num)
        {
            return be64toh(num);
        }

        static uint64_t NetworkToHost64(uint64_t num)
        {
            return htobe64(num);
        }


    };
}

#endif