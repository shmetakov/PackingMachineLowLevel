#ifndef PARAM_H
#define PARAM_H

/*
 *
 * #PARAM: | t1 | t2 | t3 | W1 | W2 | W3 | W4 | L1 | L2 | L3 | L4 |
 * #ADR:   |0000|0001|0002|0003|0004|0005|0006|0007|0008|0009|000A|
 *
 */

#define PAMAR_TEMP_1_ADR							0x0000
#define PAMAR_TEMP_2_ADR							0x0001
#define PAMAR_TEMP_3_ADR							0x0002
#define PAMAR_WIDTH_ADR								0x0003
#define PAMAR_LENGTH_ADR							0x0007

void readParamFromEE();
void saveParamToEE();


#endif
