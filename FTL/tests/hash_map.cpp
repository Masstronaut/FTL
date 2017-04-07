// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#include "../container_traits.hpp"
#include "../hash_map.hpp"
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

int main() {
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
 

  ftl::hash_map<std::string, int> hm;

  std::unordered_map<std::string, int> um;

  hm.reserve( 100 );
  assert( hm.capacity( ) >= 100 );
  assert( hm.begin( ) == hm.end( ) );
  assert( hm.cbegin( ) == hm.cend( ) );
  hm.reserve( keys.size( ) );
  assert( hm.capacity( ) >= keys.size( ) );
  for( auto &it : keys ) {
    hm.insert( { it, 0 } );
  }
  assert( hm.size( ) == keys.size( ) );
  std::vector<int> vi;
  for( auto &it : keys ) {
    hm[ it ] = it.size( );
  }

  system( "pause" );
  return 0;
}
