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
	mstimer_init();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN, ENABLE);
	GPIO_InitTypeDef gpioStructureButton;
	gpioStructureButton.GPIO_Pin = GPIO_Pin_0;
	gpioStructureButton.GPIO_Mode = GPIO_Mode_IN;
	gpioStructureButton.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructureButton.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioStructureButton.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpioStructureButton);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
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



int main() {


	init();
	PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);

	rfm69.reset();
	while (!rfm69.init()){
		rfm69.reset();
	}
	rfm69.setPowerDBm(10); // +10 dBm
	sendPressEvent();
	rfm69.sleep();
	//delay_ms(6000);

	PWR_EnterSTANDBYMode();
	while(1);

	return 0;
}
