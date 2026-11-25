/**
 * @file ds1307.h
 * @brief DS1307 module header
 * @author Harun Kaya
 * @date 25.11.2026
 */

#ifndef DS1307_H_
#define DS1307_H_

#include "stm32f401re.h"

#define DS1307_SLAVE_ADDR	0x68U

/**
 * @brief DS1307 time format configurations.
 */
typedef enum
{
		DS1307_Time_Format_12AM,
		DS1307_Time_Format_12PM,
		DS1307_Time_Format_24,
}DS1307_Time_Format;

/**
 * @brief DS1307 Module weekdays.
 */
typedef enum
{
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6,
	SUNDAY = 7,
}DS1307_Day;

/**
 * @brief DS1307 module time structure.
 */
typedef struct DS1307_Time DS1307_Time;
struct DS1307_Time
{
	uint8_t sec;
	uint8_t min;
	uint8_t hr;
	DS1307_Day day;
	uint8_t month;
	uint8_t year;
	uint8_t date;
	DS1307_Time_Format timeFrmt;
};

/**
 * @brief DS1307 module initialization API.
 *
 * @retval 0 	Succesfull initialization
 * @retval 1 	Unsuccesfull initialization
 */
uint8_t DS1307_Init(I2C_Handle *handle);

/**
 * @brief DS1307 module set time API.
 */
void DS1307_Set_Time(I2C_Handle *handle, DS1307_Time *time);

/**
 * @brief DS1307 module get time API
 */
void DS1307_Get_Time(I2C_Handle *handle, DS1307_Time *time);

#endif /* DS1307_H_ */
