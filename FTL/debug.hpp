// All content copyright (C) Allan Deutsch 2017. All rights reserved.
#pragma once
#include "io.hpp"

#include <string>
#include <sstream>

namespace ftl {
  namespace debug {
#if defined(_DEBUG)
    template<typename T>
    struct bits {
      bits( T num, const ::std::string &name )
        : m_value( num )
        , m_name( name ) { }

      bits( T num )
        : m_value( num ) { 
        ::std::stringstream ss;
        ss << num;
        m_name = ss.str( );
      }
      ::std::string name( ) const noexcept {
        return m_name;
      }
      operator T( ) const noexcept {
        return m_value;
      }

#define FTL_DEBUG_BITS_OPERATOR( OP ) template<typename U> \
bits operator##OP( U rhs ) { \
  ::ftl::println( ::ftl::io::bits( m_value ), " " #OP " ", rhs, " ( ", ::ftl::io::bits( rhs ), " ):" ); \
  ::ftl::println( ::ftl::io::bits( m_value OP rhs ), " : ", m_name ); \
  return bits<T>( num, "( " + m_name + #OP + rhs.name( ) + " )" ); \
}
      FTL_DEBUG_BITS_OPERATOR( + );
      FTL_DEBUG_BITS_OPERATOR( - );
      FTL_DEBUG_BITS_OPERATOR( / );
      FTL_DEBUG_BITS_OPERATOR( * );
      FTL_DEBUG_BITS_OPERATOR( % );
      FTL_DEBUG_BITS_OPERATOR( ^ );
      FTL_DEBUG_BITS_OPERATOR( & );
      FTL_DEBUG_BITS_OPERATOR( | );
      FTL_DEBUG_BITS_OPERATOR( = );
      FTL_DEBUG_BITS_OPERATOR( += );
      FTL_DEBUG_BITS_OPERATOR( -= );
      FTL_DEBUG_BITS_OPERATOR( /= );
      FTL_DEBUG_BITS_OPERATOR( *= );
      FTL_DEBUG_BITS_OPERATOR( %= );
      FTL_DEBUG_BITS_OPERATOR( ^= );
      FTL_DEBUG_BITS_OPERATOR( &= );
      FTL_DEBUG_BITS_OPERATOR( |= );
#undef FTL_DEBUG_BITS_OPERATOR

    private:
      T m_value;
      ::std::string m_name;
    };
#else
    template<typename T>
    using bits = T;
#endif
  }
 

}
