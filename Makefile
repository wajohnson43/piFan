.PHONY: driver sender clean

driver:  main

sender: send_temp

main: main.c
	gcc -lwiringPi -o main main.c
	./main &
	
send_temp: send_temp.c
	gcc -o send_temp send_temp.c
	./send_temp 
