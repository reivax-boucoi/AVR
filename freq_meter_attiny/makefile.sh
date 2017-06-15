#!/bin/bash
echo "making..."

avr-gcc -g -Os -mmcu=attiny85 -c main.c
avr-gcc -g -mmcu=attiny85 -o main.elf main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
sudo avrdude -P usb -p t85 -c usbasp -U flash:w:main.hex
