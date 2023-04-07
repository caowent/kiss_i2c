/**
  ******************************************************************************
  * @file    asp_ssd1306
  * @author  ������
  * @date    2023-04-05
  * @brief   �ṩSSD1306 LED��Ļ��������
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __asp_ssd1306_H
#define __asp_ssd1306_H

#include "common.h"

/* ssd1306��ʼ�� */
void asp_ssd1306_init(void);

/* ˢ����Ļ */
void ssd1306_refresh_gram(void);

/* �����Ļ */
void ssd1306_clear_screen(void);

void ssd1306_display_char(uint8_t x, uint8_t y, uint8_t ch);

#endif /* __asp_ssd1306_H */

/*********************************** END OF FILE ***********************************/
