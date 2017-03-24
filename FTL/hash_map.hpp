// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#pragma once

#include <cassert>
#include <cmath> // std::log10
#include <functional> // std::hash
#include <tuple> // std::pair
#include <memory> // allocator traits
#include <utility> // std::declval
#include "allocator.hpp"
#include "vector.hpp"
namespace ftl {

  namespace detail {
    uint32_t hash_table_primes[ ]{
      29u, 59u, 127u, 257u, 521u, 1049u,
      2099u, 4201u, 8419u, 16843u, 33703u,
      67409u, 134837u, 269683u, 539389u,
      1078787u
    };

    uint32_t next_table_size( uint32_t prime ) {
      for( unsigned i{ 0 }; i < sizeof( hash_table_primes ) / sizeof(*hash_table_primes); ++i ) {
        if( hash_table_primes[ i ] > prime ) return hash_table_primes[ i ];
      }
      assert( false && "Hash table has exceeded its max_size." );
    }
  }
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
    size_type capacity( ) const noexcept;
    // iterators
    iterator begin( ) const noexcept;
    const_iterator cbegin( ) const noexcept;
    iterator end( ) const noexcept;
    const_iterator cend( ) const noexcept;

    //
    template<typename... Args>
    iterator emplace( Args&&... args );
    void clear( );
    template<typename T>
    insert_return_type insert( T&& value );

    insert_return_type insert( const_reference value ) {
      insert_return_type result{ this->end( ), false };
      hash_type hash{ m_hasher( value.first ) };
      insert_return_type collision{ hash_collision( hash ) };
      if( !collision.second ) return collision;
      else {
        m_values.emplace_back( value );
        insert_hash( m_hashes, { hash, m_values.size( ) - 1 } );
      }
      return { &m_values.back( ), true };
    }

    void reserve( size_type n ) {
      if( this->capacity() > (float)n / m_max_load_factor ) return;
      m_size_log10 = static_cast< size_type >( std::log10( n ) );
      size_type new_size{ ::ftl::detail::next_table_size( n ) };
      size_type new_capacity{ new_size + m_size_log10 };
      ftl::vector< std::pair< hash_type, size_type > > hashes( new_capacity, tombstone );
      for(auto &it : m_hashes ){ 
        insert_hash( hashes, it );
      }
      m_hashes = std::move( hashes );
      m_values.reserve( new_capacity );
      m_capacity = new_size;
    }




  private:
    insert_return_type hash_collision( hash_type hash ) {
      size_type index{ it.first % new_size };
      for( size_type i{ 0 }; i < m_size_log10; ++i ) {
        if( m_hashes[ index + i ].first == hash ) return { {m_hashes[ index + i ].second }, false };
      }
      return { this->end( ), true };
    }
    void insert_hash( ftl::vector< std::pair<hash_type, size_type> > &vec, std::pair<hash_type, size_type> hash ) {
      size_type index{ it.first % new_size };
      unsigned probe_count{ 0 };
      while( probe_count <= m_size_log10 && vec[ index + probe_count ] != tombstone ) {
        unsigned distance_from_optimal{ vec[ index + probe_count ].first % new_size };
        if( distance_from_optimal < probe_count ) {
          std::swap( vec[ index + probe_count ], hash );
          probe_count = distance_from_optimal;
        } 
        ++probe_count;
      }
      if( vec[ index + probe_count ] == tombstone ) {
        vec[ index + probe_count ] = hash;
      } else if (probe_count > m_size_log10 ) {
        assert( false && "An internal error has occurred. Please file a report with the FTL project on github.com/masstronaut/FTL " );
      }
    }
    static const std::pair< hash_type, size_type > tombstone{ 0u, 0u };

    ftl::vector< std::pair<hash_type, size_type> > m_hashes;
    ftl::vector<value_type> m_values;
    hasher m_hasher;
    float m_max_load_factor{ 0.5f };
    size_type m_capacity{ 0u };
    size_type m_size_log10{ 0u };

    // safety + assumption checks
    static_assert( reinterpret_cast< type * >( nullptr ) == reinterpret_cast< type * >( nullptr )->m_hashes, "m_hashes not at the start of class." );
  };


}






