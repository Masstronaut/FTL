// All content copyright (C) Allan Deutsch 2017. All rights reserved.
#pragma once
#include <iostream>
#include <sstream>
#include <tuple>
namespace ftl {

  namespace io {
    namespace detail {
      template<typename T>
      struct left_pad {
        size_t pad;
        const T &object;
        char pad_byte;
        friend ::std::ostream& operator<<( ::std::ostream &os, const left_pad<T> &pad ) {
          ::std::stringstream ss;
          ss << pad.object;
          std::string obj_stream( ss.str( ) );
          for( int i{ 0 }; i < pad.pad - obj_stream.size(); ++i ) {
            ::std::cout << pad.pad_byte;
          }
          ::std::cout << obj_stream;
        }
      };

    } // detail
    template<typename T>
    detail::left_pad<T> left_pad( size_t pad, const T &object, char pad_byte = ' ' ) {
      return detail::left_pad<T>{pad, object, pad_byte};
    }
  } // io

  // Type-safe print function that automatically appends the data with a newline.
  template<typename T>
  inline void print( const T &arg ) {
    std::cout << arg << std::endl;
  }

  // Type-safe print function that automatically inserts a newline at the end of the string.
  template<typename T, typename... Args>
  inline void print( const T& arg, const Args&... args ) {
    std::cout << arg;
    print( std::forward<Args>( args )... );
  }

  // Type-safe printf alternative.
  template<typename T>
  inline void println( T&& arg ) {
    std::cout << arg;
  }

  // Type-safe printf alternative.
  template<typename T, typename... Args>
  inline void println( T&& arg, Args&&... args ) {
    std::cout << arg;
    println( std::forward<Args>( args )... );
  }

}
