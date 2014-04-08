#ifndef POINT_HPP
#define POINT_HPP

#include <cstdint>
#include <array>

// ApEk,
// NoAvailableAlias
// this code is public domain

#ifdef _MSC_VER // THANKS OBAMA
  #ifndef and
    #define and &&
  #endif
  #ifndef or
    #define or ||
  #endif
#endif

using Point = std::array<std::size_t, 2>;

#define USE_PREMATURE_OPTIMIZATION

template <typename PT> struct morton
{
    inline bool operator()(PT const& lhs, PT const& rhs) const
    {
#ifndef USE_PREMATURE_OPTIMIZATION
        std::size_t x = 0;
        std::size_t j = 0;

        for(std::size_t k = 0; k < 2; ++k)
        {
            std::size_t y = lhs[k] ^ rhs[k];

            if (x < y and x < (x ^ y))
            {
                j = k;
                x = y;
            }
        }
        return lhs[j] < rhs[j];
#else
        std::size_t x = lhs[0] ^ rhs[0];
        std::size_t y = lhs[1] ^ rhs[1];

        if ((x < y) and (x < (x ^ y)))
        {
            return lhs[1] < rhs[1];
        }
        return lhs[0] < rhs[0];
#endif
    }
};

//-----------------------------------------------------------------------------
// following code derived from:
// https://gist.github.com/s-l-teichmann/4014673
// https://gist.github.com/s-l-teichmann/4014664

namespace
{

const std::size_t SIZE = sizeof(std::size_t) * CHAR_BIT;
const std::size_t BITS = SIZE / 2;
const std::size_t MSB = 2 * BITS - 1;
const std::size_t HI_MASK = 1 << (BITS + 1);

const std::size_t _000_ = 0;
const std::size_t _001_ = 1;
const std::size_t _010_ = 2;
const std::size_t _011_ = 3;
const std::size_t _100_ = 4;
const std::size_t _101_ = 5;

const std::size_t MASK = 0xAAAAAAAAAAAAAAAA;
const std::size_t FULL = 0xFFFFFFFFFFFFFFFF;

std::size_t setbits(std::size_t p, std::size_t v)
{
    std::size_t mask = (MASK >> (MSB - p)) & (~(FULL << p) & FULL);
    return (v | mask) & ~(1 << p) & FULL;
}
std::size_t usetbit(std::size_t p, std::size_t v)
{
    std::size_t mask = ~(MASK >> (MSB - p)) & FULL;
    return (v & mask) | (1 << p);
}

std::size_t z_encode(Point p)
{
    std::size_t x = p[0];
    std::size_t y = p[1];

    std::size_t r = 0;

    for (std::size_t b = 1, m = 1; m != HI_MASK; m <<= 1)
    {
        if (x & m) r |= b; b <<= 1;
        if (y & m) r |= b; b <<= 1;
    }
    return r;
}
Point z_decode(std::size_t r)
{
    std::size_t x = 0;
    std::size_t y = 0;

    for (std::size_t b = 1, m = 1; m != HI_MASK; m <<= 1)
    {
        if (r & b) x |= m; b <<= 1;
        if (r & b) y |= m; b <<= 1;
    }
    return {{ x, y }};
}

} // anonymous namespace ------------------------------------------------------

template <typename PT> PT bigmin(PT const& min, PT const& max, PT const& rhs)
{
    std::size_t minz = z_encode(min);
    std::size_t maxz = z_encode(max);
    std::size_t code = z_encode(rhs);

    std::size_t retv = maxz;

    for (std::size_t p = MSB, mask = 1 << MSB; mask != 0; mask >>= 1, --p)
    {
        std::size_t v = (code & mask) ? _100_ : _000_;

        if (minz & mask) v |= _010_;
        if (maxz & mask) v |= _001_;

        switch (v)
        {
            case _001_:
                retv = usetbit(p, minz);
                maxz = setbits(p, maxz);
        break;
            case _011_:
                return z_decode(minz);
            case _100_:
                return z_decode(retv);
            case _101_:
                minz = usetbit(p, minz);
        }
    }
    return z_decode(retv);
}

#endif // POINT_HPP
