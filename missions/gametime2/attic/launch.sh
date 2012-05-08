#!/bin/bash 

WARP=1
HELP="no"
JUST_BUILD="no"
SHOREONLY="no"
ARCHIEONLY="no"
BETTYONLY="no"
SIMULATE="no"
SIMVPLUGFILE="plug_SIMvessel.moos"
REALVPLUGFILE="plug_REALvessel.moos"
BAD_ARGS=""
KEY="lemon"
HEIGHT1=150
HEIGHT2=150
WIDTH1=150
WIDTH2=150
LANE_WIDTH1=30
LANE_WIDTH2=30
DEGREES1=270
DEGREES2=0
CRUISESPEED=1.75

COOL_FAC=50
COOL_STEPS=1000
CONCURRENT="false"
ADAPTIVE="false"

#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
let COUNT=0
for ARGI; do
    UNDEFINED_ARG=$ARGI
    if [ "${ARGI:0:6}" = "--cool" ] ; then
	COOL_FAC="${ARGI#--cool=*}"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI:0:7}" = "--angle" ] ; then
	DEGREES1="${ARGI#--angle=*}"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI:0:5}" = "--key" ] ; then
	KEY="${ARGI#--key=*}"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	HELP="yes"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--shore" -o "${ARGI}" = "-s" ] ; then
	SHOREONLY="yes"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--betty" -o "${ARGI}" = "-B" ] ; then
      BETTYONLY="yes"
      UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--archie" -o "${ARGI}" = "-A" ] ; then
      ARCHIEONLY="yes"
      UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_BUILD="yes"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--adaptive" -o "${ARGI}" = "-a" ] ; then
	ADAPTIVE="true"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--concurrent" -o "${ARGI}" = "-c" ] ; then
	CONCURRENT="true"
	UNDEFINED_ARG=""
    fi
    if [ "${UNDEFINED_ARG}" != "" ] ; then
	BAD_ARGS=$UNDEFINED_ARG
    fi
done

if [ "${BAD_ARGS}" != "" ] ; then
    printf "Bad Argument: %s \n" $BAD_ARGS
    exit 0
fi

if [ "${HELP}" = "yes" ]; then
    printf "%s [SWITCHES]            \n" $0
    printf "Switches:                \n"
    printf "  --shore, -s            Shore station and gateway buoy only   \n"
    printf "  --archie, -A           Archie vehicle only                   \n"
    printf "  --betty, -B            Betty vehicle only                    \n"
    printf "  --sim, -S              Simulate instead of use real hardware \n"
    printf "  --adaptive, -a         \n" 
    printf "  --concurrent, -c       \n" 
    printf "  --angle=DEGREE_VALUE   \n" 
    printf "  --cool=COOL_VALUE      \n" 
    printf "  --just_build, -j       \n" 
    printf "  --help, -h             \n" 
    exit 0;
fi

# Second check that the height, width arguments are numerical
if [ "${COOL_FAC//[^0-9]/}" != "$COOL_FAC" ]; then 
    printf "Cooling factor values must be numerical. Exiting now."
    exit 127
fi
if [ "${DEGREES1//[^0-9]/}" != "$DEGREES1" ]; then 
    printf "Width values must be numerical. Exiting now."
    exit 127
fi

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------

GROUP12="GROUP12"

VNAME1="archie"  # The first vehicle Community
VPORT1="9201"
LPORT1="9301"
STATION_PT1="0,-10"
LOITERTRACK1="format=radial, x=-50, y=-75, radius=50, pts=4, snap1=1, label=ArchieLoiter"
LOITERCW1="true"

VNAME2="betty"  # The second vehicle Community
VPORT2="9202"
LPORT2="9302"
STATION_PT2="-10,-10"
LOITERTRACK2="format=radial, x=-50, y=-75, radius=50, pts=4, snap1=1, label=BettyLoiter"
LOITERCW2="false"

# gateway buoy
VNAME3="gw_buoy"  # The gateway buoy Community
VPORT3="9203"
LPORT3="9303"

SNAME="shoreside"  # Shoreside Community
SPORT="9000"
SLPORT="9200"

# Starting positions are not needed for real vessels, only simulated
START_POS1="50,0"         # Vehicle 1 Behavior configurations
START_POS2="150,-20"        # Vehicle 2 Behavior configurations
START_POS3="90,-50"        # Gateway configurations

# Deal with simulated versus real
if [ "${SIMULATED}" = "yes" ]; then
    SIMORREALPLUG=${SIMVPLUGFILE}
else
    SIMORREALPLUG=${REALVPLUGFILE}
fi

