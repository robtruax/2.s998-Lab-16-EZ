/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_Pulse.cpp                               */
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
#include "BHV_Pulse.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include "AOF_SimpleWaypoint.h"

using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_Pulse::BHV_Pulse(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  addInfoVars("NAV_X, NAV_Y, WPT_INDEX");

  // Initialization
  _last_idx = 0;
  _hit_time = getBufferCurrTime();
  _armed = false;
  _pulse_range = 20;
  _pulse_duration = 4;
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_Pulse::setParam(string param, string val) 
{
  param = tolower(param);
  double double_val = atof(val.c_str());

  if ((param == "pulse_range") && isNumber(val)) {
    _pulse_range = double_val;
    return(true);
  }
  else if ((param == "pulse_duration") && isNumber(val)) {
    _pulse_duration = double_val;
    return(true);
  }

  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_Pulse::onIdleState() 
{ 

}

//-----------------------------------------------------------
// Procedure: postViewPoint

void BHV_Pulse::postViewPoint(bool viewable) 
{
 
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_Pulse::onRunState() 
{
  // Grab relevant variables from Helm
  bool ok1, ok2, ok3;
  double x = getBufferDoubleVal("NAV_X", ok1);
  double y = getBufferDoubleVal("NAV_Y", ok2);
  double idx = getBufferDoubleVal("WPT_INDEX", ok3);

  if (!ok1 || !ok2 || !ok3) {
    postWMessage("Error, missing info");
    return(0);
  }

  // check if index has changed
  if (idx != _last_idx) {
    // start the clock
    _hit_time = getBufferCurrTime();
    _armed = true;
    _last_idx = idx;
  }

  // check clock
  if (_armed && getBufferCurrTime() - _hit_time > 5.0) {
    // BOOM!
    XYRangePulse pulse;
    pulse.set_x(x);
    pulse.set_y(y);
    pulse.set_label("bhv_pulse");
    pulse.set_rad(_pulse_range);
    pulse.set_duration(_pulse_duration);
    pulse.set_time(getBufferCurrTime());
    pulse.set_color("edge", "yellow");
    pulse.set_color("fill", "yellow");
    string spec = pulse.get_spec();
    postMessage("VIEW_RANGE_PULSE",spec);

    _armed = false;
  }

  IvPFunction *ipf = 0;
  
  return(ipf);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_Pulse::buildFunctionWithZAIC() 
{
  
  IvPFunction *ivp_function;

  return(ivp_function);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithReflector

IvPFunction *BHV_Pulse::buildFunctionWithReflector() 
{
  IvPFunction *ivp_function;

  return(ivp_function);
}
