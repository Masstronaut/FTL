// All content copyright (C) Allan Deutsch 2017. All rights reserved.
#pragma once
#include <iostream>
#include <sstream>
#include <tuple>
#include <bitset>
namespace ftl {

  namespace io {
    namespace detail {
      template<typename T>
      struct left_pad {
        uint32_t pad;
        const T &object;
        char pad_byte;
        friend ::std::ostream& operator<<( ::std::ostream &os, const left_pad<T> &pad ) {
          ::std::stringstream ss;
          ss << pad.object;
          std::string obj_stream( ss.str( ) );
          const int32_t amount{ static_cast< int32_t >( pad.pad - obj_stream.size( ) ) };
          for( int32_t i{ 0 }; i < amount; ++i ) {
            os << pad.pad_byte;
          }
          return os << obj_stream;
        }
      };
      template<typename T>
      struct right_pad {
        const T &object;
        uint32_t pad;
        char pad_byte;
        friend ::std::ostream& operator<<( ::std::ostream &os, const right_pad<T> &pad ) {
          ::std::stringstream ss;
          ss << pad.object;
          std::string obj_stream( ss.str( ) );
          os << obj_stream;
          const int32_t amount{ static_cast< int32_t >( pad.pad - obj_stream.size( ) ) };
          for( int32_t i{ 0 }; i < amount; ++i ) {
            os << pad.pad_byte;
          }
          return os;
        }
      };
    } // detail
    template<typename T>
    detail::left_pad<T> left_pad( uint32_t pad, const T &object, char pad_byte = ' ' ) {
      return detail::left_pad<T>{ pad, object, pad_byte };
    }
    template<typename T>
    detail::right_pad<T> right_pad( const T &object, uint32_t pad, char pad_byte = ' ' ) {
      return detail::right_pad<T>{ object, pad, pad_byte };
    }


    template<typename T, size_t Bits = sizeof(T) * CHAR_BIT>
    ::std::string bits( const T &value, size_t chunk_size = 4 ) {
      ::std::string str{ ::std::bitset<Bits>( value ).to_string( ) };
      int64_t i{ -1 };
      for( auto it{ str.begin( ) }; it != str.end( ); ++it, ++i) {
        if( ( i != 0 ) && ( i % (chunk_size + 1 )== 0 ) ) {
          i = 0;
          it = str.insert( it, ' ' );
        }
      }
      return str;
    }
  } // io

  // Type-safe print function that automatically appends the data with a newline.
  inline void println() {
    std::cout << std::endl;
  }
  // Type-safe print function that automatically appends the data with a newline.
  template<typename T>
  inline void println( const T &arg ) {
    std::cout << arg << std::endl;
  }

  // Type-safe print function that automatically inserts a newline at the end of the string.
  template<typename T, typename... Args>
  inline void println( const T& arg, const Args&... args ) {
    std::cout << arg;
    println( args... );
  }

  // Type-safe printf alternative.
  template<typename T>
  inline void print( T&& arg ) {
    std::cout << arg;
  }

  // Type-safe printf alternative.
  template<typename T, typename... Args>
  inline void print( T&& arg, Args&&... args ) {
    std::cout << arg;
    print( std::forward<Args>( args )... );
  }
}
