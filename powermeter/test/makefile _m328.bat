

:BEGIN
main.c
avr-gcc -g -Os -lm -lprintf_flt -Wl,-u,vfprintf -mmcu=atmega328p -c main.c && (
  echo Pas d'erreurs !
) || (
  echo Erreurs !
  PAUSE
  goto BEGIN
)

avr-gcc -g -mmcu=atmega328p -o main.elf main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex

:COMMAND
avrdude -P usb -p m328p -c usbtiny -U flash:w:main.hex && (
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