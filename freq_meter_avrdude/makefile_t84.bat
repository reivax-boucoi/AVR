@echo OFF
main.c
avr-gcc -g -Os -mmcu=attiny84 -c main.c
avr-gcc -g -mmcu=attiny84 -o main.elf main.o
if EXIST main.o (goto COMMAND )
echo "Errors !"
PAUSE
main.c
exit

:COMMAND
echo "No errors !"
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
avrdude -P usb -p t84 -c usbasp -F -e -U flash:w:main.hex
ECHO O | DEL main.elf
ECHO O | DEL main.o