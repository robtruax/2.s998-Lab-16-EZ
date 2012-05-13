#--------------------------------------------------------
# The CMakeLists.txt for:                       pGenetic
# Author(s):                              
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  Genetic.cpp
  Genetic_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pGenetic ${SRC})

TARGET_LINK_LIBRARIES(pGenetic
   MOOS
   MOOSGen
   mbutil
   m
   pthread)

