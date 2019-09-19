#ifndef AT26CXX_H
#define AT26CXX_H

#define I2C_TIMEOUT					20000
#define I2C_CLOCK_SPEED				10000

void at26cXX_Init();
void at26cXX_WriteUint8(uint8_t var, uint16_t adr);
void at26cXX_WriteUint16(uint16_t var, uint16_t adr);
void at26cXX_WriteUint32(uint32_t var, uint16_t adr);
void at26cXX_WriteMass(uint8_t* var, uint16_t len, uint16_t adr);
void at26cXX_PageWrite(uint8_t* val, uint16_t WriteAddr);
uint8_t at26cXX_ReadUint8(uint16_t adr);
uint16_t at26cXX_ReadUint16(uint16_t adr);
uint32_t at26cXX_ReadUint32(uint16_t adr);
void at26cXX_ReadMass(uint8_t* var, uint16_t len, uint16_t adr);

#endif
