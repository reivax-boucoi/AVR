@echo OFF
main.c
avr-gcc -g -Os -mmcu=atmega88 -c main.c
avr-gcc -g -mmcu=atmega88 -o main.elf main.o
if EXIST main.o (goto COMMAND )
echo "Errors !"
PAUSE
main.c
exit

:COMMAND
echo "No errors !"
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
avrdude -P usb -p m88 -c usbasp -F -U flash:w:main.hex
ECHO O | DEL main.elf
ECHO O | DEL main.o