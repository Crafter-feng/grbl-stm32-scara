/*
  defaults.h - defaults settings configuration file
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

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

/* The defaults.h file serves as a central default settings selector for different machine
   types, from DIY CNC mills to CNC conversions of off-the-shelf machines. The settings
   files listed here are supplied by users, so your results may vary. However, this should
   give you a good starting point as you get to know your machine and tweak the settings for
   your nefarious needs.
   NOTE: Ensure one and only one of these DEFAULTS_XXX values is defined in config.h */

#ifndef defaults_h
#define defaults_h

// Grbl generic default settings. Should work across different machines.
#define DEFAULT_X_STEPS_PER_MM 97.777778f
#define DEFAULT_Y_STEPS_PER_MM 97.777778f
#define DEFAULT_Z_STEPS_PER_MM 97.777778f
#define DEFAULT_X_MAX_RATE 2000.0f // mm/min
#define DEFAULT_Y_MAX_RATE 2000.0f // mm/min
#define DEFAULT_Z_MAX_RATE 2000.0f // mm/min
#define DEFAULT_X_ACCELERATION (50.0f*60*60) // 10*60*60 mm/min^2 = 10 mm/sec^2
#define DEFAULT_Y_ACCELERATION (50.0f*60*60) // 10*60*60 mm/min^2 = 10 mm/sec^2
#define DEFAULT_Z_ACCELERATION (50.0f*60*60) // 10*60*60 mm/min^2 = 10 mm/sec^2
#define DEFAULT_X_MAX_TRAVEL 200.0f // mm NOTE: Must be a positive value.
#define DEFAULT_Y_MAX_TRAVEL 200.0f // mm NOTE: Must be a positive value.
#define DEFAULT_Z_MAX_TRAVEL 200.0f // mm NOTE: Must be a positive value.
#define DEFAULT_SPINDLE_RPM_MAX 2000.0f // rpm
#define DEFAULT_SPINDLE_RPM_MIN 1.0f // rpm
#define DEFAULT_PEN_TOP  115
#define DEFAULT_PEN_BUTTON  138
#define DEFAULT_STEP_PULSE_MICROSECONDS 10
#define DEFAULT_STEPPING_INVERT_MASK 0
#define DEFAULT_DIRECTION_INVERT_MASK 2
#define DEFAULT_STEPPER_IDLE_LOCK_TIME 255 // msec (0-254, 255 keeps steppers enabled)
#define DEFAULT_STATUS_REPORT_MASK 1 // MPos enabled
#define DEFAULT_JUNCTION_DEVIATION 0.01f // mm
#define DEFAULT_ARC_TOLERANCE 0.002f // mm
#define DEFAULT_REPORT_INCHES 0 // false
#define DEFAULT_INVERT_ST_ENABLE 0 // false
#define DEFAULT_INVERT_LIMIT_PINS 0 // false
#define DEFAULT_SOFT_LIMIT_ENABLE 0 // false
#define DEFAULT_HARD_LIMIT_ENABLE 1  // false
#define DEFAULT_INVERT_PROBE_PIN 0 // false
#define DEFAULT_LASER_MODE 0 // false
#define DEFAULT_HOMING_ENABLE 1  // false
#define DEFAULT_HOMING_DIR_MASK 3 // move positive dir
#define DEFAULT_HOMING_FEED_RATE 500.0f // mm/min
#define DEFAULT_HOMING_SEEK_RATE 1000.0f // mm/min
#define DEFAULT_HOMING_DEBOUNCE_DELAY 200 // msec (0-65k)
#define DEFAULT_HOMING_PULLOFF 5.0f // mm

#endif

