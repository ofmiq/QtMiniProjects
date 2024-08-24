# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\FeedbackForm_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\FeedbackForm_autogen.dir\\ParseCache.txt"
  "FeedbackForm_autogen"
  )
endif()
