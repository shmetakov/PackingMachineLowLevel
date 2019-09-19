#include "global.h"
#include "param.h"
#include "at26cXX.h"

uint32_t paramWigth = 0;
uint32_t paramLength = 0;

void readParamFromEE()
{
	paramTemp_1 = at26cXX_ReadUint8(PAMAR_TEMP_1_ADR);
	paramTemp_2 = at26cXX_ReadUint8(PAMAR_TEMP_2_ADR);
	paramTemp_3 = at26cXX_ReadUint8(PAMAR_TEMP_3_ADR);
	paramWigth = at26cXX_ReadUint32(PAMAR_WIDTH_ADR);
	paramLength = at26cXX_ReadUint32(PAMAR_LENGTH_ADR);
}

void saveParamToEE()
{
	at26cXX_WriteUint8(paramTemp_1, PAMAR_TEMP_1_ADR);
	at26cXX_WriteUint8(paramTemp_2, PAMAR_TEMP_2_ADR);
	at26cXX_WriteUint8(paramTemp_3, PAMAR_TEMP_3_ADR);
	at26cXX_WriteUint32(paramWigth, PAMAR_WIDTH_ADR);
	at26cXX_WriteUint32(paramLength, PAMAR_LENGTH_ADR);
}
