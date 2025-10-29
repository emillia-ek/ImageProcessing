# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ImageProcessing_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ImageProcessing_autogen.dir/ParseCache.txt"
  "ImageProcessing_autogen"
  )
endif()
