/*
  cpu_map.h - CPU and pin mapping configuration file
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

/* The cpu_map.h files serve as a central pin mapping selection file for different
   processor types or alternative pin layouts. This version of Grbl officially supports
   only the Arduino Mega328p. */
#ifndef cpu_map_h
#define cpu_map_h


#ifdef CPU_MAP_ATMEGA328P // (Arduino Uno) Officially supported by Grbl.

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEP_DDR        DDRD
  #define STEP_PORT       PORTD
  #define X_STEP_BIT      2  // Uno Digital Pin 2
  #define Y_STEP_BIT      3  // Uno Digital Pin 3
  #define Z_STEP_BIT      4  // Uno Digital Pin 4
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     DDRD
  #define DIRECTION_PORT    PORTD
  #define X_DIRECTION_BIT   5  // Uno Digital Pin 5
  #define Y_DIRECTION_BIT   6  // Uno Digital Pin 6
  #define Z_DIRECTION_BIT   7  // Uno Digital Pin 7
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    DDRB
  #define STEPPERS_DISABLE_PORT   PORTB
  #define STEPPERS_DISABLE_BIT    0  // Uno Digital Pin 8
  #define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)
  #define SetStepperDisableBit() STEPPERS_DISABLE_PORT |= (1 << STEPPERS_DISABLE_BIT)
  #define ResetStepperDisableBit() STEPPERS_DISABLE_PORT &= ~(1<<STEPPERS_DISABLE_BIT)
  #define EnableStepperDisabeBit()  STEPPERS_DISABLE_DDR |= 1<<STEPPERS_DISABLE_BIT

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR        DDRB
  #define LIMIT_PIN        PINB
  #define LIMIT_PORT       PORTB
  #define X_LIMIT_BIT      1  // Uno Digital Pin 9
  #define Y_LIMIT_BIT      2  // Uno Digital Pin 10
  #ifdef VARIABLE_SPINDLE // Z Limit pin and spindle enabled swapped to access hardware PWM on Pin 11.
    #define Z_LIMIT_BIT	   4 // Uno Digital Pin 12
  #else
    #define Z_LIMIT_BIT    3  // Uno Digital Pin 11
  #endif
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits
  #define LIMIT_INT        PCIE0  // Pin change interrupt enable pin
  #define LIMIT_INT_vect   PCINT0_vect
  #define LIMIT_PCMSK      PCMSK0 // Pin change interrupt register

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR    DDRB
  #define SPINDLE_ENABLE_PORT   PORTB
  // Z Limit pin and spindle PWM/enable pin swapped to access hardware PWM on Pin 11.
  #ifdef VARIABLE_SPINDLE
    #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
      // If enabled, spindle direction pin now used as spindle enable, while PWM remains on D11.
      #define SPINDLE_ENABLE_BIT    5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
    #else
      #define SPINDLE_ENABLE_BIT    3  // Uno Digital Pin 11
    #endif
  #else
    #define SPINDLE_ENABLE_BIT    4  // Uno Digital Pin 12
  #endif
  #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
    #define SPINDLE_DIRECTION_DDR   DDRB
    #define SPINDLE_DIRECTION_PORT  PORTB
    #define SPINDLE_DIRECTION_BIT   5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
  #endif
