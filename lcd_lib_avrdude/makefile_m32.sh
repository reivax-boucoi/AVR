#!/bin/bash
while true
do
	avr-gcc -g -Os -mmcu=atmega32 -c main.c 
	if test $? -ne 0
	then
		echo "Erreurs !"
		exit
	else
		echo "Pas d'erreurs !"
	
	fi

	avr-gcc -g -mmcu=atmega32 -o main.elf main.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	read -p 'Press [Enter] to upload'
	
	avrdude -P usb -p m32 -c usbtiny -U flash:w:main.hex
	if test $? -ne 0
	then
		echo 'Unable to flash target, did you plug the programmer ? :)'
		exit
	else
		rm main.elf
		rm main.o
		echo Programme telecharge !
		read -p 'Press [Enter] to recompile'
	fi

done
