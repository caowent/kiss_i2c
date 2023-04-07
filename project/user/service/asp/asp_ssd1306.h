/**
  ******************************************************************************
  * @file    asp_ssd1306
  * @author  曹文涛
  * @date    2023-04-05
  * @brief   提供SSD1306 LED屏幕驱动服务
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __asp_ssd1306_H
#define __asp_ssd1306_H

#include "common.h"

/* ssd1306初始化 */
void asp_ssd1306_init(void);

/* 刷新屏幕 */
void ssd1306_refresh_gram(void);

/* 清除屏幕 */
void ssd1306_clear_screen(void);

void ssd1306_display_char(uint8_t x, uint8_t y, uint8_t ch);

#endif /* __asp_ssd1306_H */

/*********************************** END OF FILE ***********************************/
