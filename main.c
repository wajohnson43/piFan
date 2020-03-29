/*
 *Written By:		Will Johnson
 *Date:			3/29/20 (COVID-19 Pandemic)
 *Note:			Utilizing Basic BJT transistor and PWM to switch the fan between 0V and 5V.
 *			Must be run as root (PWM)
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

const int PWM_pin = 12;
const int max_temp = 45;

void initFan(void);		//initialize the fan
void startFan(void);		//start the fan
void killFan(void);		//kill the fan
double getTemp(void);		//get the CPU's temperature

int main(void){
	initFan();
	double temp;
	temp = getTemp();
	if(temp > max_temp){
		startFan();
	}
	else if(temp <= max_temp){
		killFan();
	}
}

void initFan(void){
	if(wiringPiSetupGpio() == -1){
		exit -1;
	}
	pinMode(PWM_pin, PWM_OUTPUT);
}

void startFan(void){
	pwmWrite(PWM_pin, 1024);
}

void  killFan(void){
	pwmWrite(PWM_pin, 0);
}

double getTemp(void){
	FILE *tempFile;
	double temp;
	tempFile = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if(tempFile == NULL){
		printf("Temp FIle not found..."); 
	}
	fscanf(tempFile, "%lf", &temp);
	temp /= 1000;
	fclose (tempFile);
	return temp;
}
