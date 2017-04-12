// All content copyright (C) Allan Deutsch 2017. All rights reserved.
#pragma once
#include <chrono>
#include <ratio>

namespace ftl {

  // Usage:
  // pass in the address of a variable of type T when constructing the ScopeTimer.
  // When the ScopeTimer leaves scope, the time elapsed since it was created will be stored at the input address.
  // Example:
  // {
  //    double time;
  //    {
  //      ScopeTimer(&time);
  //      // run code that is being timed.
  //    } // time will now contain the elapsed time in seconds.
  // }
  template<typename T = double>
  class scope_timer {
    using Tsec = std::chrono::duration<double>;
    using time_point = std::chrono::high_resolution_clock::time_point;

  public:
    scope_timer( T* result ) : m_start( std::chrono::high_resolution_clock::now( ) ), m_result( *result ) { }
    scope_timer( T& result ) : m_start( ::std::chrono::high_resolution_clock::now( ) ), m_result( result ) { }
    ~scope_timer( ) {
      // The divide is necessary because the CPU frequency acquired with Ticks() isn't known at compile time.
      Tsec elapsed{ std::chrono::high_resolution_clock::now( ) - m_start };
      m_result = elapsed.count( );
    }
  private:
    // start and end time stamps
    time_point m_start;

    // Storage location for the elapsed time.
    double &m_result;
  };

  template<typename T>
  scope_timer<T> time_scope( T* data ) {
    return scope_timer<T>( data );
  }
}
