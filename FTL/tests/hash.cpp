// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#include "../container_traits.hpp"
#include "../vector.hpp"
#include "../timer.hpp"
#include "../hash.hpp"
#include "../io.hpp"

#include <vector>
#include <array>
#include <unordered_map>
#include <typeinfo>
#include <cassert>
#include <iostream>
#include <memory>
#include <numeric> // std::accumulate
#include <cstdint>

// User defined literal conversion to convert a string in the form "foo"_hash to the corresponding Murmur2A hash value
constexpr uint64_t operator""_hash(const char *str, size_t size) {
  return ::ftl::hash<const char *, ::ftl::murmur2a<uint64_t>>{}( str, size );
}

struct nibble_distribution {
  constexpr nibble_distribution( ) = default;
  const uint32_t& operator[]( size_t index ) const {
    return values[ index ];
  }
  uint32_t& operator[]( size_t index ) {
    return values[ index ];
  }
  ::std::array<uint32_t, 16> values{ 0, 0, 0, 0,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0 };
  friend ::std::ostream& operator<<( ::std::ostream &os, const nibble_distribution &rhs ) {
    uint32_t sum{ 0 };
    for( auto it : rhs.values ) sum += it;
    const int32_t average{ static_cast< int32_t >( sum / rhs.values.size( ) ) };
    for( size_t i{ 0 }; i < rhs.values.size( ); ++i ) {
      os << ::ftl::io::left_pad( 10, ::std::abs( average - static_cast< int32_t >( rhs.values[ i ] ) ) );
    }
    os << " " << ::ftl::io::right_pad( static_cast< float >( rhs.normalized_std_dev( ) ), 11 );
    return os;
  }
  uint32_t sum_values( ) const noexcept {
    return ::std::accumulate( values.cbegin( ), values.cend( ), 0u );
  }
  uint32_t avg_value( ) const noexcept {
    return sum_values( ) / static_cast< uint32_t >( values.size( ) );
  }
  double variance( ) const noexcept {
    const uint32_t sum{ sum_values() };
    const int32_t average{ static_cast< int32_t >( sum / values.size( ) ) };
    ::std::array<int32_t, 16> errors;
    for( unsigned i{ 0 }; i < values.size( ); ++i ) {
      int32_t error{ ::std::abs( average - static_cast< int32_t >( values[ i ] ) ) };
      errors[ i ] = error;
    }
    const double weight{ 1. / static_cast< double >( values.size( ) ) };
    double mean_square_error{ 0 };
    for( auto it : errors ) {
      mean_square_error += it * it;
    }
    mean_square_error *= weight;
    return mean_square_error;
  }
  double std_dev( ) const noexcept {
    return ::std::sqrt( variance( ) );
  }
  double normalized_variance( ) const noexcept {
    const double average{ static_cast< double >( sum_values( ) / values.size( ) ) };
    ::std::array<double, 16> errors;
    for( unsigned i{ 0 }; i < values.size( ); ++i ) {
      double error{ ::std::abs( average -  values[ i ] ) / average  };
      errors[ i ] = error;
    }
    const double weight{ 1. / static_cast< double >( values.size( ) ) };
    double variance{ 0. };
    for( auto it : errors ) {
      variance += it * it;
    }
    variance *= weight;
    return variance;
  }
  double normalized_std_dev( ) const noexcept {
    return ::std::sqrt( normalized_variance( ) );
  }

};

template<typename T>
struct hash_distribution {
  constexpr hash_distribution( ) = default;

  ::std::array<nibble_distribution, sizeof( T ) * 2> values;

  constexpr const nibble_distribution& operator[]( size_t index ) const {
    return values[ index ];
  }
  constexpr uint32_t& operator[]( size_t index ) {
    return values[ index ];
  }

  constexpr void add_hash( T hash ) {
    switch( sizeof( T ) ) {
    case 8: 
      values[ 15 ][ ( hash & 0xF000000000000000 ) >> 60 ]++;
      values[ 14 ][ ( hash & 0xF00000000000000 ) >> 56 ]++;
      values[ 13 ][ ( hash & 0xF0000000000000 ) >> 52 ]++;
      values[ 12 ][ ( hash & 0xF000000000000 ) >> 48 ]++;
      values[ 11 ][ ( hash & 0xF00000000000 ) >> 44 ]++;
      values[ 10 ][ ( hash & 0xF0000000000 ) >> 40 ]++;
      values[  9 ][ ( hash & 0xF000000000 ) >> 36 ]++;
      values[  8 ][ ( hash & 0xF00000000 ) >> 32 ]++;
    case 4:
      values[  7 ][ ( hash & 0xF0000000 ) >> 28 ]++;
      values[  6 ][ ( hash & 0xF000000 ) >> 24 ]++;
      values[  5 ][ ( hash & 0xF00000 ) >> 20 ]++;
      values[  4 ][ ( hash & 0xF0000 ) >> 16 ]++;
      values[  3 ][ ( hash & 0xF000 ) >> 12 ]++;
      values[  2 ][ ( hash & 0xF00 ) >> 8 ]++;
      values[  1 ][ ( hash & 0xF0 ) >> 4 ]++;
      values[  0 ][ ( hash & 0xF ) ]++;
    }
  }
  double fitness( ) const noexcept {
    ::std::array<double, 16> errors;
    for( unsigned i{ 0 }; i < values.size( ); ++i ) {
      errors[ i ] = values[ i ].normalized_std_dev( );
    }
    //const double weight{ 1. / static_cast< double >( values.size( ) ) };
    double mean_square_error{ 0 };
    for( auto it : errors ) {
      mean_square_error += it * it;
    }
    //mean_square_error *= weight;
    return ::std::sqrt(mean_square_error);
  }
  friend ::std::ostream& operator<<( ::std::ostream &os, const hash_distribution &rhs ) {
    // print table header
    os << "   ";
    for( size_t i{ 0 }; i < rhs.values.size(); ++i ) {
      os << ::ftl::io::left_pad( 10, ::ftl::io::bits<size_t, 4>( i ) );
    }
    os << ::ftl::io::left_pad( 10, "RMS" );

    for( unsigned i{ 0 }; i < rhs.values.size( ); ++i ) {
      os << std::endl << ::ftl::io::left_pad( 3, i ) << rhs.values[ i ];
    }
    return os << std::endl << "Distribution fitness: " << rhs.fitness( );
  }
};


