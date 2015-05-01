#ifndef UTILITY_H
#define UTILITY_H

#include <type_traits>
#include <tuple>

// Note: All credit for this code goes to Mike Kinghan, Stack Overflow user.
// This code was taken and adapted from his answer on the following Stack 
// Overflow post: "How to obtain a part of a tuple?"
// http://stackoverflow.com/questions/17508129/how-to-obtain-a-part-of-a-tuple

namespace cpp_collections {

    // A class template to obtain the type and value of the subsequence 
    // [First,Last) of a tuple type TupleType

    template<
        unsigned First, unsigned Last, typename TupleType, typename Enable = void
    >
    struct tuple_part;
    
    template<unsigned First, unsigned Last, typename TupleType>
    struct tuple_part<
        First,Last,TupleType,
        typename std::enable_if<
            (First >= Last || First >= std::tuple_size<TupleType>::value)
        >::type
    > 
    {
        using type = std::tuple<>;
    
        static constexpr type get(TupleType const & tup) {
            return type();
        }
    };
    
    template<unsigned First, unsigned Last, typename TupleType>
    struct tuple_part<
        First,Last,TupleType,
        typename std::enable_if<
            (Last == First + 1 && First < std::tuple_size<TupleType>::value)
        >::type
    > 
    {
        using type = 
        typename std::tuple<typename std::tuple_element<First,TupleType>::type>;
    
        static constexpr type get(TupleType const & tup) {
            return type(std::get<First>(tup));
        } 
    };
    
    template<unsigned First, unsigned Last, typename TupleType>
    struct tuple_part<
        First,Last,TupleType,
        typename std::enable_if<
            (Last > First + 1 && Last <= std::tuple_size<TupleType>::value)
        >::type
    >
    {
        using head = typename tuple_part<First,First + 1,TupleType>::type;
        using tail = typename tuple_part<First + 1,Last,TupleType>::type;
        using type = decltype(
            std::tuple_cat(std::declval<head>(),std::declval<tail>())
        );
    
        static constexpr type get(TupleType const & tup) {
            return std::tuple_cat(
                        tuple_part<First,First + 1,TupleType>::get(tup),
                        tuple_part<First + 1,Last,TupleType>::get(tup)
                    );
        }
    };
    
    template<unsigned First, unsigned Last, typename TupleType>
    struct tuple_part<
        First,Last,TupleType,
        typename std::enable_if<
            (Last > First + 1 && Last > std::tuple_size<TupleType>::value)
        >::type
    > : tuple_part<First,std::tuple_size<TupleType>::value,TupleType>
    {
        using base_type = 
            tuple_part<First,std::tuple_size<TupleType>::value,TupleType>;
        using type = typename base_type::type;
    };
    
    /*
        `get_part<First,Last>(TupleType const & tup)`
        returns the `std::tuple` of the subsequence [First,Last) of `tup` 
    */ 
    template<unsigned First, unsigned Last, typename TupleType>
    constexpr
    decltype(
        tuple_part<First,Last,TupleType>::get(std::declval<TupleType>())
    )   
    get_part(TupleType const & tup) 
    {
        return tuple_part<First,Last,TupleType>::get(tup);
    }
    
    /*
        `get_part<First>(TupleType const & tup)`
        returns the `std::tuple` of the terminal subsequence of `tup`
        indexed by `First`
    */ 
    template<unsigned First, typename TupleType>
    constexpr
    decltype(
        get_part<First,std::tuple_size<TupleType>::value>(std::declval<TupleType>())
    )   
    get_part(TupleType const & tup) 
    {
        return get_part<First,std::tuple_size<TupleType>::value>(tup);
    }

}

#endif

