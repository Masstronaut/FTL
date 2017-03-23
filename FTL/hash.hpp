// All content copyright (C) Allan Deutsch 2017. All rights reserved.
#pragma once
#include <cstdint>
namespace ftl {

  using uint64_t = ::std::uint64_t;
  using uint32_t = ::std::uint32_t;
  // According to the top post here: http://softwareengineering.stackexchange.com/a/145633
  // murmur2 is the best hash for speed and random distribution.
  // This is a constexpr implementation of Murmur2A, a hash algorithm designed by Austin Appleby.
  // This version is optimized for 64bit architectures.
  constexpr uint64_t murmur2a_64_ct( const char *str, size_t size, uint64_t seed ) {
    uint64_t prime{ 0xc6a4a7935bd1e995ull };
    const uint64_t shift1{ 19 };
    constexpr uint32_t shift2{ 37 };
    uint64_t hash{ seed ^ ( size * prime ) };
    const uint64_t *data{ ( const uint64_t * )str };
    const uint64_t *end{ data + ( size / 8 ) };

    while( data != end ) {
      uint64_t word{ *data++ };
      word *= prime;
      word ^= word >> shift1;
      word *= prime;
      hash ^= word;
      hash *= prime;
    }

    const unsigned char *byte_data{ ( const unsigned char * )data };

    switch( size & 7 ) {
    case 7: hash ^= ( ( uint64_t )byte_data[ 6 ] ) << 48;
    case 6: hash ^= ( ( uint64_t )byte_data[ 5 ] ) << 40;
    case 5: hash ^= ( ( uint64_t )byte_data[ 4 ] ) << 32;
    case 4: hash ^= ( ( uint64_t )byte_data[ 3 ] ) << 24;
    case 3: hash ^= ( ( uint64_t )byte_data[ 2 ] ) << 16;
    case 2: hash ^= ( ( uint64_t )byte_data[ 1 ] ) << 8;
    case 1: hash ^= ( ( uint64_t )byte_data[ 0 ] );
    }

    hash ^= hash >> shift1;
    hash *= prime;
    hash ^= hash >> shift2;

    return hash;
  }

  // User defined literal conversion to convert a string in the form "foo"_hash to the corresponding Murmur2A hash value
  constexpr uint64_t operator""_hash( const char *str, size_t size ) {
    constexpr uint64_t seed_prime{ 0xFFFFFFFFFFFFFFC5ull };
    return murmur2a_64_ct( str, size, seed_prime );
  }
}