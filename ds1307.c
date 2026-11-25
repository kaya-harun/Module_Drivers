#include "ds1307.h"

static uint8_t BinarytoBCD(uint8_t value);
static uint8_t BCDtoBinary(uint8_t value);


uint8_t DS1307_Init(I2C_Handle *handle)
{
    uint8_t reg = 0x00;
    uint8_t rtn;
    uint8_t data[2];

    // Read seconds register
    I2C_Transfer_Pol(handle, &reg, 1, DS1307_SLAVE_ADDR);
    I2C_Receive_Pol(handle, &rtn, 1, DS1307_SLAVE_ADDR);

    // Clear CH bit without resetting seconds
    data[0] = 0x00;
    data[1] = rtn & 0x7F;

    I2C_Transfer_Pol(handle, data, sizeof(data),
                     DS1307_SLAVE_ADDR);

    // Verify CH bit
    I2C_Transfer_Pol(handle, &reg, 1, DS1307_SLAVE_ADDR);
    I2C_Receive_Pol(handle, &rtn, 1, DS1307_SLAVE_ADDR);

    return (rtn >> 7) & 0x01;
}


void DS1307_Set_Time(I2C_Handle *handle, DS1307_Time *time)
{
    uint8_t data[8];

    // Start register address
    data[0] = 0x00;

    // Seconds - CH bit cleared
    data[1] = BinarytoBCD(time->sec) & 0x7F;

    // Minutes
    data[2] = BinarytoBCD(time->min);

    // Hours
    data[3] = BinarytoBCD(time->hr);

    if(time->timeFrmt == DS1307_Time_Format_24)
    {
        data[3] &= ~(1U << 6);
    }
    else
    {
        data[3] |= (1U << 6);

        if(time->timeFrmt == DS1307_Time_Format_12PM)
        {
            data[3] |= (1U << 5);
        }
        else
        {
            data[3] &= ~(1U << 5);
        }
    }

    // Day, date, month and year
    data[4] = BinarytoBCD(time->day);
    data[5] = BinarytoBCD(time->date);
    data[6] = BinarytoBCD(time->month);
    data[7] = BinarytoBCD(time->year);

    // Write all time registers
    I2C_Transfer_Pol(handle, data, sizeof(data),
                     DS1307_SLAVE_ADDR);
}


void DS1307_Get_Time(I2C_Handle *handle, DS1307_Time *time)
{
    uint8_t reg = 0x00;
    uint8_t data[7];

    // Set register pointer
    I2C_Transfer_Pol(handle, &reg, 1, DS1307_SLAVE_ADDR);

    // Read all time registers
    I2C_Receive_Pol(handle, data, sizeof(data),
                    DS1307_SLAVE_ADDR);

    // Seconds
    time->sec = BCDtoBinary(data[0] & 0x7F);

    // Minutes
    time->min = BCDtoBinary(data[1] & 0x7F);

    // Hours
    if(data[2] & (1U << 6))
    {
        time->timeFrmt = (data[2] & (1U << 5))
                       ? DS1307_Time_Format_12PM
                       : DS1307_Time_Format_12AM;

        time->hr = BCDtoBinary(data[2] & 0x1F);
    }
    else
    {
        time->timeFrmt = DS1307_Time_Format_24;
        time->hr = BCDtoBinary(data[2] & 0x3F);
    }

    // Day, date, month and year
    time->day   = (DS1307_Day)(data[3] & 0x07);
    time->date  = BCDtoBinary(data[4] & 0x3F);
    time->month = BCDtoBinary(data[5] & 0x1F);
    time->year  = BCDtoBinary(data[6]);
}


static uint8_t BinarytoBCD(uint8_t value)
{
    return ((value / 10) << 4) | (value % 10);
}


static uint8_t BCDtoBinary(uint8_t value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}
