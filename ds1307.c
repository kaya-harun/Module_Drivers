#include "ds1307.h"

uint8_t DS1307_Init(I2C_Handle *handle)
{
	uint8_t data[] = {0x0, 0x0};
	uint8_t rtn[1];

	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR); //Enable clock

	I2C_Transfer_Pol(handle, (uint8_t*)0x0, 1, DS1307_SLAVE_ADDR); //Send ptr
	I2C_Receive_Pol(handle, rtn, 1, DS1307_SLAVE_ADDR); //Read clock state

	return rtn[0];
}
