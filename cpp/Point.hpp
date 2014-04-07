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
#endif

using Point = std::array<std::uint32_t, 2>;

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

#endif // POINT_HPP
