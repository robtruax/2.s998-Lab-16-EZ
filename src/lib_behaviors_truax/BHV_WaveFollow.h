/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_ZigLeg.h                                */
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
 
#ifndef BHV_WAVEFOLLOW_HEADER
#define BHV_WAVEFOLLOW_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYPoint.h"
#include "XYRangePulse.h"

class BHV_WaveFollow : public IvPBehavior {
public:
  BHV_WaveFollow(IvPDomain);
  ~BHV_WaveFollow() {};
  
  bool         setParam(std::string, std::string);
  void         onIdleState();
  IvPFunction* onRunState();

protected:
  void         postViewPoint(bool viewable=true);
  IvPFunction* buildFunctionWithZAIC();
  IvPFunction* buildFunctionWithReflector();

protected: // Configuration parameters
  double _Pd;
  double _speed;
  double _alpha_setpoint;

  bool _bangbang;
  double _railmag;
  double _min_weight;

protected: // State variables

};

#ifdef WIN32
	// Windows needs to explicitly specify functions to export from a dll
   #define IVP_EXPORT_FUNCTION __declspec(dllexport) 
#else
   #define IVP_EXPORT_FUNCTION
#endif

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_WaveFollow(domain);}
}
#endif









