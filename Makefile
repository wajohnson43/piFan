.PHONY: all cron clean

all: main cron

cron:
	sudo echo "* * * * * /home/pi/piFan/main" >> /var/spool/cron/crontabs/root

main: main.c
	gcc -lwiringPi -o main main.c

clean: 
	@echo "Cleaning up..."
	rm main
	sed -i '$$ d'  /var/spool/cron/crontabs/root
