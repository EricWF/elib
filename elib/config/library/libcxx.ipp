#ifndef ELIB_CONFIG_LIBRARY_LIBCXX_IPP
#define ELIB_CONFIG_LIBRARY_LIBCXX_IPP
# 
# define ELIB_LIBRARY libcxx
# define ELIB_LIBRARY_VERSION _LIBCPP_VERSION
# define ELIB_CONFIG_LIBCXX 1
# 
# // TODO: Stop assuming LIBCXX has regex
# define ELIB_CONFIG_HAS_REGEX 1
# 
#endif /* ELIB_CONFIG_LIBRARY_LIBCXX_IPP */