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
#include "BHV_GoSideways.h"
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
    deg = deg-180.0;
  }
  while (deg < -180.0) {
    deg = deg+180.0;
  }
  return deg;
}

//-----------------------------------------------------------
// Procedure: Constructor

BHV_GoSideways::BHV_GoSideways(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  addInfoVars("NAV_HEADING, HOT_HEADING");

  IvPBehavior::setParam("name", "GoSideways");
  m_domain = subDomain(m_domain, "course");

  // Initialization
  _gen_rand_alpha_setpoint = false;
  _last_error = 0;
  _randomize_speed = false;
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_GoSideways::setParam(string param, string val) 
{
  param = tolower(param);
  double double_val = atof(val.c_str());

  if ((param == "gen_rand_alpha_setpoint")) {
    if (val == "true") {
      _gen_rand_alpha_setpoint = true;
    }
    else {
      _gen_rand_alpha_setpoint = false;
    }
    return(true);
  }
  if((param == "wavefollow_update_var")) {
    _wavefollow_update_var = val;
    return(true);
  }
  if ((param == "randomize_speed")) {
    if (val == "true") {
      _randomize_speed = true;
    }
    else {
      _randomize_speed = false;
    }
    return(true);
  }

  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_GoSideways::onIdleState() 
{ 

}

//-----------------------------------------------------------
// Procedure: postViewPoint

void BHV_GoSideways::postViewPoint(bool viewable) 
{
 
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_GoSideways::onRunState() 
{
  // Grab relevant variables from Helm
  bool ok4, ok5;;
  double heading = getBufferDoubleVal("NAV_HEADING", ok4);
  double hot_heading = getBufferDoubleVal("HOT_HEADING", ok5);

  if (!ok4 || !ok5) {
    postWMessage("Error, missing info");
    return(0);
  }

  IvPFunction *ipf = 0;

  double new_heading;
  int new_error = 0;
  // bias heading to right angle to hot_heading direction
  if (wrapDegError(heading - hot_heading) > 0) {
    new_heading = hot_heading + 90;
    new_error = 1;
  }
  else {
    new_heading = hot_heading - 90;
    new_error = -1;
  }

  if (_gen_rand_alpha_setpoint && new_error != _last_error) {
    // Post a alpha_setpoint update
    stringstream s;
    s << "alpha_setpoint=" << ((double)rand()/(double)RAND_MAX - 0.5)*1.5;
    postMessage(_wavefollow_update_var,s.str());
  }
  if (_randomize_speed && new_error != _last_error) {
    // Post a alpha_setpoint update
    stringstream s;
    s << "speed=" << ((double)rand()/(double)RAND_MAX)*2.0 + 1.0;
    postMessage(_wavefollow_update_var,s.str());
  }
  _last_error = new_error;

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

  IvPFunction *head_ipf = head_zaic.extractIvPFunction();

  ipf = head_ipf;

  if (ipf) {
    ipf -> setPWT(m_priority_wt);
  }
  
  return(ipf);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_GoSideways::buildFunctionWithZAIC() 
{
  
  IvPFunction *ivp_function;

  return(ivp_function);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithReflector

IvPFunction *BHV_GoSideways::buildFunctionWithReflector() 
{
  IvPFunction *ivp_function;

  return(ivp_function);
}
