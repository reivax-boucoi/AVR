@echo OFF
main.c
avr-gcc -g -Os -mmcu=atmega32 -c main.c
avr-gcc -g -mmcu=atmega32 -o main.elf main.o
if EXIST main.o (goto COMMAND )
echo "Errors !"
PAUSE
main.c
exit

:COMMAND
echo "No errors !"
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
avrdude -P usb -p m32 -c usbtiny -F -U flash:w:main.hex
ECHO O | DEL main.o