#define SetSpindleEnablebit()       SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);  // Set pin to high
#define ResetSpindleEnablebit()     SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT); // Set pin to low
#define SetSpindleDirectionBit()    SPINDLE_DIRECTION_PORT |= (1<<SPINDLE_DIRECTION_BIT);
#define ResetSpindleDirectionBit()  SPINDLE_DIRECTION_PORT &= ~(1<<SPINDLE_DIRECTION_BIT);


  // Define flood and mist coolant enable output pins.
  #define COOLANT_FLOOD_DDR   DDRC
  #define COOLANT_FLOOD_PORT  PORTC
  #define COOLANT_FLOOD_BIT   3  // Uno Analog Pin 3
  #define COOLANT_MIST_DDR   DDRC
  #define COOLANT_MIST_PORT  PORTC
  #define COOLANT_MIST_BIT   4  // Uno Analog Pin 4

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       DDRC
  #define CONTROL_PIN       PINC
  #define CONTROL_PORT      PORTC
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     PCMSK1 // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       DDRC
  #define PROBE_PIN       PINC
  #define PROBE_PORT      PORTC
  #define PROBE_BIT       5  // Uno Analog Pin 5
  #define PROBE_MASK      (1<<PROBE_BIT)

  // Variable spindle configuration below. Do not change unless you know what you are doing.
  // NOTE: Only used when variable spindle is enabled.
  #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
  #ifndef SPINDLE_PWM_MIN_VALUE
    #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
  #endif
  #define SPINDLE_PWM_OFF_VALUE     0
  #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
  #define SPINDLE_TCCRA_REGISTER	  TCCR2A
  #define SPINDLE_TCCRB_REGISTER	  TCCR2B
  #define SPINDLE_OCR_REGISTER      OCR2A
  #define SPINDLE_COMB_BIT	        COM2A1

  // Prescaled, 8-bit Fast PWM mode.
  #define SPINDLE_TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.
  // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
  // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
  // #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
  #define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)

  // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
  #define SPINDLE_PWM_DDR	  DDRB
  #define SPINDLE_PWM_PORT  PORTB
  #define SPINDLE_PWM_BIT	  3    // Uno Digital Pin 11

#endif

#ifdef CPU_MAP_STM32F103

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
#define STEP_PORT       GPIOA
#define RCC_STEP_PORT   RCC_APB2Periph_GPIOA
#define X_STEP_BIT      0  
#define Y_STEP_BIT      1  
#define Z_STEP_BIT      2
#define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
#define DIRECTION_PORT      GPIOA
#define RCC_DIRECTION_PORT   RCC_APB2Periph_GPIOA
#define X_DIRECTION_BIT   3  
#define Y_DIRECTION_BIT   4  
#define Z_DIRECTION_BIT   5
#define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
#define STEPPERS_DISABLE_PORT   GPIOA
#define RCC_STEPPERS_DISABLE_PORT RCC_APB2Periph_GPIOA
#define STEPPERS_DISABLE_BIT    6  
#define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)
#define SetStepperDisableBit() GPIO_SetBits(STEPPERS_DISABLE_PORT,STEPPERS_DISABLE_MASK)
#define ResetStepperDisableBit() GPIO_ResetBits(STEPPERS_DISABLE_PORT,STEPPERS_DISABLE_MASK)


  // Define homing/hard limit switch input pins and limit interrupt vectors. 
  // NOTE: All limit bit pins must be on the same port
#define LIMIT_PIN        GPIOB
#define LIMIT_PORT       GPIOB
#define RCC_LIMIT_PORT   RCC_APB2Periph_GPIOB
#define GPIO_LIMIT_PORT  GPIO_PortSourceGPIOB
#define X_LIMIT_BIT      13  
#define Y_LIMIT_BIT      14  
#define Z_LIMIT_BIT      15  

#define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_PORT   GPIOA
#define RCC_SPINDLE_ENABLE_PORT RCC_APB2Periph_GPIOA
#define SPINDLE_ENABLE_BIT    9  // 
#ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
#define SPINDLE_DIRECTION_DDR   GPIOA
#define SPINDLE_DIRECTION_PORT  GPIOA
#define SPINDLE_DIRECTION_BIT   10  // 
#endif
#define SetSpindleEnablebit()       GPIO_WriteBit(SPINDLE_ENABLE_PORT, 1 << SPINDLE_ENABLE_BIT, Bit_SET)
#define ResetSpindleEnablebit()     GPIO_WriteBit(SPINDLE_ENABLE_PORT, 1 << SPINDLE_ENABLE_BIT, Bit_RESET)
#define SetSpindleDirectionBit()    GPIO_WriteBit(SPINDLE_DIRECTION_PORT, 1 << SPINDLE_DIRECTION_BIT,Bit_SET)
#define ResetSpindleDirectionBit()  GPIO_WriteBit(SPINDLE_DIRECTION_PORT, 1 << SPINDLE_DIRECTION_BIT,Bit_RESET)


  // Define flood and mist coolant enable output pins.
  // a later date if flash and memory space allows.
