#ifndef ALTI_H
#define ALTI_H
#include "Arduino.h"

#define FLOOR_HEIGHT (3*16)
#define ALT_RdC_INIT (130*16)
#define DEBUG_MPL 1
#define DEBUG_Alt 1

class Alt_Data {
    private:
        uint32_t alti;
        uint8_t etage;
        uint32_t alt_RdC;
    public:
        Alt_Data();
        volatile bool newDataReady;
        uint8_t getEtage(uint32_t alt);
};

void MPL_init();
uint32_t MPL_readAlt();
void MPL_readAltTemp(uint32_t *alt, uint16_t *temp);

#endif
