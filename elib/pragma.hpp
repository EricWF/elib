#ifndef ELIB_PRAGMA_HPP
#define ELIB_PRAGMA_HPP

# include <elib/config.hpp>


# undef ELIB_PRAGMA_DIAG_PUSH
# undef ELIB_PRAGMA_DIAG_POP
# undef ELIB_PRAGMA_DIAG_IGNORE
# undef ELIB_PRAGMA_IGNORE_AGGREGATE_RETURN
# undef ELIB_PRAGMA_IGNORE_EFFCXX
# undef ELIB_PRAGMA_IGNORE_EXIT_TIME_DESTRUCTORS
# undef ELIB_PRAGMA_IGNORE_GLOBAL_CONSTRUCTORS
# undef ELIB_PRAGMA_IGNORE_FORMAT_NONLITERAL
# undef ELIB_PRAGMA_IGNORE_HEADER_HYGIENE
# undef ELIB_PRAGMA_IGNORE_MISSING_VARIABLE_DECLARATIONS
# undef ELIB_PRAGMA_IGNORE_NON_VIRTUAL_DTOR
# undef ELIB_PRAGMA_IGNORE_UNREACHABLE_CODE
# undef ELIB_PRAGMA_IGNORE_UNSAFE_LOOP_OPTIMIZATIONS
# undef ELIB_PRAGMA_IGNORE_WEAK_VTABLES

# define ELIB_PRAGMA_DIAG_PUSH()
# define ELIB_PRAGMA_DIAG_POP()
# define ELIB_PRAGMA_DIAG_IGNORE(d)
# define ELIB_PRAGMA_IGNORE_AGGREGATE_RETURN()
# define ELIB_PRAGMA_IGNORE_EFFCXX()
# define ELIB_PRAGMA_IGNORE_EXIT_TIME_DESTRUCTORS()
# define ELIB_PRAGMA_IGNORE_GLOBAL_CONSTRUCTORS()
# define ELIB_PRAGMA_IGNORE_FORMAT_NONLITERAL()
# define ELIB_PRAGMA_IGNORE_HEADER_HYGIENE()
# define ELIB_PRAGMA_IGNORE_MISSING_VARIABLE_DECLARATIONS()
# define ELIB_PRAGMA_IGNORE_NON_VIRTUAL_DTOR()
# define ELIB_PRAGMA_IGNORE_UNREACHABLE_CODE()
# define ELIB_PRAGMA_IGNORE_UNSAFE_LOOP_OPTIMIZATIONS()
# define ELIB_PRAGMA_IGNORE_WEAK_VTABLES()



# if defined(ELIB_GNU) || defined(ELIB_CLANG)

#   undef ELIB_PRAGMA_DIAG_PUSH
#   undef ELIB_PRAGMA_DIAG_POP
#   undef ELIB_PRAGMA_DIAG_IGNORE
#   undef ELIB_PRAGMA_IGNORE_FORMAT_NONLITERAL

#   define ELIB_PRAGMA_DIAG_PUSH()                                \
      _Pragma(ELIB_STR_DELAY(ELIB_COMPILER_NAME diagnostic push)) \
      _Pragma(ELIB_STR_DELAY(ELIB_COMPILER_NAME diagnostic push)) \
      _Pragma(ELIB_STR_DELAY(ELIB_COMPILER_NAME diagnostic push))
  
#   define ELIB_PRAGMA_DIAG_POP() \
      _Pragma(ELIB_STR_DELAY(ELIB_COMPILER_NAME diagnostic pop))
  
#   define ELIB_PRAGMA_DIAG_IGNORE(d) \
      _Pragma(ELIB_STR_DELAY( ELIB_COMPILER_NAME diagnostic ignored d ))
      

#   define ELIB_PRAGMA_IGNORE_FORMAT_NONLITERAL() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wformat-nonliteral")
  
# endif

# if defined(ELIB_GNU)

#   undef ELIB_PRAGMA_IGNORE_AGGREGATE_RETURN
#   undef ELIB_PRAGMA_IGNORE_EFFCXX
#   undef ELIB_PRAGMA_IGNORE_UNSAFE_LOOP_OPTIMIZATIONS


# define ELIB_PRAGMA_IGNORE_AGGREGATE_RETURN() \
  ELIB_PRAGMA_DIAG_IGNORE("-Waggregate-return")
  
# define ELIB_PRAGMA_IGNORE_EFFCXX() \
  ELIB_PRAGMA_DIAG_IGNORE("-Weffc++")

# define ELIB_PRAGMA_IGNORE_UNSAFE_LOOP_OPTIMIZATIONS() \
  ELIB_PRAGMA_DIAG_IGNORE("-Wunsafe-loop-optimizations")


# elif defined(ELIB_CLANG)

#   undef ELIB_PRAGMA_IGNORE_EXIT_TIME_DESTRUCTORS
#   undef ELIB_PRAGMA_IGNORE_GLOBAL_CONSTRUCTORS
#   undef ELIB_PRAGMA_IGNORE_HEADER_HYGIENE
#   undef ELIB_PRAGMA_IGNORE_MISSING_VARIABLE_DECLARATIONS
#   undef ELIB_PRAGMA_IGNORE_NON_VIRTUAL_DTOR
#   undef ELIB_PRAGMA_IGNORE_UNREACHABLE_CODE
#   undef ELIB_PRAGMA_IGNORE_WEAK_VTABLES

#   define ELIB_PRAGMA_IGNORE_EXIT_TIME_DESTRUCTORS() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wexit-time-destructors")

#   define ELIB_PRAGMA_IGNORE_GLOBAL_CONSTRUCTORS() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wglobal-constructors")
      
#   define ELIB_PRAGMA_IGNORE_HEADER_HYGIENE() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wheader-hygiene")
      
#   define ELIB_PRAGMA_IGNORE_MISSING_VARIABLE_DECLARATIONS() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wmissing-variable-declarations")
      
#   define ELIB_PRAGMA_IGNORE_NON_VIRTUAL_DTOR() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wnon-virtual-dtor")
      
#   define ELIB_PRAGMA_IGNORE_UNREACHABLE_CODE() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wunreachable-code")
      
#   define ELIB_PRAGMA_IGNORE_WEAK_VTABLES() \
      ELIB_PRAGMA_DIAG_IGNORE("-Wweak-vtables")

# endif


#endif                                                      // ELIB_PRAGMA_HPP