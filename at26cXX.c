#include "global.h"
#include "at26cXX.h"
#include "led.h"

#define EEPROM_HW_ADDRESS      0xA0

uint32_t I2C_Timeout;

void at26cXX_Init()
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);

	I2C_DeInit(I2C3);

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 1;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_CLOCK_SPEED;

	I2C_Cmd(I2C3, ENABLE);
	I2C_Init(I2C3, &I2C_InitStructure);
}

void at26cXX_ByteWrite(uint8_t val, uint16_t WriteAddr)
{
    /* Send START condition */
    I2C_GenerateSTART(I2C3, ENABLE);

    /* Test on EV5 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)) && I2C_Timeout){
    	I2C_Timeout--;
    }

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C3, EEPROM_HW_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send the EEPROM's internal address to write to : MSB of the address first */
    I2C_SendData(I2C3, (uint8_t)((WriteAddr & 0xFF00) >> 8));

    /* Test on EV8 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData(I2C3, (uint8_t)(WriteAddr & 0x00FF));

    /* Test on EV8 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    I2C_SendData(I2C3, val);

    /* Test on EV8 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C3, ENABLE);

    //delay between write and read...not less 4ms
    Soft_Delay(5);
}

void at26cXX_PageWrite(uint8_t* val, uint16_t WriteAddr)
{
    /* Send START condition */
    I2C_GenerateSTART(I2C3, ENABLE);

    /* Test on EV5 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)) && I2C_Timeout){
    	I2C_Timeout--;
    }

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C3, EEPROM_HW_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send the EEPROM's internal address to write to : MSB of the address first */
    I2C_SendData(I2C3, (uint8_t)((WriteAddr & 0xFF00) >> 8));

    /* Test on EV8 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData(I2C3, (uint8_t)(WriteAddr & 0x00FF));

    /* Test on EV8 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    for (uint8_t i = 0; i < 32; i++){

        I2C_SendData(I2C3, val[i]);

        /* Test on EV8 and clear it */
        I2C_Timeout = I2C_TIMEOUT;
        while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
           	I2C_Timeout--;
        }
    }

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C3, ENABLE);

    //delay between write and read...not less 4ms
    Soft_Delay(5);
}

uint8_t at26cXX_ByteRead(uint16_t ReadAddr)
{
    uint8_t tmp;

    /* While the bus is busy */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_FLAG_BUSY)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send START condition */
    I2C_GenerateSTART(I2C3, ENABLE);

    /* Test on EV5 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C3, EEPROM_HW_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send the EEPROM's internal address to read from: MSB of the address first */
    I2C_SendData(I2C3, (uint8_t)((ReadAddr & 0xFF00) >> 8));

    /* Test on EV8 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send the EEPROM's internal address to read from: LSB of the address */
    I2C_SendData(I2C3, (uint8_t)(ReadAddr & 0x00FF));

    /* Test on EV8 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C3, ENABLE);

    /* Test on EV5 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C3, EEPROM_HW_ADDRESS, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    I2C_Timeout = I2C_TIMEOUT;
    while((!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_RECEIVED)) && I2C_Timeout){
       	I2C_Timeout--;
    }

    tmp = I2C_ReceiveData(I2C3);

    I2C_AcknowledgeConfig(I2C3, DISABLE);

    /* Send STOP Condition */
    I2C_GenerateSTOP(I2C3, ENABLE);

    return tmp;
}

void at26cXX_WriteUint8(uint8_t var, uint16_t adr)
{
	at26cXX_ByteWrite(var, adr);
}

void at26cXX_WriteUint16(uint16_t var, uint16_t adr)
{
	for (uint8_t i = 0; i < 2; i++){
		uint8_t byte = var >> (8 * i);
		at26cXX_ByteWrite(byte, adr + i);
	}
}

void at26cXX_WriteUint32(uint32_t var, uint16_t adr)
{
	for (uint8_t i = 0; i < 4; i++){
		uint8_t byte = var >> (8 * i);
		at26cXX_ByteWrite(byte, adr + i);
	}
}

void at26cXX_WriteMass(uint8_t* var, uint16_t len, uint16_t adr)
{
	for (uint8_t i = 0; i < len; i++){
		at26cXX_ByteWrite(var[i], adr + i);
	}
}

uint8_t at26cXX_ReadUint8(uint16_t adr)
{
	return at26cXX_ByteRead(adr);
}

uint16_t at26cXX_ReadUint16(uint16_t adr)
{
	uint16_t data = 0;

	for (uint8_t i = 0; i < 2; i++){
		data += (at26cXX_ByteRead(adr + i) << (8 * i));
	}

	return data;
}

uint32_t at26cXX_ReadUint32(uint16_t adr)
{
	uint32_t data = 0;

	for (uint8_t i = 0; i < 4; i++){
		data += (at26cXX_ByteRead(adr + i) << (8 * i));
	}

	return data;
}

void at26cXX_ReadMass(uint8_t* var, uint16_t len, uint16_t adr)
{
	for (uint8_t i = 0; i < len; i++){
		var[i] = at26cXX_ByteRead(adr + i);
	}
}
