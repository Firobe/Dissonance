# Dissonance
This library provides a simple way to setup a Discord Bot in C++. It implements a subset of the Discord API (see "Development state"). It is provided as a shared library for Linux.
This library depends on [cppRest](https://github.com/Microsoft/cpprestsdk).
Only Linux is supported at the moment.

# Installation
CMake is required to build and install the library. Make a `build` directory in
the projet root, and run `cmake ..` from it.  
If the configuration is successful, run `make` then `make install`.  
The library will be installed as `/usr/lib/libdissonance.so` and its headers in
`/usr/include/dissonance/`.  

# Usage
Code that uses the API must `#include <Dissonance.hpp>`  
The user must make a class which inherits from `DiscordBot` and implements the
desired methods (see "Event handlers").  

In these methods, the user can call API functions (see the list in "API methods") via the `api` member variable. For example,
`api->triggerTyping(channelId)`.  

Then, this class must be instanciated and its `start()` method should be
called, with the Discord Bot Token provided as a string parameter.  
If the connection between the bot and Discord is broken (as it happens
frequently), the library will seamlessly attempt to reconnect. However, if the
library fails to reconnect multiple times, the bot will stop and the user
should handle it.  

A working example is provided in the `example` directory, along with
a CMakelists.

# Event handlers

# API methods
