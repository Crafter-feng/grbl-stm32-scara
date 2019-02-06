/*
  system.c - Handles system level commands and real-time processes
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

#include "grbl.h"

float const L1 = SCARA_LINKAGE_1,
            L2 = SCARA_LINKAGE_2,
            L1_2 = sq(SCARA_LINKAGE_1),
            L2_2 = sq(SCARA_LINKAGE_2);

u8 angle_mode=false;

float ManualHomePos[3]={MANUAL_X_HOME_POS,MANUAL_Y_HOME_POS,MANUAL_Z_HOME_POS}; 

void forward_kinematics_SCARA(float const *f_scara, float *cartesian)
{
    float x_sin, x_cos, y_sin, y_cos;

    x_sin = sin(RADIANS(f_scara[X_AXIS])) * L1;
    x_cos = cos(RADIANS(f_scara[X_AXIS])) * L1;
    y_sin = sin(RADIANS(f_scara[Y_AXIS])) * L2;//当Y电机控制小臂相对于世界坐标系旋转角度时使用此公式
    y_cos = cos(RADIANS(f_scara[Y_AXIS])) * L2;//当Y电机控制小臂相对于世界坐标系旋转角度时使用此公式

    cartesian[X_AXIS] = -x_cos - y_cos - SCARA_OFFSET_X;  //求得用户坐标系下X值
    cartesian[Y_AXIS] = x_sin + y_sin - SCARA_OFFSET_Y;  //求得用户坐标系下Y值
    cartesian[Z_AXIS] = (float)f_scara[Z_AXIS];

}

void inverse_kinematics(float const *cartesian, float *f_scara)
{
    float SCARA_pos[2];

    static float SCARA_C2, SCARA_S2, SCARA_K1, SCARA_K2, SCARA_theta, SCARA_psi;

    SCARA_pos[X_AXIS] = -cartesian[X_AXIS] - SCARA_OFFSET_X;  //求世界坐标系下X值并取反
    SCARA_pos[Y_AXIS] = cartesian[Y_AXIS] + SCARA_OFFSET_Y;  //求世界坐标系下Y值

    SCARA_C2 =   ( sq(SCARA_pos[X_AXIS]) + sq(SCARA_pos[Y_AXIS]) - L1_2 - L2_2 ) /(2*L1*L2);
    SCARA_S2 = sqrtf( 1 - sq(SCARA_C2) );

    SCARA_K1 = SCARA_LINKAGE_1 + SCARA_LINKAGE_2 * SCARA_C2;
    SCARA_K2 = SCARA_LINKAGE_2 * SCARA_S2;

    SCARA_theta = ( atan2f(SCARA_K1, SCARA_K2)-atan2f(SCARA_pos[X_AXIS],SCARA_pos[Y_AXIS]) ) ;//大臂旋转角度，即主臂与-X轴夹角
    SCARA_psi   =   atan2f(SCARA_S2,SCARA_C2) + SCARA_theta;//小臂旋转角度，当Y电机控制小臂相对于世界坐标系旋转角度时使用此公式
		
		if(!angle_mode)
		{
				f_scara[X_AXIS] = DEGREES(SCARA_theta); //大臂旋转角度转换为弧度
				f_scara[Y_AXIS] = DEGREES(SCARA_psi);   //小臂旋转角度转换为弧度
				f_scara[Z_AXIS] = cartesian[Z_AXIS];
		}
		else
		{
				f_scara[X_AXIS] = cartesian[X_AXIS]; 
				f_scara[Y_AXIS] = cartesian[Y_AXIS]; 
				f_scara[Z_AXIS] = cartesian[Z_AXIS];		
		
		}
}

void scara_report_positions() 
{
		u8 idx;
		static float position_scara[3];
		
    for (idx=0; idx<N_AXIS; idx++)
    {
        position_scara[idx] = system_convert_axis_steps_to_mpos(sys_position, idx);
    }
		printPgmString(PSTR("SCARA Theta:"));
    printFloat(position_scara[X_AXIS],2);
		printPgmString(PSTR("   Psi+Theta:"));
    printFloat(position_scara[Y_AXIS],2);
		printPgmString(PSTR("\n"));		

}

