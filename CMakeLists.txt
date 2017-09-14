project(libutils)
cmake_minimum_required(VERSION 2.8)

add_definitions( -DLIBRARY_BUILD )

if( BUILDING_FOR_ARM )
	MESSAGE( STATUS "----------- ARM Build ----IPTC WRAPPER-------------" )
	set( LIBRARY_OUTPUT_PATH	${CMAKE_CURRENT_LIST_DIR}/package/integra/lib )

else() # For LINUX Build
	MESSAGE( STATUS "----------- Linux Build --IPTC WRAPPER----------" )
	set( LIBRARY_OUTPUT_PATH	${CMAKE_CURRENT_LIST_DIR}/../lib_linux )
	SET(CMAKE_C_FLAGS "-m64 -pipe -g -Wall -W -fPIC"  CACHE STRING "" FORCE)
	SET(CMAKE_CXX_FLAGS "-m64 -pipe -g -std=c++0x -Wall -W -fPIC -fvisibility=hidden"  CACHE STRING "" FORCE )

	include_directories(
		/usr/include/libiptc
		/usr/rediscpp/include
		/usr/rediscpp/cpp_redis-master/includes/cpp_redis
	)

	link_directories(
		/lib/x86_64-linux-gnu
		/usr/rediscpp/lib
		/usr/include/json-c
		#/usr/include/json-c
	)
endif(BUILDING_FOR_ARM)

file(GLOB SOURCES
	"jsonreport.cpp"
	"logger.cpp"
	"stringutils.cpp"
	"jsonreport.h"
	"stringutils.h"
    "def.h"
)

add_library( ${PROJECT_NAME} SHARED  ${SOURCES} )
SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES PREFIX "")
target_link_libraries( ${PROJECT_NAME} libcpp_redis.so json-c.so )
#TARGET0       = libutils.so
#TARGETD       = libutils.so.1.0.0
#TARGET1       = libutils.so.1
#TARGET2       = libutils.so.1.0
