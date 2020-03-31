.PHONY: driver sender clean

driver: read_pi main

sender: send_temp

main: main.c
	gcc -lwiringPi -o main main.c
	sudo echo "* * * * * /home/pi/piFan/main" >> /var/spool/cron/crontabs/root

read_pi: read_pi.c
	gcc -o read_pi read_pi.c
	./read_pi &

send_temp: send_temp.c
	gcc -o send_temp send_temp.c
	sudo echo "* * * * */home/pi/piFan/send_temp" >> /var/spool/cron/crontabs/root
clean: 
	@echo "Cleaning up..."
	rm main
	rm read_pi
	rm send_temp
	sed -i '$$ d'  /var/spool/cron/crontabs/root
