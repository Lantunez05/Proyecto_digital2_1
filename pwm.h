/* 
 * File:   pwm.h
 * Author: Luis Antunez
 *
 * Created on 15 de mayo de 2023, 05:26 PM
 */

#ifndef pwm_H
#define pwm_H 

#include <xc.h>

void PWM_Init(void);
void PWM_dc(unsigned int duty_cycle);

#endif