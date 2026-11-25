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
 * @brief DS1307 module initialization API
 *
 * @retval 0 	Succesfull initialization
 * @retval 1 	Unsuccesfull initialization
 */
uint8_t DS1307_Init(I2C_Handle *handle);

#endif /* DS1307_H_ */
