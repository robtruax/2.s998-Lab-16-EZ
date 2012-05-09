#!/bin/bash 

WARP=1
HELP="no"
JUST_BUILD="no"
AEGIR="no"
BURI="no"
BAD_ARGS=""

#-------------------------------------------------------
#  Part 1: Process command-line arguments
#-------------------------------------------------------
for ARGI; do
    UNDEFINED_ARG=$ARGI
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	HELP="yes"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--buri" -o "${ARGI}" = "-b" ] ; then
      BURI="yes"
      UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--aegir" -o "${ARGI}" = "-a" ] ; then
      AEGIR="yes"
      UNDEFINED_ARG=""
    fi
    if [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_BUILD="yes"
	UNDEFINED_ARG=""
    fi
    if [ "${ARGI:0:7}" = "--shore" ] ; then
	SHOREIP="${ARGI#--shore=*}"
	UNDEFINED_ARG=""
    fi
    if [ "${UNDEFINED_ARG}" != "" ] ; then
	BAD_ARGS=$UNDEFINED_ARG
    fi
done

#-------------------------------------------------------
#  Part 2: Handle Ill-formed command-line arguments
#-------------------------------------------------------


if [ "${BAD_ARGS}" != "" ] ; then
    printf "Bad Argument: %s \n" $BAD_ARGS
    exit 0
fi

if [ "${AEGIR}" = "no" -a "${BURI}" = "no" ] ; then
    printf "ONE vehicle MUST be selected!!!!!!!!!!!! \n"
    HELP="yes"
fi

if [ "${AEGIR}" = "yes" -a "${BURI}" = "yes" ] ; then
    printf "ONE vehicle MUST be selected!!!!!!!!!!!! \n"
    HELP="yes"
fi

if [ "${HELP}" = "yes" ]; then
    printf "%s [SWITCHES]            \n" $0
    printf "Switches:                \n"
    printf "  --aegir, -a           aegir vehicle only                   \n"
    printf "  --buri, -b            buri vehicle only                    \n"
    printf "  --just_build, -j       \n" 
    printf "  --help, -h             \n" 
    exit 0;
fi

#-------------------------------------------------------
#  Part 3: Create the .moos and .bhv files. 
#-------------------------------------------------------

CRUISESPEED="1.8"

VNAME1="aegir"  # The first vehicle Community
VPORT1="9100"
LPORT1="9101"
LOITER_PT1="x=-10,y=-60"
RETURN_PT1="0,-20"

VNAME2="buri"  # The second vehicle Community
VPORT2="9200"
LPORT2="9201"
LOITER_PT2="x=50,y=-40"
RETURN_PT2="30,-10"

# Conditionally Prepare Aegir files
if [ "${AEGIR}" = "yes" ]; then
    nsplug meta_vehicle_fld.moos targ_aegir.moos -f      \
      VNAME=$VNAME1 VPORT=$VPORT1 LPORT=$LPORT1 WARP=$WARP     

    nsplug meta_vehicle.bhv targ_aegir.bhv -f            \
        VNAME=$VNAME1                                     \
	CRUISESPEED=$CRUISESPEED                          \
        RETURN_PT=$RETURN_PT1                             \
        LOITER_PT=$LOITER_PT1
fi

# Conditionally Prepare Buri files
if [ "${BURI}" = "yes" ]; then
    nsplug meta_vehicle_fld.moos targ_buri.moos -f       \
      VNAME=$VNAME2 VPORT=$VPORT2 LPORT=$LPORT2 WARP=$WARP     

    nsplug meta_vehicle.bhv targ_buri.bhv -f             \
            VNAME=$VNAME2                                 \
      CRUISESPEED=$CRUISESPEED                            \
        RETURN_PT=$RETURN_PT2                             \
        LOITER_PT=$LOITER_PT2
fi

if [ ${JUST_BUILD} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 4: Launch the processes
#-------------------------------------------------------

# Launch Aegir
if [ "${AEGIR}" = "yes" ]; then
    printf "Launching Aegir MOOS Community \n"
    pAntler targ_aegir.moos >& /dev/null &
fi

# Launch Buri
if [ "${BURI}" = "yes" ]; then
    printf "Launching Buri MOOS Community \n"
    pAntler targ_buri.moos >& /dev/null &
fi

#-------------------------------------------------------
#  Part 5: Exiting and/or killing the simulation
#-------------------------------------------------------

ANSWER="0"
while [ "${ANSWER}" != "1" -a "${ANSWER}" != "2" ]; do
    printf "Now what? (1) Exit script (2) Exit and Kill Simulation \n"
    printf "> "
    read ANSWER
done

# %1 matches the PID of the first job in the active jobs list, 
# namely the pAntler job launched in Part 4.
if [ "${ANSWER}" = "2" ]; then
    printf "Killing all processes ... \n "
    kill %1 
    printf "Done killing processes.   \n "
fi