#define COOLANT_FLOOD_PORT            GPIOB
#define RCC_COOLANT_FLOOD_PORT        RCC_APB2Periph_GPIOB
#define COOLANT_FLOOD_BIT             3  
#define COOLANT_MIST_PORT             GPIOB
#define RCC_COOLANT_MIST_PORT         RCC_APB2Periph_GPIOB
#define COOLANT_MIST_BIT              4 

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
#define CONTROL_PIN_PORT              GPIOB
#define CONTROL_PORT                  GPIOB
#define RCC_CONTROL_PORT              RCC_APB2Periph_GPIOB
#define GPIO_CONTROL_PORT             GPIO_PortSourceGPIOB
#define CONTROL_RESET_BIT             5  
#define CONTROL_FEED_HOLD_BIT         6  
#define CONTROL_CYCLE_START_BIT       7  
#define CONTROL_SAFETY_DOOR_BIT       8  
#define CONTROL_MASK                 ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))

  // Define probe switch input pin.
#define PROBE_PORT                    GPIOA
#define RCC_PROBE_PORT                RCC_APB2Periph_GPIOA
#define PROBE_BIT                     15 
#define PROBE_MASK                    (1<<PROBE_BIT)

  // Start of PWM & Stepper Enabled Spindle
#ifdef VARIABLE_SPINDLE
//
  // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
#ifdef SERVO_PEN
#define SPINDLE_PWM_FREQUENCY       500                   // KHz
#else
#define SPINDLE_PWM_FREQUENCY       1000                   // KHz
#endif
#define SPINDLE_PWM_DDR	            GPIOA
#define SPINDLE_PWM_PORT            GPIOA
#define RCC_SPINDLE_PWM_PORT        RCC_APB2Periph_GPIOA
#define SPINDLE_PWM_BIT	            8    
#endif // End of VARIABLE_SPINDLE
#define SPINDLE_PWM_MAX_VALUE       (1000000 / SPINDLE_PWM_FREQUENCY)
#ifndef SPINDLE_PWM_MIN_VALUE
#define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
#endif
#define SPINDLE_PWM_OFF_VALUE     0
#define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)

  //  Port A                                         Port B
  //   0      X_STEP_BIT                             
  //   1      Y_STEP_BIT                            
  //   2      Z_STEP_BIT                               
  //   3      X_DIRECTION_BIT                       COOLANT_FLOOD_BIT
  //   4      Y_DIRECTION_BIT                       COOLANT_MIST_BIT
  //   5      Z_DIRECTION_BIT                       CONTROL_RESET_BIT
  //   6      STEPPERS_DISABLE_BIT                  CONTROL_FEED_HOLD_BIT    
  //   7                                            CONTROL_CYCLE_START_BIT
  //   8      SPINDLE_PWM_BIT                       CONTROL_SAFETY_DOOR_BIT    
  //   9      SPINDLE_ENABLE_BIT                                      
  //   10     SPINDLE_DIRECTION_BIT                 USART3_TX                    
  //   11                                           USART3_RX
  //   12                                            
  //   13 14 SWD																		X_LIMIT_BIT
//     14																						Y_LIMIT_BIT
  //   15     PROBE_BIT					                    Z_LIMIT_BIT

#endif
/*
#ifdef CPU_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and edit one of the available cpu
  // map files and modify it to your needs. Make sure the defined name is also changed in
  // the config.h file.
#endif
*/

#endif
