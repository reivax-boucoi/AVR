@echo OFF

:BEGIN
main.c
avr-gcc -g -Os -mmcu=attiny84 -c main.c && (
  echo Pas d'erreurs !
) || (
  echo Erreurs !
  PAUSE
  goto BEGIN
)

avr-gcc -g -mmcu=attiny84 -o main.elf main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex

:COMMAND
avrdude -P usb -p t84 -c usbasp -U flash:w:main.hex && (
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