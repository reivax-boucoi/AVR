#ifndef CALENDAR_EVENTS_H_
#define CALENDAR_EVENTS_H_

#include <Arduino.h>

struct Birthday {
  uint8_t day;
  uint8_t month;
  const char *name;
};

const Birthday birthdays[] = {
  { 28, 1, "Guillaume C" },
  { 1 , 2, "Clément" },
  { 9 , 2, "Pierre" },
  { 19, 2, "Félix" },
  { 20, 2, "Lise" },
  { 22, 2, "Claire" },
  { 18, 3, "Guillaume L" },
  { 23, 3, "Paul" },
  { 7 , 4, "Romain" },
  { 1 , 5, "Marguerite" },
  { 3 , 5, "Xavier" },
  { 6 , 5, "Huguette" },
  { 9 , 5, "Dominique" },
  { 26, 6, "Papa" },
  { 4 , 7, "Jean-Luc" },//JL ou Marithé?
  { 31, 7, "Jean-Baptiste" },//JB ou Estelle?
  { 3 , 8, "Isaïa" },
  { 5 , 8, "Paolino" },
  { 6 , 8, "Maman" },
  { 7 , 8, "Bertrand" },
  { 11, 8, "Elio" },
  { 24, 8, "Clémence" },
  { 15, 9, "Lucile" },
  { 12, 9, "Gaëtan" },
  { 18, 12,"Adèle" }
};

const uint8_t BIRTHDAY_COUNT =  sizeof(birthdays) / sizeof(birthdays[0]);
#endif
