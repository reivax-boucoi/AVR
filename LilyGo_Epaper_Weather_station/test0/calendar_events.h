#ifndef CALENDAR_EVENTS_H_
#define CALENDAR_EVENTS_H_

#include <Arduino.h>
/*
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

const uint8_t BIRTHDAY_COUNT =  sizeof(birthdays) / sizeof(birthdays[0]);*/
String currentBirthday="";
const char * const saints[12][31] = {
  // January
  {
    "Basile",            // 1
    "Grégoire",          // 2
    "Geneviève",         // 3
    "Odilon",            // 4
    "Édouard",           // 5
    "Mélaine",           // 6
    "Raymond",           // 7
    "Lucien",            // 8
    "Alix",              // 9
    "Guillaume",         // 10
    "Paulin",            // 11
    "Tatiana",           // 12
    "Yvette",            // 13
    "Nina",              // 14
    "Rémi",              // 15
    "Marcel",            // 16
    "Roseline",          // 17
    "Prisca",            // 18
    "Marius",            // 19
    "Sébastien",         // 20
    "Agnès",             // 21
    "Vincent",           // 22
    "Barnard",           // 23
    "François",          // 24
    "Ananie",            // 25
    "Paule",             // 26
    "Angèle",            // 27
    "Thomas",            // 28
    "Gildas",            // 29
    "Martine",           // 30
    "Marcelle"           // 31
  },
  // February
  {
    "Ella",              // 1
    "Théophane",         // 2
    "Blaise",            // 3
    "Véronique",         // 4
    "Agathe",            // 5
    "Gaston",            // 6
    "Eugénie",           // 7
    "Jacqueline",        // 8
    "Apolline",          // 9
    "Arnaud",            // 10
    "Heloïse",           // 11
    "Félix",             // 12
    "Béatrice",          // 13
    "Valentin",          // 14
    "Claude",            // 15
    "Julienne",          // 16
    "Alexis",            // 17
    "Bernadette",        // 18
    "Gabin",             // 19
    "Aimée",             // 20
    "Damien",            // 21
    "Isabelle",          // 22
    "Lazare",            // 23
    "Modeste",           // 24
    "Roméo",             // 25
    "Nestor",            // 26
    "Honorine",          // 27
    "Romain",            // 28
    "Auguste",           // 29
    "",                  // 30
    ""                   // 31
  },
  // March
  {
    "Aubin",             // 1
    "Charles",           // 2
    "Guénolé",           // 3
    "Casimir",           // 4
    "Olive",             // 5
    "Colette",           // 6
    "Félicité",          // 7
    "Jean de Dieu",      // 8
    "Françoise",         // 9
    "Vivien",            // 10
    "Rosine",            // 11
    "Justine",           // 12
    "Rodrigue",          // 13
    "Mathilde",          // 14
    "Louise",            // 15
    "Bénédicte",         // 16
    "Patrick",           // 17
    "Cyrille",           // 18
    "Joseph",            // 19
    "Herbert",           // 20
    "Clémence",          // 21
    "Léa",               // 22
    "Victorien",         // 23
    "Catherine",         // 24
    "Humbert",           // 25
    "Larissa",           // 26
    "Habib",             // 27
    "Gontran",           // 28
    "Gwladys",           // 29
    "Amédée",            // 30
    "Benjamin"           // 31
  },
  // April
  {
    "Hugues",            // 1
    "Sandrine",          // 2
    "Richard",           // 3
    "Isidore",           // 4
    "Irène",             // 5
    "Marcellin",         // 6
    "Jean-Baptiste",     // 7
    "Julie",             // 8
    "Gautier",           // 9
    "Fulbert",           // 10
    "Stanislas",         // 11
    "Jules",             // 12
    "Ida",               // 13
    "Maxime",            // 14
    "Paterne",           // 15
    "Benoît-Joseph",     // 16
    "Anicet",            // 17
    "Parfait",           // 18
    "Emma",              // 19
    "Odette",            // 20
    "Anselme",           // 21
    "Alexandre",         // 22
    "Georges",           // 23
    "Fidèle",            // 24
    "Marc",              // 25
    "Alida",             // 26
    "Zita",              // 27
    "Valérie",           // 28
    "Catherine",         // 29
    "Robert",            // 30
    ""                   // 31
  },
  // May
  {
    "Jérémie",           // 1
    "Boris",             // 2
    "Philippe",          // 3
    "Sylvain",           // 4
    "Judith",            // 5
    "Prudence",          // 6
    "Gisèle",            // 7
    "Désiré",            // 8
    "Pacôme",            // 9
    "Solange",           // 10
    "Estelle",           // 11
    "Achille",           // 12
    "Rolande",           // 13
    "Matthias",          // 14
    "Denise",            // 15
    "Honoré",            // 16
    "Pascal",            // 17
    "Éric",              // 18
    "Yves",              // 19
    "Bernardin",         // 20
    "Constantin",        // 21
    "Émile",             // 22
    "Didier",            // 23
    "Donatien",          // 24
    "Sophie",            // 25
    "Bérenger",          // 26
    "Augustin",          // 27
    "Germain",           // 28
    "Aymar",             // 29
    "Ferdinand",         // 30
    "Perrine"            // 31
  },
  // June
  {
    "Justin",            // 1
    "Blandine",          // 2
    "Kévin",             // 3
    "Clotilde",          // 4
    "Igor",              // 5
    "Norbert",           // 6
    "Gilbert",           // 7
    "Médard",            // 8
    "Diane",             // 9
    "Landry",            // 10
    "Barnabé",           // 11
    "Guy",               // 12
    "Antoine",           // 13
    "Élisée",            // 14
    "Germaine",          // 15
    "Régis",             // 16
    "Hervé",             // 17
    "Léonce",            // 18
    "Romuald",           // 19
    "Silvère",           // 20
    "Rodolphe",          // 21
    "Alban",             // 22
    "Audrey",            // 23
    "Jean-Baptiste",     // 24
    "Prosper",           // 25
    "Anthelme",          // 26
    "Fernand",           // 27
    "Irénée",            // 28
    "Pierre",            // 29
    "Martial",           // 30
    ""                   // 31
  },
  // July
  {
    "Thierry",           // 1
    "Martinien",         // 2
    "Thomas",            // 3
    "Florent",           // 4
    "Antoine",           // 5
    "Mariette",          // 6
    "Raoul",             // 7
    "Thibault",          // 8
    "Amandine",          // 9
    "Ulrich",            // 10
    "Benoît",            // 11
    "Olivier",           // 12
    "Henri",             // 13
    "Camille",           // 14
    "Bonaventure",       // 15
    "Carmen",            // 16
    "Charlotte",         // 17
    "Frédéric",          // 18
    "Arsène",            // 19
    "Marina",            // 20
    "Victor",            // 21
    "Madeleine",         // 22
    "Brigitte",          // 23
    "Christine",         // 24
    "Jacques",           // 25
    "Anne",              // 26
    "Nathalie",          // 27
    "Samson",            // 28
    "Marthe",            // 29
    "Juliette",          // 30
    "Ignace"             // 31
  },
  // August
  {
    "Alphonse",          // 1
    "Julien",            // 2
    "Lydie",             // 3
    "Jean-Marie",        // 4
    "Abel",              // 5
    "Félicissime",       // 6
    "Gaétan",            // 7
    "Dominique",         // 8
    "Amour",             // 9
    "Laurent",           // 10
    "Claire",            // 11
    "Clarisse",          // 12
    "Hippolyte",         // 13
    "Évrard",            // 14
    "Marie",             // 15
    "Armel",             // 16
    "Hyacinthe",         // 17
    "Hélène",            // 18
    "Jean-Eudes",        // 19
    "Bernard",           // 20
    "Christophe",        // 21
    "Fabrice",           // 22
    "Rose",              // 23
    "Barthélemy",        // 24
    "Louis",             // 25
    "Natacha",           // 26
    "Monique",           // 27
    "Augustin",          // 28
    "Sabine",            // 29
    "Fiacre",            // 30
    "Aristide"           // 31
  },
  // September
  {
    "Gilles",            // 1
    "Ingrid",            // 2
    "Grégoire",          // 3
    "Rosalie",           // 4
    "Raïssa",            // 5
    "Bertrand",          // 6
    "Reine",             // 7
    "Nativité",          // 8
    "Alain",             // 9
    "Inès",              // 10
    "Adelphe",           // 11
    "Apollinaire",       // 12
    "Aimé",              // 13
    "Lubin",             // 14
    "Roland",            // 15
    "Édith",             // 16
    "Renaud",            // 17
    "Nadège",            // 18
    "Émilie",            // 19
    "Davy",              // 20
    "Matthieu",          // 21
    "Maurice",           // 22
    "Constant",          // 23
    "Thècle",            // 24
    "Hermann",           // 25
    "Côme",              // 26
    "Vincent",           // 27
    "Venceslas",         // 28
    "Michel",            // 29
    "Jérôme",            // 30
    ""                   // 31
  },
  // October
  {
    "Thérèse",           // 1
    "Léger",             // 2
    "Gérard",            // 3
    "François",          // 4
    "Fleur",             // 5
    "Bruno",             // 6
    "Serge",             // 7
    "Pélagie",           // 8
    "Denis",             // 9
    "Ghislain",          // 10
    "Firmin",            // 11
    "Wilfried",          // 12
    "Géraud",            // 13
    "Juste",             // 14
    "Thérèse",           // 15
    "Edwige",            // 16
    "Baudoin",           // 17
    "Luc",               // 18
    "René",              // 19
    "Adeline",           // 20
    "Céline",            // 21
    "Élodie",            // 22
    "Arnould",           // 23
    "Florentin",         // 24
    "Crépin",            // 25
    "Dimitri",           // 26
    "Émeline",           // 27
    "Simon",             // 28
    "Narcisse",          // 29
    "Bienvenue",         // 30
    "Quentin"            // 31
  },
  // November
  {
    "Toussaint",         // 1
    "Défunts",           // 2
    "Hubert",            // 3
    "Charles",           // 4
    "Sylvie",            // 5
    "Bertille",          // 6
    "Carine",            // 7
    "Geoffroy",          // 8
    "Théodore",          // 9
    "Léon",              // 10
    "Martin",            // 11
    "Christian",         // 12
    "Brice",             // 13
    "Sidoine",           // 14
    "Albert",            // 15
    "Marguerite",        // 16
    "Élisabeth",         // 17
    "Aude",              // 18
    "Tanguy",            // 19
    "Edmond",            // 20
    "Rufus",             // 21
    "Cécile",            // 22
    "Clément",           // 23
    "Flora",             // 24
    "Catherine",         // 25
    "Delphine",          // 26
    "Séverin",           // 27
    "Jacques",           // 28
    "Saturnin",          // 29
    "André",             // 30
    ""                   // 31
  },
  // December
  {
    "Florence",          // 1
    "Viviane",           // 2
    "Xavier",            // 3
    "Barbara",           // 4
    "Gérald",            // 5
    "Nicolas",           // 6
    "Ambroise",          // 7
    "Immaculée\nConception", // 8
    "Pierre-\nvFourier",    // 9
    "Romaric",           // 10
    "Daniel",            // 11
    "Chantal",           // 12
    "Lucie",             // 13
    "Odile",             // 14
    "Ninon",             // 15
    "Alice",             // 16
    "Gaël",              // 17
    "Gatien",            // 18
    "Urbain",            // 19
    "Théophile",         // 20
    "Pierre",            // 21
    "Françoise-\nXavière", // 22
    "Armand",            // 23
    "Adèle",             // 24
    "Emmanuel",          // 25
    "Étienne",           // 26
    "Jean",              // 27
    "Gaspard",           // 28
    "David",             // 29
    "Roger",             // 30
    "Sylvestre"          // 31
  }
};


#endif /* ifndef CALENDAR_EVENTS_H_ */
