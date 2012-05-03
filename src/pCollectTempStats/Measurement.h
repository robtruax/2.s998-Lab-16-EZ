/************************************************************/
/*    NAME: robtruax                                              */
/*    ORGN: MIT                                             */
/*    FILE: CollectTempStats.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Measurement_HEADER
#define Measurement_HEADER


class Measurement {
public:
    Measurement() { };

    Measurement(double _x, double _y, double _temp, double _timestamp, std::string _vehicleID) {
	x = _x;
	y = _y;
	temp = _temp;
	timestamp = _timestamp;
	vehicleID = _vehicleID;
    };

public:
    double x, y, temp, timestamp;
    std::string vehicleID;

};

#endif
