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

	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN, ENABLE);
	GPIO_InitTypeDef gpioStructureButton;
	gpioStructureButton.GPIO_Pin = GPIO_Pin_0;
	gpioStructureButton.GPIO_Mode = GPIO_Mode_IN;
	gpioStructureButton.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructureButton.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioStructureButton.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpioStructureButton);
}


typedef enum{
	OIC_R_SWITCH_BINARY


}OcfDeviceType;


int main() {
	init();

	Rfm69 rfm69;
	rfm69.reset();

	if (!rfm69.init()){
		delay_ms(200);
	}

	rfm69.sleep();
	rfm69.setPowerDBm(10); // +10 dBm
	rfm69.setMode(RFM69_MODE_RX);
	rfm69.waitForModeReady();

	uint8_t packet[6] = {0,1,2,3,OIC_R_SWITCH_BINARY,1};
	uint8_t status;

	while(1){
		status = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		if (status)
		{
			delay_ms(80);
			status = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
			if (status){
				int16_t len = rfm69.send(packet,6,0);
			}
		}
	}

	return 0;
}
