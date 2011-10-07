//  chrono.hpp  --------------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

/*

This code was derived by Beman Dawes from Howard Hinnant's time2_demo prototype.
Many thanks to Howard for making his code available under the Boost license.
The original code was modified to conform to Boost conventions and to section
20.9 Time utilities [time] of the C++ committee's working paper N2798.
See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2798.pdf.

time2_demo contained this comment:

    Much thanks to Andrei Alexandrescu,
                   Walter Brown,
                   Peter Dimov,
                   Jeff Garland,
                   Terry Golubiewski,
                   Daniel Krugler,
                   Anthony Williams.
*/

/*

TODO:

  * Fully implement error handling, with test cases.
  * Use boost::throw_exception. (Currently not used because of an issue with Intel 11.0.)
  * Consider issues raised by Michael Marcin:

    > In the past I've seen QueryPerformanceCounter give incorrect results,
    > especially with SpeedStep processors on laptops. This was many years ago and
    > might have been fixed by service packs and drivers.
    >
    > Typically you check the results of QPC against GetTickCount to see if the
    > results are reasonable.
    > http://support.microsoft.com/kb/274323
    >
    > I've also heard of problems with QueryPerformanceCounter in multi-processor
    > systems.
    >
    > I know some people SetThreadAffinityMask to 1 for the current thread call
    > their QueryPerformance* functions then restore SetThreadAffinityMask. This
    > seems horrible to me because it forces your program to jump to another
    > physical processor if it isn't already on cpu0 but they claim it worked well
    > in practice because they called the timing functions infrequently.
    >
    > In the past I have chosen to use timeGetTime with timeBeginPeriod(1) for
    > high resolution timers to avoid these issues.

*/

#ifndef BOOST_CHRONO_SYSTEM_CLOCKS_HPP
#define BOOST_CHRONO_SYSTEM_CLOCKS_HPP

#include <boost/chrono/config.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/chrono/time_point.hpp>
#include <boost/chrono/detail/system.hpp>
#include <boost/system/error_code.hpp>

#include <ctime>

#ifdef BOOST_CHRONO_WINDOWS_API
// The system_clock tick is 100 nanoseconds
# define BOOST_SYSTEM_CLOCK_DURATION boost::chrono::duration<boost::int_least64_t, ratio<BOOST_RATIO_INTMAX_C(1), BOOST_RATIO_INTMAX_C(10000000)> >
#else
# define BOOST_SYSTEM_CLOCK_DURATION boost::chrono::nanoseconds
#endif

#ifndef BOOST_CHRONO_HEADER_ONLY
#include <boost/config/abi_prefix.hpp> // must be the last #include
#endif


//----------------------------------------------------------------------------//
//                                                                            //
//                        20.9 Time utilities [time]                          //
//                                 synopsis                                   //
//                                                                            //
//----------------------------------------------------------------------------//

namespace boost {
namespace chrono {

  // Clocks
  class BOOST_CHRONO_DECL system_clock;
#ifdef BOOST_CHRONO_HAS_CLOCK_STEADY
  class BOOST_CHRONO_DECL steady_clock; // as permitted by [time.clock.steady]
#endif

#ifdef BOOST_CHRONO_HAS_CLOCK_STEADY
  typedef steady_clock high_resolution_clock;  // as permitted by [time.clock.hires]
#else
  typedef system_clock high_resolution_clock;  // as permitted by [time.clock.hires]
#endif

//----------------------------------------------------------------------------//
//                                                                            //
//      20.9.5 Clocks [time.clock]                                            //
//                                                                            //
//----------------------------------------------------------------------------//

// If you're porting, clocks are the system-specific (non-portable) part.
// You'll need to know how to get the current time and implement that under now().
// You'll need to know what units (tick period) and representation makes the most
// sense for your clock and set those accordingly.
// If you know how to map this clock to time_t (perhaps your clock is std::time, which
// makes that trivial), then you can fill out system_clock's to_time_t() and from_time_t().

//----------------------------------------------------------------------------//
//      20.9.5.1 Class system_clock [time.clock.system]                       //
//----------------------------------------------------------------------------//

  class BOOST_CHRONO_DECL system_clock
  {
  public:
      typedef BOOST_SYSTEM_CLOCK_DURATION          duration;
      typedef duration::rep                        rep;
      typedef duration::period                     period;
      typedef chrono::time_point<system_clock>     time_point;
      BOOST_CHRONO_STATIC_CONSTEXPR bool is_steady =             false;

      static BOOST_CHRONO_INLINE time_point  now() BOOST_CHRONO_NOEXCEPT;
      static BOOST_CHRONO_INLINE time_point  now(system::error_code & ec);

      static BOOST_CHRONO_INLINE std::time_t to_time_t(const time_point& t) BOOST_CHRONO_NOEXCEPT;
      static BOOST_CHRONO_INLINE time_point  from_time_t(std::time_t t) BOOST_CHRONO_NOEXCEPT;
  };

//----------------------------------------------------------------------------//
//      20.9.5.2 Class steady_clock [time.clock.steady]                 //
//----------------------------------------------------------------------------//

// As permitted  by [time.clock.steady]
// The class steady_clock is conditionally supported.

#ifdef BOOST_CHRONO_HAS_CLOCK_STEADY
  class BOOST_CHRONO_DECL steady_clock
  {
  public:
      typedef nanoseconds                          duration;
      typedef duration::rep                        rep;
      typedef duration::period                     period;
      typedef chrono::time_point<steady_clock>  time_point;
      BOOST_CHRONO_STATIC_CONSTEXPR bool is_steady =             true;

      static BOOST_CHRONO_INLINE time_point  now() BOOST_CHRONO_NOEXCEPT;
      static BOOST_CHRONO_INLINE time_point  now(system::error_code & ec);
  };
#endif
//----------------------------------------------------------------------------//
//      20.9.5.3 Class high_resolution_clock [time.clock.hires]               //
//----------------------------------------------------------------------------//

//  As permitted, steady_clock or system_clock is a typedef for high_resolution_clock.
//  See synopsis.


} // namespace chrono
} // namespace boost

#ifndef BOOST_CHRONO_HEADER_ONLY
#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas
#else
#include <boost/chrono/detail/inlined/chrono.hpp>
#endif

#endif // BOOST_CHRONO_SYSTEM_CLOCKS_HPP
