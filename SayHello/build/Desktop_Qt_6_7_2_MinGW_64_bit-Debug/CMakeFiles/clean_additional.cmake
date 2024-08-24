# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\SayHello_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SayHello_autogen.dir\\ParseCache.txt"
  "SayHello_autogen"
  )
endif()
