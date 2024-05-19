#ifndef INC_HEADSUP_H_
#define INC_HEADSUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "digital_io.h"
#include "gyroscope.h"
#include "serial.h"
#include "stm32f303xc.h"
#include "timers.h"

char* randomWord();

void headsUp(uint8_t *P1score, uint8_t *P2score);

#endif
