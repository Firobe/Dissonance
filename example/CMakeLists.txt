cmake_minimum_required(VERSION 3.1.0)

project(BotExample CXX C)
set(THREADS_PREFER_PTHREADS_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -O3")

add_executable(example
	main.cpp
	bot.cpp
	bot.hpp
	)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpprest REQUIRED)
find_package(Boost 1.58 REQUIRED COMPONENTS system)

include_directories(${cpprest_LIB_DIR}
	${Boost_INCLUDE_DIRS}
	${OPENSSL_INCLUDE_DIR}
	)

target_link_libraries(example
	dissonance
	${cpprest_LIBRARIES}
	${Boost_LIBRARIES}
	Threads::Threads
	${OPENSSL_LIBRARIES}
	)
