// All content copyright (C) Allan Deutsch 2017. All rights reserved.
#pragma once
#include <cstdint>
#include <utility> // std::forward
#include <cassert>
#include <cstddef> // size_t
#include <string>
#if defined(_DEBUG)
#include "debug.hpp"
#endif
namespace ftl {

  template<typename Hasher>
  struct hash_traits {
    using hasher_type = Hasher;
    using hash_type = typename hasher_type::hash_type;
    static constexpr hash_type prime{ sizeof( hash_type ) == 4 ? 16777619u : 1099511628211ull };
    static constexpr hash_type seed{ sizeof( hash_type ) == 4 ? 0x811c9dc5u : 0xcbf29ce484222325ull };
    static inline constexpr void start( hasher_type &hasher, size_t size ) { hasher.start( size ); }
    static inline constexpr void insert( hasher_type &hasher, hash_type chunk ) { hasher.insert( chunk ); }
    static inline constexpr hash_type finish( hasher_type &hasher, hash_type last_chunk ) { return hasher.finish( last_chunk ); }
  };

  template<typename Result>
  struct fnv1 {
    constexpr fnv1( ) = default;
    static_assert( std::is_integral<Result>::value, "FNV-1a hash function requires an integer type for the result of the hash." );
    using hash_type = Result;

    constexpr void start( size_t size ) { }
    constexpr void insert( hash_type block ) {
      m_hash *= hash_traits<::ftl::fnv1<hash_type> >::prime;
      m_hash ^= block;
    }
    constexpr hash_type finish( hash_type last_block ) {
      insert( last_block );
      hash_type result{ m_hash };
      m_hash = hash_traits<::ftl::fnv1<hash_type> >::seed;
      return result;
    }
    constexpr hash_type finish( ) {
      hash_type result{ m_hash };
      m_hash = hash_traits<::ftl::fnv1<hash_type> >::seed;
      return result;
    }

  private:
    hash_type m_hash{ hash_traits<::ftl::fnv1<hash_type> >::seed };
  };

  template<typename Result>
  struct fnv1a {
    constexpr fnv1a( ) = default;
    static_assert( std::is_integral<Result>::value, "FNV-1a hash function requires an integer type for the result of the hash." );
    using hash_type = Result;

    constexpr void start( size_t size ) { }
    constexpr void insert( hash_type block ) {
      m_hash ^= block;
      m_hash *= hash_traits<::ftl::fnv1a<hash_type> >::prime;
    }
    constexpr hash_type finish( hash_type last_block ) {
      insert( last_block );
      return finish( );
    }
    constexpr hash_type finish( ) {
      hash_type result{ m_hash };
      // reset state
      m_hash = hash_traits<::ftl::fnv1a<hash_type> >::seed;
      return result;
    }

  private:
    hash_type m_hash{ hash_traits<::ftl::fnv1a<hash_type> >::seed };
  };

  template<typename Result>
  struct ftlhash {
    constexpr ftlhash( ) = default;
    static_assert( std::is_integral<Result>::value, "FTLhash function requires an integer type for the result of the hash." );
    using hash_type = Result;

    constexpr void start( size_t size ) { 
      m_hash *= ~(size << 13) ^ hash_traits<::ftl::ftlhash<hash_type> >::prime;
    }
    constexpr void insert( hash_type block ) {
      m_hash ^= block;
      m_hash ^= ( block << 7 ) ^ ~( block >> 11 );
      m_hash *= hash_traits<::ftl::ftlhash<hash_type> >::prime;
    }
    constexpr hash_type finish( hash_type last_block ) {
      insert( last_block );
      return finish( );
    }
    constexpr hash_type finish( ) {
      m_hash ^= ~( m_hash << 7 ) ^ ~( m_hash >> 11 );
      hash_type result{ m_hash };
      // reset state for next hash
      m_hash = hash_traits<::ftl::ftlhash<hash_type> >::seed;
      return result;
    }

  private:
    ::ftl::debug::bits<hash_type> m_hash{ hash_traits<::ftl::ftlhash<hash_type> >::seed };
  };

  // According to the top post here: http://softwareengineering.stackexchange.com/a/145633
  // murmur2 is the best hash for speed and random distribution.
  // This is a constexpr implementation of Murmur2A, a hash algorithm designed by Austin Appleby.
  // This version is optimized for 64bit architectures.
  template<typename Result>
  struct murmur2a {
    using hash_type = Result;
    constexpr murmur2a( ) = default;

    static_assert( std::is_integral<Result>::value, "MurMurHash2a hash function requires an integer type for the result of the hash." );
    static_assert( sizeof( hash_type ) == 4 || sizeof( hash_type ) == 8, "Hash type must be a 4 or 8 byte integer." );

