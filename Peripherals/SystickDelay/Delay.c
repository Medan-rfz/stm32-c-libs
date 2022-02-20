#include "stm_pch.h"	// User precompile file in project folder with header on used stm32
#include "Delay.h"


static __IO uint32_t usTicks;

void SysTick_Handler()
{
    if (usTicks != 0)
        usTicks--;
    else
    {
    	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    	usTicks = 0;
    	//SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    }
}

void DelayInit()
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000000); // 1 us
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void DelayUs(uint32_t us)
{
	//SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	while(!(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk))
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	usTicks = us;
	while (usTicks)
	{
		if(!(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk))
			SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}
}

void DelayMs(uint32_t ms)
{
	while (ms--)
		DelayUs(1000);
}
