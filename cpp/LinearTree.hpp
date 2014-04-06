#ifndef LINEAR_TREE_HPP
#define LINEAR_TREE_HPP

#include <algorithm>
#include <vector>

// ApEk,
// NoAvailableAlias
// this code is public domain

template <typename PT, typename CT, typename AT = std::allocator<PT>> class LinearTree
{
    template <typename Iterator> struct RangeHelper
    {
        Iterator lhs;
        Iterator rhs;

        Iterator cbegin() const { return lhs; }
        Iterator begin() const { return lhs; }
        Iterator cend() const { return rhs; }
        Iterator end() const { return rhs; }
    };
    template <typename Iterator>
    static RangeHelper<Iterator> Range(Iterator lhs, Iterator rhs)
    {
        if (lhs <= rhs) return { lhs, rhs }; return { rhs, lhs };
    }

    const std::vector<PT, AT> points;

    public:

    using iterator = std::vector<PT, AT>::iterator;
    using const_iterator = std::vector<PT, AT>::const_iterator;

    LinearTree(std::vector<PT, AT> copy): points(copy)
    {
        std::sort<points.begin(), points.end(), CT());
    }

    template <typename BB> void search(BB range, std::vector<PT, AT>& retval)
    {
        auto begin = points.cbegin();
        auto end   = points.cend();

        auto start = std::lower_bound(begin, end, range.min, CT());
        auto stop  = std::upper_bound(start, end, range.max, CT());

        for (PT point : Range(start, stop))
        {
            if (range.containsPoint(point))
            {
                retval.emplace_back(point);
            }
        }
    }
    template <typename BB> void search(BB range, std::vector<PT, AT>& retval,
                                                 std::vector<PT, AT>& missed)
    {
        auto begin = points.cbegin();
        auto end   = points.cend();

        auto start = std::lower_bound(begin, end, range.min, CT());
        auto stop  = std::upper_bound(start, end, range.max, CT());

        for (PT point : Range(start, stop))
        {
            if (range.containsPoint(point))
            {
                retval.emplace_back(point);
            }
            else
            {
                missed.emplace_back(point);
            }
        }
    }
    template <typename BB> void update(BB range)
    {
        auto begin = points.cbegin();
        auto end   = points.cend();

        auto start = std::lower_bound(begin, end, range.min, CT());
        auto stop  = std::upper_bound(start, end, range.max, CT());

        std::sort(start, stop, CT());
    }
    void update()
    {
        std::sort(points.begin(), points.end(), CT());
    }

    const_iterator cbegin() const
    {
        return points.cbegin();
    }
    const_iterator cend() const
    {
        return points.cend();
    }
    RangeHelper iterate() const
    {
        return { points.cbegin(), points.cend() };
    }
    RangeHelper reverse() const
    {
        return { points.rbegin(), points.rend() };
    }
    std::size_t size() const
    {
        return points.size();
    }
};

#endif // LINEAR_TREE_HPP
