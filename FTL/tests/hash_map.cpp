// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#include "../container_traits.hpp"
#include "../hash_map.hpp"
#include "../vector.hpp"
#include "../timer.hpp"
#include "../hash.hpp"
#include "../io.hpp"

#include <vector>
#include <array>
#include <unordered_set>
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

  const ftl::vector<std::string> eng_keys = load_dict( "engdict.txt" );
  ftl::vector<int> int_keys;
  int_keys.reserve( 10000000 );
  for( int i{ 0 }; i < 10000000; ++i ) int_keys.push_back( i );
  ftl::println( "Dictionary loaded with ", eng_keys.size( ), " keys." );
  std::unordered_set<std::string> keys( eng_keys.begin( ), eng_keys.end( ) );
 

  ftl::hash_map<std::string, int> hm;

  std::unordered_map<std::string, int> um;

  hm.reserve( 100 );
  assert( hm.capacity( ) >= 100 );
  assert( hm.begin( ) == hm.end( ) );
  assert( hm.cbegin( ) == hm.cend( ) );
  hm.reserve( (uint32_t)keys.size( ) );
  assert( hm.capacity( ) >= keys.size( ) );
  for( auto &it : keys ) {
    hm.insert( { it, 0 } );
  }
  // verify collisions are resolved successfully.
  assert( hm.size( ) == keys.size( ) );
  std::vector<int> vi;
  for( auto &it : keys ) {
    hm[ it ] = ( int )it.size( );
  }

  for( size_t i{ 0 }; i < 1000; ++i ) {
    hm.erase( eng_keys[ i ] );
  }
  for( size_t i{ 0 }; i < 1000; ++i ) {
    assert(hm.find( eng_keys[ i ] ) == hm.end( ));
  }
  for( size_t i{ 0 }; i < 1000; ++i ) {
    auto iter{ hm.find( eng_keys[ i ] ) };
    assert( iter == hm.end( ) );
  }
  for( auto &it : hm ) {
    assert( it.first.size( ) == it.second );
  }

  system( "pause" );
  return 0;
}
