/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_ZigLeg.cpp                               */
/*    DATE: July 1st 2008  (For purposes of simple illustration) */
/*                                                               */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation; either version  */
/* 2 of the License, or (at your option) any later version.      */
/*                                                               */
/* This program is distributed in the hope that it will be       */
/* useful, but WITHOUT ANY WARRANTY; without even the implied    */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the GNU General Public License for more details. */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with this program; if not, write to the Free    */
/* Software Foundation, Inc., 59 Temple Place - Suite 330,       */
/* Boston, MA 02111-1307, USA.                                   */
/*****************************************************************/

#include <cstdlib>
#include <math.h>
#include "BHV_WaveFollow.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include "AOF_SimpleWaypoint.h"

using namespace std;

double wrapDeg(double deg) {
  while (deg > 360.0) {
    deg = deg-360.0;
  }
  while (deg < 0) {
    deg = deg+360.0;
  }
  return deg;
}

double wrapDegError(double deg) {
  while (deg > 180.0) {
    deg = deg-360.0;
  }
  while (deg < -180.0) {
    deg = deg+360.0;
  }
  return deg;
}

//-----------------------------------------------------------
// Procedure: Constructor

BHV_WaveFollow::BHV_WaveFollow(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  addInfoVars("NAV_HEADING, UCTD_MSMNT_REPORT, HOT_HEADING, MIN_TEMP, MAX_TEMP");

  IvPBehavior::setParam("name", "WaveFollow");
  m_domain = subDomain(m_domain, "course,speed");

  // Initialization
  _Pd = 0.5;
  _speed = 1.0;
  _alpha_setpoint = 0;
  _bangbang = false;
  _railmag = 0.0;
  _min_weight = 25;
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_WaveFollow::setParam(string param, string val) 
{
  param = tolower(param);
  double double_val = atof(val.c_str());

  if ((param == "pd") && isNumber(val)) {
    _Pd = double_val;
    return(true);
  }
  else if ((param == "speed") && isNumber(val)) {
    _speed = double_val;
    return(true);
  }
  else if ((param == "alpha_setpoint") && isNumber(val)) {
    _alpha_setpoint = double_val;
    return(true);
  }
  else if ((param == "bangbang" && val == "true") ) {
    _bangbang = true;
    return(true);
  }
  else if ( (param == "railmag") && isNumber(val) ) {
    _railmag = double_val;
    _alpha_setpoint = _railmag;
    return(true);
  }

  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_WaveFollow::onIdleState() 
{ 

}

//-----------------------------------------------------------
// Procedure: postViewPoint

void BHV_WaveFollow::postViewPoint(bool viewable) 
{
 
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_WaveFollow::onRunState() 
{
  // Grab relevant variables from Helm
  bool ok1, ok2, ok3, ok4, ok5;
  double max_T = getBufferDoubleVal("MAX_TEMP", ok1);
  double min_T = getBufferDoubleVal("MIN_TEMP", ok2);
  string sensor_message = getBufferStringVal("UCTD_MSMNT_REPORT", ok3);
  double heading = getBufferDoubleVal("NAV_HEADING", ok4);
  double hot_heading = getBufferDoubleVal("HOT_HEADING", ok5);

  if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5) {
    postWMessage("Error, missing info");
    return(0);
  }

  double temp = -1;
  // Process UCTD Message
  vector<string> inmsg = parseString(sensor_message, ",");
  for (int i = 0; i < inmsg.size(); i++) {
    vector<string> items = parseString(inmsg[i],"=");
    if (items.size() == 2) {
      if (items[0] == "temp") {
	temp = atof(items[1].c_str());
      }
    }
  }

  IvPFunction *ipf = 0;

  if (temp == -1) {
    // uninitialized.  Return
    return(ipf);
  }

  // pushing strength calculation.  rough temp distance to front
  double alpha = ( (max_T - temp) / (max_T - min_T) * 2 ) - 1;
  postMessage("ALPHA_SETPOINT",_alpha_setpoint);
  // ranges from 1 to -1 with 0 on the front

  // do bang bang controller
  if (_bangbang) {
    if (_alpha_setpoint > 0 && alpha >= _alpha_setpoint) {
      //bang
      _alpha_setpoint = -_alpha_setpoint;
    }
    else if (_alpha_setpoint < 0 && alpha <= _alpha_setpoint) {
      _alpha_setpoint = -_alpha_setpoint;
    }
  }

  // get heading errror
  double error;
  if (alpha > _alpha_setpoint) {
    error = wrapDegError(hot_heading - heading);
  }
  else {
    error = wrapDegError((hot_heading-180) - heading);
  }

  // Error hack for better performance
  if (error < -15) {
    error = error + 15;
  }
  else if (error > 15) {
    error = error - 15;
  }
  else {
    error = 0;
  }

  double new_heading = heading + _Pd*error;
  new_heading = wrapDeg(new_heading);

  ZAIC_PEAK head_zaic(m_domain, "course");
  head_zaic.setSummit(new_heading);
  head_zaic.setPeakWidth(0);
  head_zaic.setBaseWidth(180.0);
  head_zaic.setSummitDelta(0);
  head_zaic.setValueWrap(true);

  if ( head_zaic.stateOK() == false ) {
    string warnings = "Course Zaic problems " +  head_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  double d_speed = _speed;
  ZAIC_PEAK spd_zaic(m_domain,"speed");
  spd_zaic.setSummit(d_speed);
  spd_zaic.setPeakWidth(0.5);
  spd_zaic.setBaseWidth(1.0);
  spd_zaic.setSummitDelta(0.8);  
  if(spd_zaic.stateOK() == false) {
    string warnings = "Speed ZAIC problems " + spd_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  IvPFunction *head_ipf = head_zaic.extractIvPFunction();
  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction();

  OF_Coupler coupler;
  ipf = coupler.couple(head_ipf, spd_ipf, 50, 50);

  if (ipf) {
    ipf -> setPWT(m_priority_wt*fabs(alpha-_alpha_setpoint) + _min_weight);
  }
  
  return(ipf);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_WaveFollow::buildFunctionWithZAIC() 
{
  
  IvPFunction *ivp_function;

  return(ivp_function);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithReflector

IvPFunction *BHV_WaveFollow::buildFunctionWithReflector() 
{
  IvPFunction *ivp_function;

  return(ivp_function);
}
