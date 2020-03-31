/*
 *Written By:		Will Johnson
 *Date:			3/29/20 (COVID-19 Pandemic)
 *Note:			Utilizing Basic BJT transistor and PWM to switch the fan between 0V and 5V.
 *			Must be run as root (PWM)
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

const int pin = 17;
const int max_temp = 30;

void init(void);		//initialize the fan
double getTemp(void);		//get the CPU's temperature
void sendTemp(double);

int main(void){
	init();
	double temp = getTemp();
	printf("%g", temp);
	sendTemp(temp);
	return 0;	
}

void init(void){
	if(wiringPiSetupGpio() == -1){
		exit -1;
	}
	pinMode(pin, OUTPUT);
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

void sendTemp(double temp){
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	//convert IPV4 and IPV6 address from text to binary form 
	if(inet_pton(AF_INET, "192.168.0.220", &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address, or address not supported \n");
		exit(EXIT_FAILURE);
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0){
		printf("\nConnection Failed \n");
		exit(EXIT_FAILURE);
	}
	send(sock, &temp, sizeof(double), 0);
}

