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

void startServer(void);

int main(void){
	while(1)
		startServer();

}

void startServer(void){
	int serverFd, newSocket, val_read;
	struct sockaddr_in address;
	int opt = 1;
	int addrelen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	double other_temp;
	FILE *fp;

	//create socket file descriptor
	if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//Forcefully attaching socket to the PORT
	if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if(bind(serverFd, (struct sockaddr *)&address, sizeof(address))<0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if(listen(serverFd, 3) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t*)&addrelen))<0){
		perror("accept");
		exit(EXIT_FAILURE);
	}

	val_read = read(newSocket, &other_temp, sizeof(double));
	//printf("%f\n", other_temp);
	
	fp = fopen("temp", "w+");
	fprintf(fp, "%f", other_temp);
	fclose(fp);
}
