#include <type_traits>

#ifndef PACK_H
#define PACK_H

namespace pack_detail {

template <typename... Args>
constexpr int packSize = sizeof...(Args);

// Index to type
template <int QueryIndex, typename... Args>
struct _packIndexToType {
    static_assert(QueryIndex >= 0 && QueryIndex < packSize<Args...>, "QueryIndex out of range.");
};

// Found the type at the index
template <typename Head, typename... Tail>
struct _packIndexToType<0, Head, Tail...> {
    using type = Head;
};

// Recursive trim down the pack
template <int QueryIndex, typename Head, typename... Tail>
struct _packIndexToType<QueryIndex, Head, Tail...> {
    using type = typename _packIndexToType<QueryIndex - 1, Tail...>::type;
};

template <int QueryIndex, typename... Args>
using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

// Type to index

template <int Accumulated, typename QueryType, typename... Args>
constexpr int _packTypeToIndex = -1;

template <int Accumulated, typename QueryType, typename Head, typename... Tail>
constexpr int _packTypeToIndex<Accumulated, QueryType, Head, Tail...> =
    std::is_same<QueryType, Head>::value ? Accumulated
                                         : _packTypeToIndex<Accumulated + 1, QueryType, Tail...>;

template <typename QueryType, typename... Args>
constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;

}  // namespace pack_detail

template <typename... Args>
struct Pack {
    static constexpr int size = pack_detail::packSize<Args...>;

    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    template <typename QueryType>
    constexpr static int index = pack_detail::packTypeToIndex<QueryType, Args...>;
};

#endif  // PACK_H
