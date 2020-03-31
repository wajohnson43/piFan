/*
 *Written By:		Will Johnson
 *Date:			3/29/20 (COVID-19 Pandemic)
 *Note:			Utilizing Basic BJT transistor and PWM to switch the fan between 0V and 5V.
 *			Must be run as root (PWM)
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

const int PWM_pin = 12;		//using GPIO pin numbering
const int read_pin = 17;
const int max_temp = 47;	//change this to change the temperature the fan comes on

void init(void);		//initialize the fan
void startFan(void);		//start the fan
void killFan(void);		//kill the fan
double getTemp(void);		//get the CPU's temperature

int main(void){
	init();
	double temp, i, otherTemp;
	temp = getTemp();
	
	FILE *f = fopen("temp", "r");
	i = fscanf(f, "%lf", &otherTemp);
	//printf("%lf\n", otherTemp);

	if(temp > max_temp || otherTemp > max_temp){
		startFan();
	}
	else if(temp <= max_temp && otherTemp <= max_temp){
		killFan();
	}

	return 0;
	
}

void init(void){
	if(wiringPiSetupGpio() == -1){
		exit -1;
	}
	pinMode(PWM_pin, PWM_OUTPUT);
	pinMode(read_pin, INPUT);
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
