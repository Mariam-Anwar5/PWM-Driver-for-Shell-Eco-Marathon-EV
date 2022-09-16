#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PWM.h"

void PWM_Init()
{
	SYSCTL_RCGCPWM_R |= 0x01;
        SYSCTL_RCGCPWM_R |= 0X02;
	
	SYSCTL_RCC_R &= ~(1 << 20);
	
	GPIO_PORTB_DIR_R |= 0xF0;
	GPIO_PORTE_DIR_R |= 0x30;
        
	
	GPIO_PORTB_AFSEL_R |= 0xF0;
	GPIO_PORTE_AFSEL_R |= 0x30;
	
	GPIO_PORTB_PCTL_R = (GPIO_PORTF_PCTL_R & 0x0000FFFF) | (0x44440000);
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFF00FFFF) | (0x00440000);
	
	GPIO_PORTB_DEN_R |= 0xF0;
	GPIO_PORTE_DEN_R |= 0x30;
        
        GPIO_PORTF_AFSEL_R |= (1<<2);     /* PF2 sets a alternate function */
        GPIO_PORTF_PCTL_R &= ~0x00000F00; /*set PF2 as output pin */
        GPIO_PORTF_PCTL_R |= 0x00000500; /* make PF2 PWM output pin */
        GPIO_PORTF_DEN_R |= (1<<2);      /* set PF2 as a digital pin */
	
	PWM0_0_CTL_R &= ~0x03;
	PWM0_1_CTL_R &= ~0x03;
	PWM0_2_CTL_R &= ~0x03;
	
	PWM0_0_GENA_R = 0x8C;
	PWM0_0_GENB_R = 0x80C;
	PWM0_1_GENA_R = 0x8C;
	PWM0_1_GENB_R = 0x80C;
	PWM0_2_GENA_R = 0x8C;
	PWM0_2_GENB_R = 0x80C;
	
	PWM0_0_LOAD_R = 8000 - 1;
	PWM0_1_LOAD_R = 8000 - 1;
	PWM0_2_LOAD_R = 8000 - 1;
	
	PWM0_0_CMPA_R = 8000 - 1;
	PWM0_0_CMPB_R = 8000 - 2;
	PWM0_1_CMPA_R = 8000 - 1;
	PWM0_1_CMPB_R = 8000 - 2;
	PWM0_2_CMPA_R = 8000 - 1;
	PWM0_2_CMPB_R = 8000 - 2;
	
	PWM0_0_CTL_R |= 0x01;
	PWM0_1_CTL_R |= 0x01;
	PWM0_2_CTL_R |= 0x01;
	
	PWM0_ENABLE_R = 0x00;
        
        PWM1_3_CTL_R &= ~(1<<0);   /* Disable Generator 3 counter */
        PWM1_3_CTL_R &= ~(1<<1);   /* select down count mode of counter 3*/
        PWM1_3_GENA_R = 0x0000008C; /* Set PWM output when counter reloaded and clear when matches PWMCMPA */
        PWM1_3_LOAD_R = 16000;   /* set load value for 1kHz (16MHz/16000) */
        PWM1_3_CMPA_R = 8000-1;  /* set duty cyle to 50% by loading of 16000 to PWM1CMPA */
        PWM1_3_CTL_R = 1;         /* Enable Generator 3 counter */
        PWM1_ENABLE_R = 0x00;      /* Enable PWM1 channel 6 output */
}

void SetDutyCycle(int duty_cycle)
{
	if(duty_cycle >= 100)
	{
		PWM0_0_CMPB_R = 8000 - 1;
		PWM0_1_CMPB_R = 8000 - 1;
		PWM0_2_CMPB_R = 8000 - 1;
	}
	
	else if(duty_cycle <= 0)
	{
		PWM0_0_CMPB_R = 8000 - 2;
		PWM0_1_CMPB_R = 8000 - 2;
		PWM0_2_CMPB_R = 8000 - 2;
	}
		
	else
	{
		PWM0_0_CMPB_R = (uint16_t)((8000 - ((8000 * duty_cycle) / 100)) - 1);
		PWM0_1_CMPB_R = (uint16_t)((8000 - ((8000 * duty_cycle) / 100)) - 1);
		PWM0_2_CMPB_R = (uint16_t)((8000 - ((8000 * duty_cycle) / 100)) - 1);
                
                PWM1_3_CMPA_R = (uint16_t)((8000 - ((8000 * duty_cycle) / 100)) - 1);
                
	}
}
