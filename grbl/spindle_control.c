/*
  spindle_control.c - spindle control methods
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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


#ifdef VARIABLE_SPINDLE
  static float pwm_gradient; // Precalulated value to speed up rpm to PWM conversions.
#endif


void spindle_init()
{
#ifdef VARIABLE_SPINDLE
  pwm_gradient = SPINDLE_PWM_RANGE / (settings.rpm_max - settings.rpm_min);
#endif

#if defined (STM32F103C8)
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_SPINDLE_ENABLE_PORT, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
#ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
  GPIO_InitStructure.GPIO_Pin = 1 << SPINDLE_ENABLE_BIT;
#else
  GPIO_InitStructure.GPIO_Pin = 1 << SPINDLE_DIRECTION_BIT;
#endif
  GPIO_Init(SPINDLE_ENABLE_PORT, &GPIO_InitStructure);


#ifdef VARIABLE_SPINDLE
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseInitTypeDef timerInitStructure;
    TIM_OCInitTypeDef outputChannelInit = { 0 };
    TIM_TimeBaseStructInit(&timerInitStructure);

    timerInitStructure.TIM_Prescaler = 719;//F_CPU / 1000000 - 1; // 1000
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1999;//SPINDLE_PWM_MAX_VALUE - 1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &timerInitStructure);

    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = 0;     // initi speed is 0
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM1, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
    TIM_Cmd(TIM1, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_SPINDLE_PWM_PORT, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = 1 << SPINDLE_PWM_BIT;
    GPIO_Init(SPINDLE_PWM_PORT, &GPIO_InitStructure);


#endif
#endif

  spindle_stop();
}


uint8_t spindle_get_state()
{
  uint8_t pin = 0;
	#ifdef VARIABLE_SPINDLE
    #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN

#if defined (STM32F103C8)
  pin = GPIO_ReadInputData(SPINDLE_ENABLE_PORT);
#endif
		  // No spindle direction output pin.
			#ifdef INVERT_SPINDLE_ENABLE_PIN
			  if (bit_isfalse(pin,(1<<SPINDLE_ENABLE_BIT))) { return(SPINDLE_STATE_CW); }
	    #else
	 			if (bit_istrue(pin,(1<<SPINDLE_ENABLE_BIT))) { return(SPINDLE_STATE_CW); }
	    #endif
    #else

#if defined (STM32F103C8)
      pin = GPIO_ReadInputData(SPINDLE_DIRECTION_PORT);
#endif
     {
        if (pin & (1<<SPINDLE_DIRECTION_BIT)) { return(SPINDLE_STATE_CCW); }
        else { return(SPINDLE_STATE_CW); }
      }
    #endif
	#else

#if defined (STM32F103C8)
  pin = GPIO_ReadInputData(SPINDLE_ENABLE_PORT);
#endif
		#ifdef INVERT_SPINDLE_ENABLE_PIN
		  if (bit_isfalse(pin,(1<<SPINDLE_ENABLE_BIT))) {
		#else
		  if (bit_istrue(pin,(1<<SPINDLE_ENABLE_BIT))) {
		#endif
		  if (pin & (1 << SPINDLE_DIRECTION_BIT)) { return(SPINDLE_STATE_CCW); }
		  else { return(SPINDLE_STATE_CW); }
		}
	#endif
	return(SPINDLE_STATE_DISABLE);
}


// Disables the spindle and sets PWM output to zero when PWM variable spindle speed is enabled.
// Called by various main program and ISR routines. Keep routine small, fast, and efficient.
// Called by spindle_init(), spindle_set_speed(), spindle_set_state(), and mc_reset().
void spindle_stop()
{

#ifdef SERVO_PEN
    TIM1->CCR1 = (uint16_t)settings.pen_top;
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
#else

#ifdef VARIABLE_SPINDLE
#if defined (STM32F103C8)
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
#endif

    #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        SetSpindleEnablebit();
      #else
        ResetSpindleEnablebit();
      #endif
    #endif
#else
    #ifdef INVERT_SPINDLE_ENABLE_PIN
      SetSpindleEnablebit();
    #else
      ResetSpindleEnablebit();
    #endif
#endif
#endif
}


#ifdef VARIABLE_SPINDLE
  // Sets spindle speed PWM output and enable pin, if configured. Called by spindle_set_state()
  // and stepper ISR. Keep routine small and efficient.
  void spindle_set_speed(SPINDLE_PWM_TYPE pwm_value)
  {
#ifdef SERVO_PEN
		if(pwm_value == 0)
			TIM1->CCR1 = (uint16_t)settings.pen_top;
		else
			TIM1->CCR1 = (uint16_t)settings.pen_button;
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
#else

#if defined (STM32F103C8)
		TIM1->CCR1 = pwm_value;
#endif
		#ifdef SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED
     if (pwm_value == SPINDLE_PWM_OFF_VALUE) {
        spindle_stop();
      } else {
				#ifdef AVRTARGET
					SPINDLE_TCCRA_REGISTER |= (1<<SPINDLE_COMB_BIT); // Ensure PWM output is enabled.
				#endif
				#if defined (STM32F103C8)
						TIM_CtrlPWMOutputs(TIM1, ENABLE);
				#endif
				#ifdef INVERT_SPINDLE_ENABLE_PIN
					ResetSpindleEnablebit();
				#else
					SetSpindleEnablebit();
				#endif
		 }
		#else
			if (pwm_value == SPINDLE_PWM_OFF_VALUE) {
			#if defined (STM32F103C8)
				spindle_stop();
			#endif
			} else {
			#if defined (STM32F103C8)
      TIM_CtrlPWMOutputs(TIM1, ENABLE);
			#endif
			}
		#endif
#endif
	}


  #ifdef ENABLE_PIECEWISE_LINEAR_SPINDLE
	// Called by spindle_set_state() and step segment generator. Keep routine small and efficient.
	SPINDLE_PWM_TYPE spindle_compute_pwm_value(float rpm) // 328p PWM register is 8-bit.
	{
		SPINDLE_PWM_TYPE pwm_value;
		rpm *= (0.010*sys.spindle_speed_ovr); // Scale by spindle speed override value.
																					// Calculate PWM register value based on rpm max/min settings and programmed rpm.
		if ((settings.rpm_min >= settings.rpm_max) || (rpm >= RPM_MAX)) {
			rpm = RPM_MAX;
			pwm_value = SPINDLE_PWM_MAX_VALUE;
		}
		else if (rpm <= RPM_MIN) {
			if (rpm == 0.0) { // S0 disables spindle
				pwm_value = SPINDLE_PWM_OFF_VALUE;
			}
			else {
				rpm = RPM_MIN;
				pwm_value = SPINDLE_PWM_MIN_VALUE;
			}
		}
		else {
			// Compute intermediate PWM value with linear spindle speed model via piecewise linear fit model.
#if (N_PIECES > 3)
			if (rpm > RPM_POINT34) {
				pwm_value = floorf(RPM_LINE_A4*rpm - RPM_LINE_B4);
			}
			else
#endif
#if (N_PIECES > 2)
				if (rpm > RPM_POINT23) {
					pwm_value = floorf(RPM_LINE_A3*rpm - RPM_LINE_B3);
				}
				else
#endif
#if (N_PIECES > 1)
					if (rpm > RPM_POINT12) {
						pwm_value = floorf(RPM_LINE_A2*rpm - RPM_LINE_B2);
					}
					else
#endif
					{
						pwm_value = floorf(RPM_LINE_A1*rpm - RPM_LINE_B1);
					}
		}
		sys.spindle_speed = rpm;
		return(pwm_value);
	}
  #else
	// Called by spindle_set_state() and step segment generator. Keep routine small and efficient.
	SPINDLE_PWM_TYPE spindle_compute_pwm_value(float rpm) // 328p PWM register is 8-bit.
	{
		SPINDLE_PWM_TYPE pwm_value;
		rpm *= (0.010f*sys.spindle_speed_ovr); // Scale by spindle speed override value.
																					 // Calculate PWM register value based on rpm max/min settings and programmed rpm.
		if ((settings.rpm_min >= settings.rpm_max) || (rpm >= settings.rpm_max)) {
			// No PWM range possible. Set simple on/off spindle control pin state.
			sys.spindle_speed = settings.rpm_max;
			pwm_value = SPINDLE_PWM_MAX_VALUE;
		}
		else if (rpm <= settings.rpm_min) {
			if (rpm == 0.0f) { // S0 disables spindle
				sys.spindle_speed = 0.0f;
				pwm_value = SPINDLE_PWM_OFF_VALUE;
			}
			else { // Set minimum PWM output
				sys.spindle_speed = settings.rpm_min;
				pwm_value = SPINDLE_PWM_MIN_VALUE;
			}
		}
		else {
			// Compute intermediate PWM value with linear spindle speed model.
			// NOTE: A nonlinear model could be installed here, if required, but keep it VERY light-weight.
			sys.spindle_speed = rpm;
			pwm_value = (SPINDLE_PWM_TYPE)floorf((rpm - settings.rpm_min)*pwm_gradient) + SPINDLE_PWM_MIN_VALUE;
		}
		return(pwm_value);
	}
  #endif
#endif


// Immediately sets spindle running state with direction and spindle rpm via PWM, if enabled.
// Called by g-code parser spindle_sync(), parking retract and restore, g-code program end,
// sleep, and spindle stop override.
#ifdef VARIABLE_SPINDLE
  void spindle_set_state(uint8_t state, float rpm)
#else
  void _spindle_set_state(uint8_t state)
#endif
{
  if (sys.abort) { return; } // Block during abort.
  if (state == SPINDLE_DISABLE) { // Halt or set spindle direction and rpm.

    #ifdef VARIABLE_SPINDLE
      sys.spindle_speed = 0.0f;
    #endif
    spindle_stop();

  } else {
    #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
      if (state == SPINDLE_ENABLE_CW) {
        ResetSpindleDirectionBit();
	  }
	  else {
      SetSpindleDirectionBit();
      }
    #endif

    #ifdef VARIABLE_SPINDLE
      // NOTE: Assumes all calls to this function is when Grbl is not moving or must remain off.
      if (settings.flags & BITFLAG_LASER_MODE) {
        if (state == SPINDLE_ENABLE_CCW) { rpm = 0.0f; } // TODO: May need to be rpm_min*(100/MAX_SPINDLE_SPEED_OVERRIDE);
      }
    spindle_set_speed(spindle_compute_pwm_value(rpm));
		#endif
    #if (defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) && \
        !defined(SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED)) || !defined(VARIABLE_SPINDLE)
      // NOTE: Without variable spindle, the enable bit should just turn on or off, regardless
      // if the spindle speed value is zero, as its ignored anyhow.
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        ResetSpindleEnablebit();
      #else
        SetSpindleEnablebit();
      #endif
    #endif
  }

  sys.report_ovr_counter = 0; // Set to report change immediately
}


// G-code parser entry-point for setting spindle state. Forces a planner buffer sync and bails
// if an abort or check-mode is active.
#ifdef VARIABLE_SPINDLE
  void spindle_sync(uint8_t state, float rpm)
  {
    if (sys.state == STATE_CHECK_MODE) { return; }
    protocol_buffer_synchronize(); // Empty planner buffer to ensure spindle is set when programmed.
    spindle_set_state(state,rpm);
  }
#else
  void _spindle_sync(uint8_t state)
  {
    if (sys.state == STATE_CHECK_MODE) { return; }
    protocol_buffer_synchronize(); // Empty planner buffer to ensure spindle is set when programmed.
    _spindle_set_state(state);
  }
#endif
