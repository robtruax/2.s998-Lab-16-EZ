/************************************************************/
/*    NAME: robtruax                                              */
/*    ORGN: MIT                                             */
/*    FILE: CollectTempStats.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef CollectTempStats_HEADER
#define CollectTempStats_HEADER

#include "MOOSLib.h"
#include "MBUtils.h"
#include <math.h>
#include <Eigen/Dense>

class Measurement {
public:
    double x, y, temp, timestamp;
    int vehicleID;

    Measurement() { };

    Measurement(double _x, double _y, double _temp, double _timestamp, int _vehicleID) {
	x = _x;
	y = _y;
	temp = _temp;
	timestamp = _timestamp;
	vehicleID = _vehicleID;
    };

    ~Measurement() { };
};

class CollectTempStats : public CMOOSApp
{
 public:
   CollectTempStats();
   ~CollectTempStats();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();

   void clearMeasurements() {
     _meas.clear();
   };

   double getHotHeading();
   double getHotHeading2();
   Measurement getMaxTemp();
   Measurement getMinTemp();
   void publishDecisionLine(double,double,double);

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;

   bool _last_underway_state;

   std::vector<Measurement> _meas;
};

#endif 
