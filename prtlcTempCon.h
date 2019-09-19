#ifndef PROTOCOL_FOR_TEMP_CON_H
#define PROTOCOL_FOR_TEMP_CON_H

#define SENS_1_BYTE										1
#define SENS_2_BYTE										2
#define SENS_3_BYTE										3
#define TIME_FOR_SET_TEMP								10000

void parsingPackageTempCon();
uint8_t xorTempCon(uint8_t *buf, uint8_t len);
void saveDataTempCon(uint8_t* packege);
void clearBuffer_TempCon_Send();
void cmd_sendTempCon();

#endif
