/*
 *Written By:		Will Johnson
 *Date:			3/29/20 (COVID-19 Pandemic)
 *Note:			Utilizing Basic BJT transistor and PWM to switch the fan between 0V and 5V.
 *			Must be run as root (PWM)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>

#define PORT 8080

const int max_temp = 50;
const int write_pin = 17;

void init(void);
double getTemp(void);		//get the CPU's temperature

int main(void){
	init();
	while(1){
		double temp = getTemp();
		if(temp <= max_temp){
			digitalWrite(write_pin, 0);
		}
		else{
			digitalWrite(write_pin, 1);
		}
		delay(1000);
	}
	return 0;
}

void init(void){
	if(wiringPiSetupGpio() == -1){
		exit(EXIT_FAILURE);
	}	
	pinMode(write_pin, OUTPUT);
}

double getTemp(void){
	FILE *tempFile;
	double temp;
	tempFile = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if(tempFile == NULL){
		printf("Temp File not found..."); 
	}
	fscanf(tempFile, "%lf", &temp);
	temp /= 1000;
	fclose (tempFile);
	return(temp);
}

