cmake_minimum_required (VERSION 2.8)

option(CONFIG_MY_BUILD "These are my specific build settings" ON)

# These options need to go before project declaration
if (CONFIG_MY_BUILD)
	option(CONFIG_OPT_GCC "Use /opt/gnu/bin/* and /opt/gnu/lib64" OFF)
	
	if (CONFIG_OPT_GCC)
		set(CMAKE_C_COMPILER /opt/gnu/bin/gcc)
		set(CMAKE_CXX_COMPILER /opt/gnu/bin/g++)
		set(LD_FLAGS -Wl,-rpath -Wl,/opt/gnu/lib64)
	endif()	
endif()

# we need C to get the pthreads library 
project(EricsLibrary CXX C)

if (CONFIG_MY_BUILD)
	# I like to install in weird places 
	if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/usr" 
			CACHE PATH "library install prefix" FORCE)
		MESSAGE(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
	endif()
endif()
	

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set( CMAKE_BUILD_TYPE RELEASE )
set( CMAKE_CXX_FLAGS_RELEASE "-O3  -D NDEBUG" )
set( CMAKE_C_FLAGS_RELEASE "-O3  -D NDEBUG" )
set( CMAKE_C_FLAGS_DEBUG "-g -O0" )
set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" )

include_directories(${CMAKE_CURRENT_LIST_DIR}/src/include)
find_package( Threads REQUIRED )
add_definitions(-std=c++11 -Wall -Wextra -pedantic)
add_definitions(-include ${CMAKE_CURRENT_LIST_DIR}/src/include/elib/commondef.h)

add_subdirectory(src)
add_subdirectory(tests)

