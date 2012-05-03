#--------------------------------------------------------
# The CMakeLists.txt for:              lib_behaviors-test
# Author(s):                                
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
      )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m )
endif (${WIN32})


MACRO(ADD_BHV BHV_NAME)
  ADD_LIBRARY(${BHV_NAME} SHARED ${BHV_NAME}.cpp)
  TARGET_LINK_LIBRARIES(${BHV_NAME} 
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )
ENDMACRO(ADD_BHV)


#--------------------------------------------------------
#                                      BHV_WaveFollow
#--------------------------------------------------------
ADD_LIBRARY(BHV_WaveFollow SHARED 
   BHV_WaveFollow.cpp )
TARGET_LINK_LIBRARIES(BHV_WaveFollow
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil 
   geometry 
   ${SYSTEM_LIBS} )

#--------------------------------------------------------
#                                      BHV_RubberBand
#--------------------------------------------------------
ADD_LIBRARY(BHV_RubberBandTruax SHARED 
   BHV_RubberBandTruax.cpp )
TARGET_LINK_LIBRARIES(BHV_RubberBandTruax
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil 
   geometry 
   ${SYSTEM_LIBS} )


#--------------------------------------------------------
#                                      BHV_GoSideways
#--------------------------------------------------------
ADD_LIBRARY(BHV_GoSideways SHARED 
   BHV_GoSideways.cpp )
TARGET_LINK_LIBRARIES(BHV_GoSideways
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil 
   geometry 
   ${SYSTEM_LIBS} )


