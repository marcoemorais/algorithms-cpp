#pragma once

#include <algorithm>
#include <forward_list>
#include <iterator>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename FwdIter>
struct IterPair
{
    FwdIter first;
    FwdIter last;
};

namespace std // doctest requires operator<< to be in std:: namespace.
{

template <typename T>
std::ostream&
operator<<(std::ostream& os, const std::vector<T>& elems)
{
    os << "{";
    std::copy(begin(elems), end(elems), std::ostream_iterator<T>(os, ","));
    os << "}";
    return os;
}

template <typename T>
std::ostream&
operator<<(std::ostream& os, const std::forward_list<T>& elems)
{
    os << "{";
    std::copy(begin(elems), end(elems), std::ostream_iterator<T>(os, ","));
    os << "}";
    return os;
}

template <typename K, typename V>
std::ostream&
operator<<(std::ostream& os, const std::unordered_map<K, V>& kvs)
{
    os << "{";
    for (const auto& kv : kvs) {
        os << " " << kv.first << "=" << kv.second << ",";
    }
    os << "}";
    return os;
}

template <typename T1, typename T2>
std::ostream&
operator<<(std::ostream& os, const std::pair<T1, T2>& p)
{
    os << "{" << p.first << "," << p.second << "}";
    return os;
}

template <typename FwdIter,
          typename T = typename std::iterator_traits<FwdIter>::value_type>
std::ostream&
operator<<(std::ostream& os, const IterPair<FwdIter>& p)
{
    os << "{";
    std::copy(p.first, p.last, std::ostream_iterator<T>(os, ","));
    os << "}";
    return os;
}

}
