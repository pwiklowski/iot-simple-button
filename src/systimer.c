#include <ctype.h>
#include <stdio.h>
#include <stdint-gcc.h>
#include "systimer.h"
#include "stm32f0xx_misc.h"

volatile uint32_t uptime_ms = 0;


void SysTick_Handler(void)
{
	uptime_ms++;
}

void delay_ms(uint32_t ms)
{
	uint32_t start = uptime_ms;
	while (uptime_ms - start < ms);
}

void mstimer_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_Config(SystemCoreClock / 1000);
}

uint32_t mstimer_get(void)
{
    return uptime_ms;
}
