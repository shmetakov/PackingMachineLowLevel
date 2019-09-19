#include "global.h"
#include "magnet.h"

void init_Magnet()
{
	GPIO_InitTypeDef magnetInit;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	magnetInit.GPIO_Mode = GPIO_Mode_OUT;
	magnetInit.GPIO_OType = GPIO_OType_PP;
	magnetInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	magnetInit.GPIO_Speed = GPIO_Speed_2MHz;

	magnetInit.GPIO_Pin = MAGNET_PIN_1;
	GPIO_Init(MAGNET_GPIOx_1, &magnetInit);
	MAGNET_1_OFF();

	magnetInit.GPIO_Pin = MAGNET_PIN_2;
	GPIO_Init(MAGNET_GPIOx_2, &magnetInit);
	MAGNET_2_OFF();

	magnetInit.GPIO_Pin = MAGNET_PIN_3;
	GPIO_Init(MAGNET_GPIOx_3, &magnetInit);
	MAGNET_3_OFF();

	magnetInit.GPIO_Pin = MAGNET_PIN_4;
	GPIO_Init(MAGNET_GPIOx_4, &magnetInit);
	MAGNET_4_OFF();

	magnetInit.GPIO_Pin = MAGNET_PIN_5;
	GPIO_Init(MAGNET_GPIOx_5, &magnetInit);
	MAGNET_5_OFF();

	magnetInit.GPIO_Pin = MAGNET_PIN_6;
	GPIO_Init(MAGNET_GPIOx_6, &magnetInit);
	MAGNET_6_OFF();

	magnetInit.GPIO_Pin = MAGNET_PIN_7;
	GPIO_Init(MAGNET_GPIOx_7, &magnetInit);
	MAGNET_7_OFF();

	magnetInit.GPIO_Pin = MAGNET_PIN_8;
	GPIO_Init(MAGNET_GPIOx_8, &magnetInit);
	MAGNET_8_OFF();

	_delay_magnet_work_1 = 0;
	_delay_magnet_work_2 = 0;
	_delay_magnet_work_3 = 0;
	_delay_magnet_work_4 = 0;
	_delay_magnet_work_5 = 0;
	_delay_magnet_work_6 = 0;
	_delay_magnet_work_7 = 0;
	_delay_magnet_work_8 = 0;
}

_Bool magnet_set_state(uint8_t mag,  uint16_t ms)
{
	switch(mag){
		case 0x01:{
			MAGNET_1_ON();
			_delay_magnet_work_1 = ms;
			break;
		}
		case 0x02:{
			MAGNET_2_ON();
			_delay_magnet_work_2 = ms;
			break;
		}
		case 0x03:{
			MAGNET_3_ON();
			_delay_magnet_work_3 = ms;
			break;
		}
		case 0x04:{
			MAGNET_4_ON();
			_delay_magnet_work_4 = ms;
			break;
		}
		case 0x05:{
			MAGNET_5_ON();
			_delay_magnet_work_5 = ms;
			break;
		}
		case 0x06:{
			MAGNET_6_ON();
			_delay_magnet_work_6 = ms;
			break;
		}
		case 0x07:{
			MAGNET_7_ON();
			_delay_magnet_work_7 = ms;
			break;
		}
		case 0x08:{
			MAGNET_8_ON();
			_delay_magnet_work_8 = ms;
			break;
		}

		default: return false;
	}

	return true;
}

inline void magnet_delay_work()
{
	if (_delay_magnet_work_1){
		_delay_magnet_work_1--;
		if (!_delay_magnet_work_1){
			MAGNET_1_OFF();
		}
	}

	if (_delay_magnet_work_2){
		_delay_magnet_work_2--;
		if (!_delay_magnet_work_2){
			MAGNET_2_OFF();
		}
	}

	if (_delay_magnet_work_3){
		_delay_magnet_work_3--;
		if (!_delay_magnet_work_3){
			MAGNET_3_OFF();
		}
	}

	if (_delay_magnet_work_4){
		_delay_magnet_work_4--;
		if (!_delay_magnet_work_4){
			MAGNET_4_OFF();
		}
	}

	if (_delay_magnet_work_5){
		_delay_magnet_work_5--;
		if (!_delay_magnet_work_5){
			MAGNET_5_OFF();
		}
	}

	if (_delay_magnet_work_6){
		_delay_magnet_work_6--;
		if (!_delay_magnet_work_6){
			MAGNET_6_OFF();
		}
	}

	if (_delay_magnet_work_7){
		_delay_magnet_work_7--;
		if (!_delay_magnet_work_7){
			MAGNET_7_OFF();
		}
	}

	if (_delay_magnet_work_8){
		_delay_magnet_work_8--;
		if (!_delay_magnet_work_8){
			MAGNET_8_OFF();
		}
	}
}
