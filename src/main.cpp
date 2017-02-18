#include <ctype.h>
#include "stm32f0xx.h"
#include "stm32f0xx_exti.h"


#include "rfm69.h"
#include "rfm69hal.h"

extern "C" {
	#include "printf.h"
	#include "systimer.h"
}

void init() {
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	mstimer_init();

	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN, ENABLE);
	GPIO_InitTypeDef gpioStructureButton;
	gpioStructureButton.GPIO_Pin = GPIO_Pin_0;
	gpioStructureButton.GPIO_Mode = GPIO_Mode_IN;
	gpioStructureButton.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructureButton.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioStructureButton.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpioStructureButton);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


}


typedef enum{
	OIC_R_SWITCH_BINARY


}OcfDeviceType;


volatile uint32_t last_click_time=0;
Rfm69 rfm69;
uint8_t packet[6] = {0,0,0,1,OIC_R_SWITCH_BINARY,1};

void sendPressEvent(){
	uint8_t status;

	int16_t len = rfm69.send(packet,6,0);
	rfm69.sleep();
}

extern "C" {
void EXTI0_1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        sendPressEvent();



        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
}



int main() {


	init();

	rfm69.reset();

	if (!rfm69.init()){
		delay_ms(200);
	}

	rfm69.sleep();
	rfm69.setPowerDBm(10); // +10 dBm
	rfm69.setMode(RFM69_MODE_RX);
	rfm69.waitForModeReady();
	rfm69.sleep();



	uint8_t packet[6] = {0,0,0,1,OIC_R_SWITCH_BINARY,1};
	uint8_t status;

	while(1){
		PWR_EnterSTANDBYMode();
	}

	return 0;
}
