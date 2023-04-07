/**
  ******************************************************************************
  * @file    msp_i2c
  * @author  曹文涛
  * @date    2020-2-20
  * @brief   提供GPIO模拟I2C驱动
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MSP_I2C_PROBE_H
#define __MSP_I2C_PROBE_H

#include "common.h"

#define ADDR_IS_8b  (1)
#define ADDR_IS_16b (0)

/* i2c初始化 */
void msp_i2c_init(void);

/* 校验从节点 */
uint8_t msp_i2c_check(uint8_t dev);

/* 主节点写数据 */
uint8_t msp_i2c_write_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b, 
uint8_t* buf, uint16_t len);

/* 主节点读数据 */
uint8_t msp_i2c_read_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b, 
uint8_t* buf, uint16_t len);

#endif /* __MSP_I2C_PROBE_H */

/*********************************** END OF FILE ***********************************/
