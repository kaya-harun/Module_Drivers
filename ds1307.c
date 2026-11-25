#include "ds1307.h"

static uint8_t BinarytoBCD(uint8_t value);

uint8_t DS1307_Init(I2C_Handle *handle)
{
	uint8_t data[] = {0x0, 0x0};
	uint8_t rtn[1];

	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR); //Enable clock

	I2C_Transfer_Pol(handle, (uint8_t*)0x0, 1, DS1307_SLAVE_ADDR); //Send ptr
	I2C_Receive_Pol(handle, rtn, 1, DS1307_SLAVE_ADDR); //Read clock state

	return rtn[0];
}

void DS1307_Set_Time(I2C_Handle *handle, DS1307_Time *time)
{
	uint8_t data[2];

	//Send second
	data[1] = BinarytoBCD(time->sec);
	data[1] &= ~(0x1U << 7);
	data[0] = 0x0U;
	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR);

	//Send minute
	data[1] = BinarytoBCD(time->min);
	data[0] = 0x1U;
	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR);

	//Send hour
	data[1] = BinarytoBCD(time->hr);
	if(time->timeFrmt == DS1307_Time_Format_24)
		data[1] &= ~(0x1U << 6);
	else
	{
		data[1] |= 0x1U << 6;
		data[1] = (time->timeFrmt == DS1307_Time_Format_12PM) ? (data[1] | (0x1U << 5)) : (data[1] & ~(0x1U << 5));
		data[0] = 0x2U;
	}
	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR);

	//Send day
	data[1] = BinarytoBCD(time->day);
	data[0] = 0x3U;
	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR);

	//Send date
	data[1] = BinarytoBCD(time->date);
	data[0] = 0x4U;
	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR);

	//Send mount
	data[1] = BinarytoBCD(time->month);
	data[0] = 0x5U;
	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR);

	//Send year
	data[1] = BinarytoBCD(time->year);
	data[0] = 0x6U;
	I2C_Transfer_Pol(handle, data, sizeof(data), DS1307_SLAVE_ADDR);
}

static uint8_t BinarytoBCD(uint8_t value)
{
	uint8_t a, b;

	if(value >= 10)
	{
		a = value / 10;
		b = value % 10;
		value = (a << 4) | b;
	}

	return value;
}
