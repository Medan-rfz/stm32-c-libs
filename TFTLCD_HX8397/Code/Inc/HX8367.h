#ifndef __HX8367_H
#define __HX8367_H

#include <stdint.h>

void HX8367_Init();
void HX8367_EnterSleep();
void HX8367_ExitSleep();
void HX8367_test();

typedef enum
{
	HX8367_statOK,
	HX8367_statErr,
} HX8367_Stat;

#endif


