#ifndef PROTOCOL_FOR_SERVICE_COM_PORT_H
#define PROTOCOL_FOR_SERVICE_COM_PORT_H

#define BUFFER_SERVICE_SIZE								128

#define SERVICE_LEN_BYTE								1
#define SERVICE_CMD_BYTE								2
#define SERVICE_DATA_BYTE								3

void parsingPackageService();
uint8_t xorService_COM(uint8_t *buf, uint8_t len);
void saveDataService(uint8_t* packege);
void clearBuffer_Service_Send();

#endif
