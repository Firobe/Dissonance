cmake_minimum_required(VERSION 3.1.0)

project(Dissonance CXX C)
set(THREADS_PREFER_PTHREADS_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -O3")

add_library(dissonance SHARED
	src/DiscordAPI.cpp
	src/DiscordBot.cpp
	src/Embed.cpp
	src/Message.cpp
	src/User.cpp
	src/Role.cpp
	src/Channel.cpp
	src/GuildMember.cpp
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

target_link_libraries(dissonance
	${cpprest_LIBRARIES}
	${Boost_LIBRARIES}
	Threads::Threads
	${OPENSSL_LIBRARIES}
	)

install(TARGETS dissonance
	DESTINATION /usr/lib/
	)
install(FILES
	src/DiscordAPI.hpp
	src/DiscordBot.hpp
	src/Embed.hpp
	src/User.hpp
	src/Role.hpp
	src/Message.hpp
	src/Channel.hpp
	DESTINATION /usr/include/dissonance/
	)
install(FILES src/Dissonance.hpp
	DESTINATION /usr/include/
	)