# Prepare Archie files
if [ "${SHOREONLY}" = "no" -a "${BETTYONLY}" = "no" ]; then
    nsplug meta_vehicle.moos targ_archie.moos -f          \
      VNAME=$VNAME1 VPORT=$VPORT1 LPORT=$LPORT1           \
      GROUP=$GROUP12                                      \
      START_POS=$START_POS1                               \
      KEY=$KEY                                            \
      COOL_FAC=$COOL_FAC COOL_STEPS=$COOL_STEPS           \
      CONCURRENT=$CONCURRENT ADAPTIVE=$ADAPTIVE           \
      SIMULATED=$SIMULATED SIMORREALPLUG=$SIMORREALPLUG

    nsplug meta_vehicle.bhv targ_archie.bhv -f            \
      VNAME=$VNAME1 CONTACT=$VNAME2                       \
      STATION_PT=$STATION_PT1                             \
      CRUISESPEED=$CRUISESPEED                            \
      LOITERTRACK=$LOITERTRACK1 LOITERCW=$LOITERCW1       \
      SURVEY_X=$SURVEY_X                                  \
      SURVEY_Y=$SURVEY_Y                                  \
      HEIGHT=$HEIGHT1                                     \
      WIDTH=$WIDTH1                                       \
      LANE_WIDTH=$LANE_WIDTH1                             \
      DEGREES=$DEGREES1
fi

# Prepare Betty files
if [ "${SHOREONLY" != "no" -a "${ARCHIEONLY}" = "no" ]; then
    nsplug meta_vehicle.moos targ_betty.moos -f           \
      VNAME=$VNAME2 VPORT=$VPORT2 LPORT=$LPORT2           \
      GROUP=$GROUP12                                      \
      START_POS=$START_POS2                               \
      KEY=$KEY                                            \
      COOL_FAC=$COOL_FAC COOL_STEPS=$COOL_STEPS           \
      CONCURRENT=$CONCURRENT ADAPTIVE=$ADAPTIVE           \
      SIMULATED=$SIMULATED SIMORREALPLUG=$SIMORREALPLUG

    nsplug meta_vehicle.bhv targ_betty.bhv -f             \
      VNAME=$VNAME2 CONTACT=$VNAME1                       \
      STATION_PT=$STATION_PT2                             \
      CRUISESPEED=$CRUISESPEED                            \
      LOITERTRACK=$LOITERTRACK2 LOITERCW=$LOITERCW2       \
      SURVEY_X=$SURVEY_X                                  \
      SURVEY_Y=$SURVEY_Y                                  \
      HEIGHT=$HEIGHT2                                     \
      WIDTH=$WIDTH2                                       \
      LANE_WIDTH=$LANE_WIDTH2                             \
      DEGREES=$DEGREES2
fi

# Prepate shorestation and gateway buoy files
if [ "${ARCHIEONLY" = "no" -a "${BETTYONLY}" != "no" ]; then
    nsplug meta_gateway.moos targ_gateway.moos -f         \
      VNAME=$VNAME3                                       \
      VPORT=$VPORT3                                       \
      LPORT=$LPORT3                                       \
      GROUP=$GROUP12                                      \
      START_POS=$START_POS3                               \
      KEY=$KEY

    nsplug meta_shoreside.moos targ_shoreside.moos -f     \
      SLPORT=$SLPORT                                      \
      SPORT=$SPORT                                        \
      SNAME=$SNAME

fi

if [ ${JUST_BUILD} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------

# Launch Archie
if [ "${SHOREONLY}" = "no" -a "${BETTYONLY}" = "no" ]; then
    printf "Launching $VNAME1 MOOS Community \n"
    pAntler targ_archie.moos >& /dev/null &
    sleep 0.1
fi

# Launch Betty
if [ "${SHOREONLY" != "no" -a "${ARCHIEONLY}" = "no" ]; then
    printf "Launching $VNAME2 MOOS Community \n"
    pAntler targ_betty.moos >& /dev/null &
    sleep 0.1
fi

# Launch shorestation and gateway buoy
if [ "${ARCHIEONLY" = "no" -a "${BETTYONLY}" != "no" ]; then
    printf "Launching $VNAME3 MOOS Community \n"
    pAntler targ_betty.moos >& /dev/null &
    sleep 0.1

    printf "Launching $SNAME MOOS Community \n"
    pAntler targ_shoreside.moos >& /dev/null &
fi

#-------------------------------------------------------
#  Part 4: Exiting and/or killing the simulation
#-------------------------------------------------------

ANSWER="0"
while [ "${ANSWER}" != "1" -a "${ANSWER}" != "2" ]; do
    printf "Now what? (1) Exit script (2) Exit and Kill Simulation \n"
    printf "> "
    read ANSWER
done

# %1, %2, %3 matches the PID of the first three jobs in the active
# jobs list, namely the three pAntler jobs launched in Part 3.
if [ "${ANSWER}" = "2" ]; then
    printf "Killing all processes ... \n "
    kill %1 %2 %3 %4 %5
    printf "Done killing processes.   \n "
fi
