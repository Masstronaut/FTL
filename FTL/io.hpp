// All content copyright (C) Allan Deutsch 2017. All rights reserved.
#pragma once
#include <iostream>
namespace ftl {


  // Type-safe printf alternative that automatically inserts a newline at the end of the string.
  inline void print( const char *str ) {
    std::cout << str << std::endl;
  }

  // Type-safe printf alternative that automatically inserts a newline at the end of the string.
  template<typename T, typename... Args>
  inline void print( const char *str, T&& arg, Args&&... args ) {
    const char *begin{ str };
    const char *probe{ begin };
    while( *probe != '\0' ) {
      if( *probe == '{' && *( probe + 1 ) == '}' ) {
        std::cout.write( begin, probe - begin ) << arg;
        probe += 2;
        break;
      } else {
        ++probe;
      }
    }
    print( probe, std::forward<Args>( args )... );
  }

  // Type-safe printf alternative.
  inline void println( const char *str ) {
    std::cout << str;
  }

  // Type-safe printf alternative.
  template<typename T, typename... Args>
  inline void println( const char *str, T&& arg, Args&&... args ) {
    const char *begin{ str };
    const char *probe{ begin };
    while( *probe != '\0' ) {
      if( *probe == '{' && *( probe + 1 ) == '}' ) {
        std::cout.write( begin, probe - begin ) << arg;
        probe += 2;
        break;
      } else {
        ++probe;
      }
    }
    println( probe, std::forward<Args>( args )... );
  }

}
