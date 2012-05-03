#!/bin/bash 

WARP=1
HELP="no"
JUST_BUILD="no"
SHOREONLY="no"
TWOVEHICLES="no"
BAD_ARGS=""
KEY="lemon"
SURVEY_X=80
SURVEY_Y=-100
HEIGHT1=150
HEIGHT2=150
WIDTH1=150
WIDTH2=150
LANE_WIDTH1=30
LANE_WIDTH2=30
DEGREES1=225
DEGREES2=0
COOL_FAC=10
COOL_STEPS=1000
CONCURRENT="false"

#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
let COUNT=0
for ARGI; do
    UNDEFINED_ARG=$ARGI
    if [ "${ARGI:0:6}" = "--warp" ] ; then
	WARP="${ARGI#--warp=*}"
	UNDEFINED_ARG=""
    fi
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
    # Handle Warp shortcut
    if [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$COUNT" = 0 ]; then 
        WARP=$ARGI
        let "COUNT=$COUNT+1"
        UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_BUILD="yes"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--all" -o "${ARGI}" = "-a" ] ; then
	TWOVEHICLES="yes"
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
    printf "  --warp=WARP_VALUE      \n" 
    printf "  --all, -a              \n" 
    printf "  --concurrent, -c       \n" 
    printf "  --angle=DEGREE_VALUE   \n" 
    printf "  --cool=COOL_VALUE      \n" 
    printf "  --just_build, -j       \n" 
    printf "  --help, -h             \n" 
    exit 0;
fi

# Second check that the warp, height, width arguments are numerical
if [ "${WARP//[^0-9]/}" != "$WARP" ]; then 
    printf "Warp values must be numerical. Exiting now."
    exit 127
fi
if [ "${HEIGHT//[^0-9]/}" != "$HEIGHT" ]; then 
    printf "Height values must be numerical. Exiting now."
    exit 127
fi
if [ "${WIDTH//[^0-9]/}" != "$WIDTH" ]; then 
    printf "Width values must be numerical. Exiting now."
    exit 127
fi

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------

GROUP12="GROUP12"
VNAME1="henry"  # The first vehicle Community
VPORT1="9201"
LPORT1="9301"
VNAME2="gilda"  # The second vehicle Community
VPORT2="9202"
LPORT2="9302"
# gateway buoy
VNAME3="gw_buoy"  # The gateway buoy Community
VPORT3="9203"
LPORT3="9303"

SNAME="shoreside"  # Shoreside Community
SPORT="9000"
SLPORT="9200"

START_POS1="50,0"         # Vehicle 1 Behavior configurations
START_POS2="150,-20"        # Vehicle 2 Behavior configurations
START_POS3="90,-50"        # Gateway configurations

if [ "${SHOREONLY}" != "yes" ]; then

    nsplug meta_vehicle.moos targ_henry.moos -f WARP=$WARP      \
	VNAME=$VNAME1  VPORT=$VPORT1  LPORT=$LPORT1             \
        GROUP=$GROUP12  START_POS=$START_POS1  KEY=$KEY \
        COOL_FAC=$COOL_FAC COOL_STEPS=$COOL_STEPS CONCURRENT=$CONCURRENT
    
    nsplug meta_vehicle.bhv targ_henry.bhv -f VNAME=$VNAME1     \
	START_POS=$START_POS1 SURVEY_X=$SURVEY_X SURVEY_Y=$SURVEY_Y \
        HEIGHT=$HEIGHT1   WIDTH=$WIDTH1 LANE_WIDTH=$LANE_WIDTH1 DEGREES=$DEGREES1          
    if [ "${TWOVEHICLES}" = "yes" ]; then

	nsplug meta_vehicle.moos targ_gilda.moos -f WARP=$WARP      \
	    VNAME=$VNAME2  VPORT=$VPORT2  LPORT=$LPORT2             \
            GROUP=$GROUP12  START_POS=$START_POS2  KEY=$KEY \
            COOL_FAC=$COOL_FAC COOL_STEPS=$COOL_STEPS CONCURRENT=$CONCURRENT
	
	nsplug meta_vehicle.bhv targ_gilda.bhv -f VNAME=$VNAME2     \
	    START_POS=$START_POS2 SURVEY_X=$SURVEY_X SURVEY_Y=$SURVEY_Y \
            HEIGHT=$HEIGHT2   WIDTH=$WIDTH2 LANE_WIDTH=$LANE_WIDTH2 DEGREES=$DEGREES2      
	
    fi    
fi    

# gateway buoy

    nsplug meta_gateway.moos targ_gateway.moos -f WARP=$WARP      \
	VNAME=$VNAME3  VPORT=$VPORT3  LPORT=$LPORT3             \
        GROUP=$GROUP12  START_POS=$START_POS3  KEY=$KEY 
    
nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$WARP    \
    SLPORT=$SLPORT  SPORT=$SPORT  SNAME=$SNAME
    
    
if [ ${JUST_BUILD} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------

if [ "${SHOREONLY}" != "yes" ]; then
    printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $WARP
    pAntler targ_henry.moos >& /dev/null &
    sleep 0.1
    if [ "${TWOVEHICLES}" = "yes" ]; then
	printf "Launching $VNAME2 MOOS Community (WARP=%s) \n" $WARP
	pAntler targ_gilda.moos >& /dev/null &
	sleep 0.1
    fi
fi

# printf "Launching $VNAME3 MOOS Community (WARP=%s) \n" $WARP
# pAntler targ_gateway.moos >& /dev/null &
#sleep 0.1


printf "Launching $SNAME MOOS Community (WARP=%s) \n"  $WARP
pAntler targ_shoreside.moos >& /dev/null &
printf "Done \n"

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
    printf "Killing all processes ... \n"
    kill %1 %2 %3 %4 %5
    printf "Done killing processes.   \n"
fi
