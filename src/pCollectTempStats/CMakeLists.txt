#--------------------------------------------------------
# The CMakeLists.txt for:                       pCollectTempStats
# Author(s):                              robtruax
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  CollectTempStats.cpp
  CollectTempStats_Info.cpp
  main.cpp
)

include_directories(/usr/include/eigen3)

ADD_EXECUTABLE(pCollectTempStats ${SRC})

TARGET_LINK_LIBRARIES(pCollectTempStats
   MOOS
   MOOSGen
   mbutil
   m
   pthread)

