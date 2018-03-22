@echo OFF

:BEGIN
main.cpp
avr-gcc -g -Os -lm -lprintf_flt -Wl,-u,vfprintf -mmcu=atmega169 -c main.cpp && (
  echo Pas d'erreurs !
) || (
  echo Erreurs !
  PAUSE
  goto BEGIN
)

avr-gcc -g -mmcu=atmega169 -o main.elf main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex

:COMMAND
avrdude -P usb -p m169 -c usbtiny -U flash:w:main.hex && (
  echo Trouve Programmeur !
) || (
  echo Pas de programmeur !
  PAUSE
  goto COMMAND
)

ECHO O | DEL main.elf
ECHO O | DEL main.o
echo Programme telecharge !
PAUSE
goto BEGIN