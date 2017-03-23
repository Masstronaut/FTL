// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#pragma once

#include <functional> // std::hash
#include <tuple> // std::pair
#include <memory> // allocator traits
#include <utility> // std::declval
#include "allocator.hpp"
#include "vector.hpp"
namespace ftl {
  /*
  * design decisions:
  * Open addressing
  * linear probing
  * Max probe length before grow: log(n)
  * Robin Hood hashing
  * Default prime size table, hash can set it to pow2 sizes
  * Size is always actually prime + log(prime) to avoid bounds checks
  * k+v pairs are stored in one array
  * hashes + index into kv pair array are stored in another array
  */
  template<typename Key,
    typename Value,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Allocator = ftl::default_allocator< std::pair<const Key, Value> >
  >
  class hash_map {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const Key, Value>;
    using size_type = typename std::allocator_traits<allocator_type>::size_type;
    using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const reference;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using hash_type = decltype( std::declval<hasher>( )( std::declval<key_type>( ) ) );
    using type = hash_map<key_type, value_type, key_equal, allocator_type>;
    using iterator = void;
    using const_iterator = void;
    using insert_return_type = std::pair<iterator, bool>;
    // constructors/destructor


    // capacity
    size_type empty( ) const noexcept;
    size_type size( ) const noexcept;
    size_type max_size( ) const noexcept;
    // iterators
    iterator begin( ) const noexcept;
    const_iterator cbegin( ) const noexcept;
    iterator end( ) const noexcept;
    const_iterator cend( ) const noexcept;

    //
    iterator insert( const_reference value );
    template<typename... Args>
    iterator emplace( Args&&... args );
    void clear( );
    insert_return_type insert( const_reference value );
    template<typename T>
    insert_return_type insert( T&& value );






  private:
    ftl::vector<std::pair<hash_type, size_type> > m_hashes;
    ftl::vector<value_type> m_values;


    // safety + assumption checks
    static_assert( reinterpret_cast< type * >( nullptr ) == reinterpret_cast< type * >( nullptr )->m_hashes, "m_hashes not at the start of class." );
  };


}






