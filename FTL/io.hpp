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
          for( int32_t i{ static_cast<int32_t>( pad.pad - obj_stream.size( ) ) }; i > 0; --i ) {
            os << pad.pad_byte;
          }
          return os << obj_stream;
        }
      };
      template<typename T>
      struct right_pad{
        const T &object;
        uint32_t pad;
        char pad_byte;
        friend ::std::ostream& operator<<( ::std::ostream &os, const right_pad<T> &pad ) {
          ::std::stringstream ss;
          ss << pad.object;
          std::string obj_stream( ss.str( ) );
          os << obj_stream;
          for( int i{ static_cast<int32_t>( pad.pad - obj_stream.size( ) ) }; i > 0; --i ) {
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
      return detail::left_pad<T>{ object, pad, pad_byte };
    }


    template<typename T>
    ::std::bitset<sizeof( T )> bits( const T &value ) {
      return ::std::bitset<sizeof( T )>( value );
    }
  } // io

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
