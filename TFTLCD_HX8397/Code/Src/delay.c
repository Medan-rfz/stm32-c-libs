#include "delay.h"
#include "main.h"

static __IO uint32_t usTicks;

void SysTick_Handler()
{
    if (usTicks != 0)
        usTicks--;
    else
    	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void DelayInit()
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000000); // 1 us
}

void DelayUs(uint32_t us)
{
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	usTicks = us;
	while (usTicks);
}

void DelayMs(uint32_t ms)
{
	while (ms--)
		DelayUs(1000);
}