struct profile_results {
  double best, worst, mean;
  std::string name;
  hash_distribution<uint64_t> distribution;
  friend ::std::ostream& operator<<( std::ostream& os, const profile_results &pr ) {
    return os
      << pr.name << " timing results:\n"
      << "Best case: " << pr.best << ".\n"
      << "Mean case: " << pr.mean << ".\n"
      << "Worst case: " << pr.worst << ".\n"
      << "Hash fitness: " << pr.distribution.fitness( );
      //<< "Distribution:\n" << pr.distribution;
  }
};
template<typename Hasher, typename T>
profile_results profile_hasher( const ftl::vector<T>& keys, size_t iterations = 10 ) {
  static_assert( sizeof( typename ::ftl::hash_traits<Hasher>::hash_type ) == 8, "Currently only supports 64-bit hashes." );
  ftl::hash<T, Hasher> hasher;
  ftl::inline_vector<double, 10> times;
  ftl::vector< typename ftl::hash_traits< Hasher >::hash_type > results;
  results.reserve( keys.size( ) );
  profile_results ret;
  for( size_t i{ 0 }; i < iterations; ++i ) {
    times.push_back( 0.0 );
    {
      ftl::scope_timer<> timer( &times.back( ) );
      for( size_t i{ 0 }; i < keys.size( ); ++i ) {
        results.push_back( hasher( keys[ i ] ) );
        ret.distribution.add_hash( results.back( ) );
      }
    }
    if( results.size( ) != keys.size( ) ) {
      ftl::println( "WTF" );
    }
    results.clear( );
  }
  std::sort( times.begin( ), times.end( ) );
  ret.best = times.front( );
  ret.worst = times.back( );
  times.erase( times.begin( ) );
  times.pop_back( );
  ret.mean = 0.;
  for( auto it : times ) ret.mean += it;
  ret.mean /= times.size( );
  ret.name = typeid( hasher ).name( );
  return ret;
}
/*
#include <fstream>
ftl::vector<std::string> load_dict( const std::string& path ) {
  ftl::vector<std::string> words;
  std::ifstream infile( path );
  if( !infile.good( ) ) return words;
  char buffer[ 1000 ];
  while( !infile.eof( ) ) {
    infile.getline( buffer, 1000 );
    words.push_back( buffer );
  }
  return words;
}

int main3() {
  //constexpr uint64_t hash{ "Hello world"_hash };
  //ftl::println( "\"Hello world\"_hash is ", hash, "." );
  //ftl::vector<int> keys;
  //keys.reserve( 100000000 );
  //for( int i{ 0 }; i < 100000000; ++i ) keys.push_back( i );
  const ftl::vector<std::string> keys = load_dict( "engdict.txt" );
  ftl::vector<int> int_keys;
  int_keys.reserve( 10000000 );
  for( int i{ 0 }; i < 10000000; ++i ) int_keys.push_back( i );
  ftl::println( "Dictionary loaded with ", keys.size(), " keys." );
  uint64_t hash_result_t;
  ftl::inline_vector<profile_results, 4> results;
  results.push_back( profile_hasher< ftl::ftlhash< decltype( hash_result_t )>, decltype( keys )::value_type >( keys ) );
  results.push_back( profile_hasher< ftl::murmur2a<decltype( hash_result_t )>, decltype( keys )::value_type >( keys ) );
  results.push_back( profile_hasher< ftl::fnv1<    decltype( hash_result_t )>, decltype( keys )::value_type >( keys ) );
  results.push_back( profile_hasher< ftl::fnv1a<   decltype( hash_result_t )>, decltype( keys )::value_type >( keys ) );
  results.push_back( profile_hasher< ftl::ftlhash< decltype( hash_result_t )>, decltype( int_keys )::value_type >( int_keys ) );
  results.push_back( profile_hasher< ftl::murmur2a<decltype( hash_result_t )>, decltype( int_keys )::value_type >( int_keys ) );
  results.push_back( profile_hasher< ftl::fnv1<    decltype( hash_result_t )>, decltype( int_keys )::value_type >( int_keys ) );
  results.push_back( profile_hasher< ftl::fnv1a<   decltype( hash_result_t )>, decltype( int_keys )::value_type >( int_keys ) );
  for( auto& it : results ) ftl::println( it );



  system( "pause" );
  return 0;
}
*/
