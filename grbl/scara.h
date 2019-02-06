/*
  system.h - Header for system level commands and real-time processes
  Part of Grbl

  Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * scara.h - SCARA-specific functions
 */

#ifndef SCARA_h
#define SCARA_h

#include "grbl.h"

#ifdef SCARA
#define IS_SCARA true
#endif


// Length of inner and outer support arms. Measure arm lengths precisely.
#define SCARA_LINKAGE_1 200.0f //mm
#define SCARA_LINKAGE_2 200.0f //mm

// SCARA tower offset (position of Tower relative to bed zero position)
// This needs to be reasonably accurate as it defines the printbed position in the SCARA space.
#define SCARA_OFFSET_X -129 //mm
#define SCARA_OFFSET_Y 128//mm	

#define MANUAL_X_HOME_POS -48.27f
#define MANUAL_Y_HOME_POS 90.67f
#define MANUAL_Z_HOME_POS 0.0f

#define RADIANS(d) ((d)*(float)M_PI/180.0f)
#define DEGREES(r) ((r)*180.0f/(float)M_PI)

#define sq(x) x*x

extern float ManualHomePos[3];
void inverse_kinematics(float const *cartesian, float *f_scara);
void forward_kinematics_SCARA(float const *f_scara, float *cartesian);
void scara_report_positions(void) ;

#endif
