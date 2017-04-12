// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#pragma once

#include <cassert>
#include <cmath> // std::log10
#include <functional> // std::hash
#include <tuple> // std::pair
#include <memory> // allocator traits
#include <utility> // std::declval
#include <cstdint> // uint32_t
#include <iterator> // std::advance
#include "allocator.hpp"
#include "vector.hpp"
#include "hash.hpp"
namespace ftl {

  namespace detail {
    uint32_t hash_table_primes[ ]{
      29u, 59u, 127u, 257u, 521u, 1049u,
      2099u, 4201u, 8419u, 16843u, 33703u,
      67409u, 134837u, 269683u, 539389u,
      1078787u
    };
#define FTL_DEFINE_MOD_FUNC( N ) uint64_t mod##N(uint64_t value) { return value % N##ull; }
    FTL_DEFINE_MOD_FUNC(29)
    FTL_DEFINE_MOD_FUNC(59)
    FTL_DEFINE_MOD_FUNC(127)
    FTL_DEFINE_MOD_FUNC(257)
    FTL_DEFINE_MOD_FUNC(521)
    FTL_DEFINE_MOD_FUNC(1049)
    FTL_DEFINE_MOD_FUNC(2099)
    FTL_DEFINE_MOD_FUNC(4201)
    FTL_DEFINE_MOD_FUNC(8419)
    FTL_DEFINE_MOD_FUNC(16843)
    FTL_DEFINE_MOD_FUNC(33703)
    FTL_DEFINE_MOD_FUNC(67409)
    FTL_DEFINE_MOD_FUNC(134837)
    FTL_DEFINE_MOD_FUNC(269683)
    FTL_DEFINE_MOD_FUNC(539389)
    FTL_DEFINE_MOD_FUNC(1078787)
#undef FTL_DEFINE_MOD_FUNC

