#ifndef UTILITIES_H
#define UTILITIES_H

#include <unistd.h>
#define CAN_BITRATE (1000000u) // Can Bit Rate in Hz (1 MHz)
#define SYSTICKHZ 200          // SysTickHandler frequency
#define TD 0.005f              // fixed time step [s] (1/SYSTICKHZ)
#define BUZTICKHZ 20           // Buzzer Handler frequency
#define G 9.81f
#define M_PI 3.14159265358979323846f
#define KNEE_ROM 115.0f        // Full mechanical ROM of the prosthesis
#define KNEE_STANCE_ANGLE 3.0f // Physiological angle while standing straight

void delayMicrosec(useconds_t delay);

#endif // UTILITIES_H