    constexpr void start( size_t size ) { 
      m_hash ^= size * ::ftl::hash_traits<::ftl::murmur2a<hash_type> >::prime;
    }
    constexpr void insert( hash_type block ) {
      block *= ::ftl::hash_traits<::ftl::murmur2a<hash_type> >::prime;
      block ^= block >> s_shift1;
      block *= ::ftl::hash_traits<::ftl::murmur2a<hash_type> >::prime;
      m_hash ^= block;
      m_hash *= ::ftl::hash_traits<::ftl::murmur2a<hash_type> >::prime;
    }
    constexpr hash_type finish( hash_type last_block ) {
      insert( last_block );
      return finish( );
    }
    constexpr hash_type finish( ) {
      m_hash ^= m_hash >> s_shift1;
      m_hash *= ::ftl::hash_traits<::ftl::murmur2a<hash_type> >::prime;
      m_hash ^= m_hash >> s_shift2;
      hash_type result{ m_hash };
      // reset the state for next hash
      m_hash = ::ftl::hash_traits<::ftl::murmur2a<hash_type> >::seed;
      return result;
    }

  private:
    static constexpr uint16_t s_shift1{ 7 };
    static constexpr uint16_t s_shift2{ 19 };
    hash_type m_hash{ ::ftl::hash_traits<::ftl::murmur2a<hash_type> >::seed };
  };

  namespace detail {
    template<typename Hasher = murmur2a<uint32_t>>
    constexpr typename ::ftl::hash_traits<Hasher>::hash_type hash( const char *str, size_t size, Hasher hasher = {} ) {

      using hash_type = typename ::ftl::hash_traits<Hasher>::hash_type;
#pragma warning( push )
#pragma warning( disable : 4293 )
      const char *data{ str };
      const char *end{ data + ( sizeof( hash_type ) * ( size / sizeof( hash_type ) ) ) };
      ::ftl::hash_traits<Hasher>::start( hasher, size );
      while( data != end ) {
        ::ftl::debug block{ 0 };
        switch( sizeof( block ) ) {

        case 8: block ^= ( ( hash_type )data[ sizeof( block ) ] ) << ( ( sizeof( block ) - 1 ) * 8 );
        case 7: block ^= ( ( hash_type )data[ 6 ] ) << 48;
        case 6: block ^= ( ( hash_type )data[ 5 ] ) << 40;
        case 5: block ^= ( ( hash_type )data[ 4 ] ) << 32;
        case 4: block ^= ( ( hash_type )data[ 3 ] ) << 24;
        case 3: block ^= ( ( hash_type )data[ 2 ] ) << 16;
        case 2: block ^= ( ( hash_type )data[ 1 ] ) << 8;
        case 1: block ^= ( ( hash_type )data[ 0 ] ) << 0;

          break;
        default:
          assert( false && "Invalid hash type." );
        }
        data += sizeof(block);
        ::ftl::hash_traits<Hasher>::insert( hasher, block );
      }

      const char *last_block{ data };
      hash_type block{ 0 };
      switch( size & ( sizeof( hash_type ) - 1 ) ) {
      case 7: block ^= ( ( hash_type )last_block[ 6 ] ) << 48;
      case 6: block ^= ( ( hash_type )last_block[ 5 ] ) << 40;
      case 5: block ^= ( ( hash_type )last_block[ 4 ] ) << 32;
      case 4: block ^= ( ( hash_type )last_block[ 3 ] ) << 24;
      case 3: block ^= ( ( hash_type )last_block[ 2 ] ) << 16;
      case 2: block ^= ( ( hash_type )last_block[ 1 ] ) << 8;
      case 1: block ^= ( ( hash_type )last_block[ 0 ] ) << 0;
#pragma warning( pop )
        break;
      default:
        assert( false && "Invalid hash type." );
      }

      return ::ftl::hash_traits<Hasher>::finish( hasher, block );
    }

  } // detail


  template<typename Key, typename Hasher = ::ftl::ftlhash<uint32_t>>
  struct hash {
    using result_type = typename ::ftl::hash_traits<Hasher>::hash_type;
    constexpr hash( ) = default;
    result_type operator()( const Key &key ) {
      ::ftl::detail::hash<Hasher>( ( const char * )&reinterpret_cast< const char & >( key ), sizeof( Key ) );
    }
  };
  // specializations
  template<typename Hasher>
  struct hash<const char *, Hasher> {
    using result_type = typename ::ftl::hash_traits<Hasher>::hash_type;
    constexpr hash( ) = default;
    constexpr result_type operator()( const char *str ) {
      return ::ftl::detail::hash<Hasher>( str, c_str_length(str) );
    }
    constexpr result_type operator()( const char *str, size_t size ) {
      return ::ftl::detail::hash<Hasher>( str, size );
    }
  private:
    constexpr size_t c_str_length( const char *str ) {
      size_t size{ 0 };
      while( *str++ && ++size ); // intentionally empty body
      return size;
    }
  };
  template<typename Hasher>
  struct hash<::std::string, Hasher> {
    using result_type = typename ::ftl::hash_traits<Hasher>::hash_type;
    result_type operator()( const ::std::string &str ) {
      return ::ftl::detail::hash<Hasher>( str.c_str( ), str.size( ) );
    }
  };


}
