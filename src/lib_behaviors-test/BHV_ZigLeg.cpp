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
#include "BHV_ZigLeg.h"
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

BHV_ZigLeg::BHV_ZigLeg(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  addInfoVars("NAV_X, NAV_Y, NAV_HEADING, WPT_INDEX");

  IvPBehavior::setParam("name", "ZigLeg");
  m_domain = subDomain(m_domain, "course");

  // Initialization
  _last_idx = 0;
  _hit_time = getBufferCurrTime();
  _armed = false;
  _pulse_range = 20;
  _pulse_duration = 4;
  _zig_duration = 10;
  _zig_angle = 45;
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_ZigLeg::setParam(string param, string val) 
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
  else if ((param == "zig_duration") && isNumber(val)) {
    _zig_duration = double_val;
    return(true);
  }
  else if ((param == "zig_angle") && isNumber(val)) {
    _zig_angle = double_val;
    return(true);
  }

  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_ZigLeg::onIdleState() 
{ 

}

//-----------------------------------------------------------
// Procedure: postViewPoint

void BHV_ZigLeg::postViewPoint(bool viewable) 
{
 
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_ZigLeg::onRunState() 
{
  // Grab relevant variables from Helm
  bool ok1, ok2, ok3, ok4;
  double x = getBufferDoubleVal("NAV_X", ok1);
  double y = getBufferDoubleVal("NAV_Y", ok2);
  double idx = getBufferDoubleVal("WPT_INDEX", ok3);
  double heading = getBufferDoubleVal("NAV_HEADING", ok4);

  if (!ok1 || !ok2 || !ok3 || !ok4) {
    postWMessage("Error, missing info");
    return(0);
  }

  // check if index has changed
  if (idx != _last_idx) {
    // start the clock
    _hit_time = getBufferCurrTime();
    _armed = true;
    _zig_armed = true;
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
  
  if (getBufferCurrTime() - _hit_time > 5.0 &&
      getBufferCurrTime() - _hit_time < _zig_duration + 5.0) {
    //   cout << "Changing Heading" << endl;
    if (_zig_armed) {
      _last_heading = heading;
      _zig_armed = false;
    }
    double new_heading = _last_heading + _zig_angle;
    if (new_heading > 360) {
      new_heading = new_heading - 360;
    }
    ZAIC_PEAK head_zaic(m_domain, "course");
    head_zaic.setSummit(new_heading);
    head_zaic.setPeakWidth(0);
    head_zaic.setBaseWidth(20.0);
    head_zaic.setSummitDelta(0);
    head_zaic.setValueWrap(true);

    if ( head_zaic.stateOK() == false ) {
      string warnings = "Course Zaic problems " +  head_zaic.getWarnings();
      postWMessage(warnings);
      return(0);
    }

    ipf = head_zaic.extractIvPFunction();
    if (ipf) {
      ipf -> setPWT(m_priority_wt);
    }
  }
  
  return(ipf);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_ZigLeg::buildFunctionWithZAIC() 
{
  
  IvPFunction *ivp_function;

  return(ivp_function);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithReflector

IvPFunction *BHV_ZigLeg::buildFunctionWithReflector() 
{
  IvPFunction *ivp_function;

  return(ivp_function);
}
