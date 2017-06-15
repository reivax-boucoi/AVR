@echo OFF
main.c
avr-gcc -g -Os -mmcu=atmega328p -c main.c
avr-gcc -g -mmcu=atmega328p -o main.elf main.o
if EXIST main.o (goto COMMAND )
echo "Errors !"
PAUSE
main.c
exit

:COMMAND
echo "No errors !"
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
start "" "C:\Program Files (x86)\khazama.com\Khazama AVR Programmer\Khazama AVR Programmer.exe"
wscript "program.vbs"
ECHO O | DEL main.elf
ECHO O | DEL main.o