//  (C) Copyright John Maddock 2001. 
//  (C) Copyright Peter Dimov 2001. 
//  (C) Copyright Jens Maurer 2001. 
//  (C) Copyright David Abrahams 2002 - 2003. 
//  (C) Copyright Aleksey Gurtovoy 2002 - 2003. 
//  (C) Copyright Guillaume Melquiond 2002 - 2003. 
//  (C) Copyright Beman Dawes 2003. 
//  (C) Copyright Martin Wille 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Intel compiler setup:

#include "sysc/packages/boost/config/compiler/common_edg.hpp"

#if defined(__INTEL_COMPILER)
#  define SC_BOOST_INTEL_CXX_VERSION __INTEL_COMPILER
#elif defined(__ICL)
#  define SC_BOOST_INTEL_CXX_VERSION __ICL
#elif defined(__ICC)
#  define SC_BOOST_INTEL_CXX_VERSION __ICC
#elif defined(__ECC)
#  define SC_BOOST_INTEL_CXX_VERSION __ECC
#endif

#define SC_BOOST_COMPILER "Intel C++ version " SC_BOOST_STRINGIZE(SC_BOOST_INTEL_CXX_VERSION)
#define SC_BOOST_INTEL SC_BOOST_INTEL_CXX_VERSION

#if defined(_WIN32) || defined(_WIN64)
#  define SC_BOOST_INTEL_WIN SC_BOOST_INTEL
#else
#  define SC_BOOST_INTEL_LINUX SC_BOOST_INTEL
#endif

#if (SC_BOOST_INTEL_CXX_VERSION <= 500) && defined(_MSC_VER)
#  define SC_BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
#  define SC_BOOST_NO_TEMPLATE_TEMPLATES
#endif

#if (SC_BOOST_INTEL_CXX_VERSION <= 600)

#  if defined(_MSC_VER) && (_MSC_VER <= 1300) // added check for <= VC 7 (Peter Dimov)

// Boost libraries assume strong standard conformance unless otherwise
// indicated by a config macro. As configured by Intel, the EDG front-end
// requires certain compiler options be set to achieve that strong conformance.
// Particularly /Qoption,c,--arg_dep_lookup (reported by Kirk Klobe & Thomas Witt)
// and /Zc:wchar_t,forScope. See boost-root/tools/build/intel-win32-tools.jam for
// details as they apply to particular versions of the compiler. When the
// compiler does not predefine a macro indicating if an option has been set,
// this config file simply assumes the option has been set.
// Thus SC_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP will not be defined, even if
// the compiler option is not enabled.

#     define SC_BOOST_NO_SWPRINTF
#  endif

// Void returns, 64 bit integrals don't work when emulating VC 6 (Peter Dimov)

#  if defined(_MSC_VER) && (_MSC_VER <= 1200)
#     define SC_BOOST_NO_VOID_RETURNS
#     define SC_BOOST_NO_INTEGRAL_INT64_T
#  endif

#endif

#if (SC_BOOST_INTEL_CXX_VERSION <= 710) && defined(_WIN32)
#  define SC_BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS
#endif

// See http://aspn.activestate.com/ASPN/Mail/Message/boost/1614864
#if SC_BOOST_INTEL_CXX_VERSION < 600
#  define SC_BOOST_NO_INTRINSIC_WCHAR_T
#else
// We should test the macro _WCHAR_T_DEFINED to check if the compiler
// supports wchar_t natively. *BUT* there is a problem here: the standard
// headers define this macro if they typedef wchar_t. Anyway, we're lucky
// because they define it without a value, while Intel C++ defines it
// to 1. So we can check its value to see if the macro was defined natively 
// or not. 
// Under UNIX, the situation is exactly the same, but the macro _WCHAR_T 
// is used instead.
#  if ((_WCHAR_T_DEFINED + 0) == 0) && ((_WCHAR_T + 0) == 0)
#    define SC_BOOST_NO_INTRINSIC_WCHAR_T
#  endif
#endif

#if defined(__GNUC__) && !defined(SC_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL)
//
// Figure out when Intel is emulating this gcc bug:
//
#  if ((__GNUC__ == 3) && (__GNUC_MINOR__ <= 2)) || (SC_BOOST_INTEL <= 900)
#     define SC_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#  endif
#endif

//
// Verify that we have actually got SC_BOOST_NO_INTRINSIC_WCHAR_T
// set correctly, if we don't do this now, we will get errors later
// in type_traits code among other things, getting this correct
// for the Intel compiler is actually remarkably fragile and tricky:
//
#if defined(SC_BOOST_NO_INTRINSIC_WCHAR_T)
#include <cwchar>
namespace sc_boost {
template< typename T > struct assert_no_intrinsic_wchar_t;
template<> struct assert_no_intrinsic_wchar_t<wchar_t> { typedef void type; };
// if you see an error here then you need to unset SC_BOOST_NO_INTRINSIC_WCHAR_T
// where it is defined above:
typedef assert_no_intrinsic_wchar_t<unsigned short>::type assert_no_intrinsic_wchar_t_;
} // namespace sc_boost
#else
namespace sc_boost {
template< typename T > struct assert_intrinsic_wchar_t;
template<> struct assert_intrinsic_wchar_t<wchar_t> {};
// if you see an error here then define SC_BOOST_NO_INTRINSIC_WCHAR_T on the command line:
template<> struct assert_intrinsic_wchar_t<unsigned short> {};
} // namespace sc_boost
#endif

#if _MSC_VER+0 >= 1000
#  if _MSC_VER >= 1200
#     define SC_BOOST_HAS_MS_INT64
#  endif
#  define SC_BOOST_NO_SWPRINTF
#elif defined(_WIN32)
#  define SC_BOOST_DISABLE_WIN32
#endif

// I checked version 6.0 build 020312Z, it implements the NRVO.
// Correct this as you find out which version of the compiler
// implemented the NRVO first.  (Daniel Frey)
#if (SC_BOOST_INTEL_CXX_VERSION >= 600)
#  define SC_BOOST_HAS_NRVO
#endif

//
// versions check:
// we don't support Intel prior to version 5.0:
#if SC_BOOST_INTEL_CXX_VERSION < 500
#  error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version:
#if (SC_BOOST_INTEL_CXX_VERSION > 900)
#  if defined(SC_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  elif defined(_MSC_VER)
#     pragma message("Unknown compiler version - please run the configure tests and report the results")
#  endif
#endif





