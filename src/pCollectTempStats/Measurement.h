/************************************************************/
/*    NAME: robtruax                                              */
/*    ORGN: MIT                                             */
/*    FILE: CollectTempStats.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Measurement_HEADER
#define Measurement_HEADER

#include "MOOSLib.h"
#include "MBUtils.h"
#include <iterator>
#include <iostream>
#include <string>
#include <math.h>

class Measurement {
public:
    Measurement() { };

    Measurement(double _x, double _y, double _temp) {
	x = _x;
	y = _y;
	temp = _temp;
	timestamp = -1;
	vehicleID = "none";	
    };

    Measurement(double _x, double _y, double _temp, double _timestamp, std::string _vehicleID) {
	x = _x;
	y = _y;
	temp = _temp;
	timestamp = _timestamp;
	vehicleID = _vehicleID;
    };

    Measurement(std::string s) {
	this->fromString(s);
    }

    std::string toString() {
      // Format stolen from CTD_Sensor_Model
      std::string out = "vname=" + this->vehicleID 
	+ ",utc=" + doubleToString(this->timestamp,1)
	+",x=" + doubleToString(this->x,1)
	+",y=" + doubleToString(this->y,1)
	+",temp=" + doubleToString(this->temp,2);
	return out;
    }
    
    void fromString(std::string encodedMeasurement) {
      std::vector<std::string> inmsg = parseString(encodedMeasurement, ",");
      for (int i = 0; i < inmsg.size(); i++) {
	std::vector<std::string> items = parseString(inmsg[i],"=");
	if (items.size() == 2) {
	  if (items[0] == "temp") {
	    temp = atof(items[1].c_str());
	  }
	  else if (items[0] == "x") {
	    x = atof(items[1].c_str());
	  }
	  else if (items[0] == "y") {
	    y = atof(items[1].c_str());
	  }
	  else if (items[0] == "utc") {
	    timestamp = atof(items[1].c_str());
	  }
	  else if (items[0] == "vname") {
	    vehicleID = items[1];
	  }
	}
      }
    }

public:
    double x, y, temp, timestamp;
    std::string vehicleID;

};

#endif