    uint32_t next_table_size( uint32_t prime ) {
      for( unsigned i{ 0 }; i < sizeof( hash_table_primes ) / sizeof(*hash_table_primes); ++i ) {
        if( hash_table_primes[ i ] > prime ) return hash_table_primes[ i ];
      }
      assert( false && "Hash table has exceeded its max_size." );
      return -1;
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
    typename Hash = ::ftl::hash<Key, ::ftl::ftlhash<uint64_t>>,
    typename KeyEqual = ::std::equal_to<Key>,
    typename Allocator = ::ftl::default_allocator< ::std::pair<const Key, Value> >
  >
  class hash_map {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = ::std::pair<const Key, Value>;
    using allocator_type = Allocator;
    using size_type = typename ::std::uint32_t;
    using difference_type = typename ::std::allocator_traits<allocator_type>::difference_type;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename ::std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename ::std::allocator_traits<allocator_type>::const_pointer;
    using hash_type = decltype( ::std::declval<hasher>( )( ::std::declval<key_type>( ) ) );
    using type = hash_map<key_type, value_type, key_equal, allocator_type>;
    using iterator = typename ::ftl::vector<value_type>::iterator;
    using const_iterator = typename ::ftl::vector<value_type>::const_iterator;
    using insert_return_type = ::std::pair<iterator, bool>;
    // constructors/destructor


    // capacity
    size_type empty( ) const noexcept { return ( size_type )m_values.empty( ); }
    size_type size( ) const noexcept { return ( size_type )m_values.size( ); }
    size_type max_size( ) const noexcept {
      //@@TODO: update this to support other allocation strategies (like power 2)
      return ::ftl::detail::hash_table_primes[ sizeof(::ftl::detail::hash_table_primes) 
                                             / sizeof(::ftl::detail::hash_table_primes[0])
                                             - 1u ];
    }
    size_type capacity( ) const noexcept { 
      return static_cast< size_type >( static_cast< float >( m_capacity + max_probe_count( ) ) * m_max_load_factor );
    }
    // iterators
    iterator begin( ) const noexcept { return m_values.begin( ); }
    const_iterator cbegin( ) const noexcept { return m_values.cbegin( ); }
    iterator end( ) const noexcept { return m_values.end( ); }
    const_iterator cend( ) const noexcept { return m_values.cend( ); }
    //
    template<typename... Args>
    ::std::pair<iterator, bool> emplace( Args&&... args ) {
      m_values.emplace_back( ::std::forward<Args>( args )... );
      const hash_type hash{ m_hasher( m_values.back( ).first ) };
      // @@TODO: Add a find overload (possibly private) which takes a hash as well.
      iterator collision{ this->find( m_values.back( ).first ) };
      if( collision != this->end( ) ) {
        m_values.pop_back( );
        auto pos{ m_values.begin( ) };
        ::std::advance( pos, collision->second );
        return { pos , false };
      } else {
        insert_hash( m_hashes, { hash, m_values.size( ) - 1 } );
        return { m_values.get_iterator( m_values.back( ) ), true };
      }
    }
    void clear( ) {
      m_hashes.clear( );
      m_values.clear( );
    }

    mapped_type& operator[]( const key_type &key );
    mapped_type& operator[]( key_type &&key );

    insert_return_type insert( value_type&& value ) {
      return emplace( ::std::forward<value_type>( value ) );
    }

    insert_return_type insert( const_reference value ) {
      insert_return_type result{ this->end( ), false };
      hash_type hash{ m_hasher( value.first ) };
      insert_return_type collision{ hash_collision( hash ) };
      if( collision.second ) return { collision.first, false }; // Failed: hash exists
      else {
        m_values.emplace_back( value );
        insert_hash( m_hashes, { hash, m_values.size( ) - 1 } );
      }
      return { --m_values.end( ), true };
    }

    /*
    * If a key equivalent to k already exists in the container, 
    * assigns std::forward<M>(obj) to the mapped_type corresponding to the key k. 
    * If the key does not exist, inserts the new value as if by insert, 
    * constructing it from value_type(k, std::forward<M>(obj))
    */
    template<typename M>
    insert_return_type insert_or_assign( const key_type &k, M&& obj ) {
      hash_type hash{ m_hasher( k ) };
      auto collision{ hash_collision( hash ) };
      if( collision == m_hashes.end( ) ) {
        return insert( std::move( value_type{ k, std::forward<M>( obj ) } ) );
      } else {
        m_values[ collision->second ].first = k;
        m_values[ collision->second ].second = std::forward<M>( obj );
        return { { m_values.begin( ) + collision->second }, false };
      }
    }

    /*
    * If a key equivalent to k already exists in the container,
    * assigns std::forward<M>(obj) to the mapped_type corresponding to the key k.
    * If the key does not exist, inserts the new value as if by insert,
    * constructing it from value_type(std::move(k), std::forward<M>(obj))
    */
    template<typename M>
    insert_return_type insert_or_assign( key_type &&k, M&& obj ) {
      hash_type hash{ m_hasher( k ) };
      auto collision{ hash_collision( hash ) };
      if( collision == m_hashes.end( ) ) {
        return insert( std::move( value_type{ std::move( k ), std::forward<M>( obj ) } ) );
      } else {
        m_values[ collision->second ].first = std::move( k );
        m_values[ collision->second ].second = std::forward<M>( obj );
        return { { m_values.begin( ) + collision->second }, false };
      }
    }

    /* While the hint variant of insert_or_assign from std::unordered_map is available,
     * this implementation can't use it, and as such this is equivalent to calling the 
     * regular insert_or_assign method. */
    template<typename M>
    insert_return_type insert_or_assign( const_iterator hint, const key_type &k, M&& obj ) {
      ( void )hint;
      return insert_or_assign( k, std::forward<M>( obj ) );
    }

    /* While the hint variant of insert_or_assign from std::unordered_map is available,
    * this implementation can't use it, and as such this is equivalent to calling the
    * regular insert_or_assign method. */
    template<typename M>
    insert_return_type insert_or_assign( const_iterator hint, key_type &&k, M&& obj ) {
      ( void )hint;
      return insert_or_assign( std::forward<key_type>( k ), std::forward<M>( obj ) );
    }

    void reserve( size_type n ) {
      if( this->capacity( ) > n ) return;
      const size_type new_size{ ::ftl::detail::next_table_size( n * ( 1. / m_max_load_factor ) ) };
      m_max_probe_count = static_cast< size_type >( std::log2( new_size ) );
      const size_type new_capacity{ static_cast<size_type>((new_size + max_probe_count( )) * this->m_max_load_factor) };
      decltype(this->m_hashes) hashes( new_capacity / m_max_load_factor, tombstone );
      for(auto &it : m_hashes ){
        if( it != tombstone ) {
          insert_hash( hashes, it );
        }
      }
      m_hashes = hashes;
      m_values.reserve( new_capacity );
      m_capacity = new_size;
    }

    iterator find( const key_type &key, hash_type hint ) {
      return const_cast< iterator >( 
        const_cast< const hash_map < Key, Value, Hash, KeyEqual, Allocator> *>
        (this)->find( key, hint ) );
    }

    const_iterator find( const key_type &key, hash_type hint ) const {
      size_type index{ hint % m_capacity };
      for( size_type i{ 0 }; i <= max_probe_count( ); ++i ) {
        hash_entry_type value{ m_hashes.at( index + i ) };
        const_iterator element{ m_values.cbegin( ) + value.second };
        if( value.first == hint  && m_key_equal( key, element->first ) ) {
          return element;
        }
      }
      return m_values.cend( );
    }

    iterator find( const key_type &key ) {
      return find( key, m_hasher( key ) );
    }

    const_iterator find( const key_type &key ) const {
      return this->find( key, m_hasher( key ) );
    }

    iterator find( key_type &&key ) {
      return this->find( key, m_hasher( key ) );
    }

    const_iterator find( key_type &&key ) const {
      return this->find( key, m_hasher(key) );
    }


    // find() version which doesn't perform validation of keys.
    // By omitting the key compare, speed is improved substantially.
    // The cost of this is two unique keys with a hash collision may 
    //   return the wrong value.
    iterator find_fast( const key_type &key ) {
      hash_type hash{ m_hasher( key ) };
      size_type index{ hash % m_capacity };
      for( size_type i{ 0 }; i <= max_probe_count( ); ++i ) {
        hash_entry_type value{ m_hashes.at( index + i ) };
        if( value.first == hash ) {
          return { m_values.begin( ) + value.second };
        }
      }
      return m_values.end( );
    }

    // find() version which doesn't perform validation of keys.
    // By omitting the key compare, speed is improved substantially.
    // The cost of this is two unique keys with a hash collision may 
    //   return the wrong value.
    const_iterator find_fast( const key_type &key ) const {
      return { this->find_fast( key ) };
    }

    // find() version which doesn't perform validation of keys.
    // By omitting the key compare, speed is improved substantially.
    // The cost of this is two unique keys with a hash collision may 
    //   return the wrong value.
    iterator find_fast( key_type &&key ) {
      hash_type hash{ m_hasher( key ) };
      size_type index{ hash % m_capacity };
      for( size_type i{ 0 }; i <= max_probe_count( ); ++i ) {
        hash_entry_type value{ m_hashes.at( index + i ) };
        if( value.first == hash ) {
          return { m_values.begin( ) + value.second };
        }
      }
      return m_values.end( );
    }

    // find() version which doesn't perform validation of keys.
    // By omitting the key compare, speed is improved substantially.
    // The cost of this is two unique keys with a hash collision may 
    //   return the wrong value.
    const_iterator find_fast( key_type &&key ) const {
      return { this->find_fast( std::forward<key_type>( key ) ) };
    }

    iterator erase( const_iterator pos );

    // Whether or not this should exist is still undecided
    //iterator erase( const_iterator first, const_iterator last );

    size_type erase( const key_type &key );

  private:
    using hash_entry_type = std::pair< hash_type, size_type >;
    using hash_table_type = ftl::vector< hash_entry_type, typename allocator_type::template rebind<hash_entry_type>>;
    using value_table_type = ftl::vector< value_type, allocator_type >;
    using probe_table_iterator = typename hash_table_type::iterator;
    static const hash_entry_type tombstone;

    hash_table_type m_hashes;
    value_table_type m_values;
    hasher m_hasher;
    key_equal m_key_equal;
    float m_max_load_factor{ 0.5f };
    size_type m_capacity{ 0u };
    size_type m_max_probe_count{ 0u };



    size_type max_probe_count( ) const noexcept { 
      return m_max_probe_count; 
    }

    probe_table_iterator probe_key( const key_type &k ) {
      hash_type hash{ m_hasher( k ) };
      size_type index{ hash % m_capacity };
      for( size_type i{ 0 }; i < m_max_probe_count; ++i ) {
        if( m_hashes[ index + i ].first == hash 
            && this->m_key_equal( k, m_values[ m_hashes[ index + i ].second ].first ) )
          return { m_hashes.begin( ) + ( index + i ) };
      }
      return m_hashes.end( );
    }

    probe_table_iterator hash_collision( hash_type hash ) {
      size_type index{ hash % m_capacity };
      for( size_type i{ 0 }; i < m_max_probe_count; ++i ) {
        if( m_hashes[ index + i ].first == hash ) {
          return { m_hashes.begin() + (index + i) };
        }
      }
      return m_hashes.end( );
    }

    void insert_hash( hash_table_type &vec, hash_entry_type hash ) {
      size_type index{ hash.first % m_capacity };
      unsigned probe_count{ 0 };
      while( probe_count <= m_max_probe_count ) {
        hash_entry_type &cur_entry{ vec[ index + probe_count ] };
        if( cur_entry == tombstone ) {
          cur_entry = hash;
          return;
        }
        difference_type other_index{ cur_entry.first % m_capacity };
        unsigned other_probe_count{ (index + probe_count) - other_index };
        if( other_probe_count < probe_count ) {
          ::std::swap( cur_entry, hash );
          probe_count = other_probe_count;
          index = other_index;
        }
        ++probe_count;
      }
      if (probe_count > m_max_probe_count ) {
        assert( false && "An internal error has occurred. Please file a report with the FTL project on github.com/masstronaut/FTL " );
      }
    }
    void erase_hash( hash_table_type &vec, hash_type hash ) {

    }
  };
  template< typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator >
  const typename hash_map<Key, Value, Hash, KeyEqual, Allocator>::hash_entry_type hash_map<Key, Value, Hash, KeyEqual, Allocator>::tombstone{ 0u, 0u };




  template< typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator >
  typename hash_map<Key, Value, Hash, KeyEqual, Allocator>::iterator hash_map<Key, Value, Hash, KeyEqual, Allocator>::erase( const_iterator pos ) {
    assert( pos != this->end( ) );
    hash_type hash{ this->m_hasher( pos->first ) };
    auto hash_iter{ probe_key( pos->first ) };
    probe_table_iterator replacement_hash_iter{ probe_key( this->m_values.back( ).first ) };
    replacement_hash_iter->second = ::std::distance( m_values.cbegin( ), pos );
    iterator it{ m_values.get_iterator( *pos ) };
    it->~value_type( );
    new( it ) ::std::pair<const Key, Value>( this->m_values.back( ) );
    this->m_values.pop_back( );
    this->erase_hash( this->m_hashes, hash );
    return it;
  }


  template< typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator >
  typename hash_map<Key, Value, Hash, KeyEqual, Allocator>::size_type hash_map<Key, Value, Hash, KeyEqual, Allocator>::erase( const key_type &key ) {
    iterator it{ find( key ) };
    if( it == this->end( ) ) {
      return 0;
    }
    this->erase( it );
    return 1;
  }



  template< typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator >
  typename hash_map< Key, Value, Hash, KeyEqual, Allocator >::mapped_type& hash_map< Key, Value, Hash, KeyEqual, Allocator >::operator[]( const key_type &key ) {
    probe_table_iterator iter{ this->probe_key( key ) };
    if( iter != this->m_hashes.end( ) ) {
      return this->m_values[ iter->second ].second;
    }
    else {
      return insert( { key, {} } ).first->second;
    }
  }


  template< typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator >
  typename hash_map< Key, Value, Hash, KeyEqual, Allocator >::mapped_type& hash_map< Key, Value, Hash, KeyEqual, Allocator >::operator[]( key_type &&key ) {
    probe_table_iterator iter{ this->probe_key( key ) };
    if( iter != this->m_hashes.end( ) ) {
      return this->m_values[ iter->second ].second;
    } else {
      return insert( { key,{ } } ).first->second;
    }
  }


} // namespace ftl






