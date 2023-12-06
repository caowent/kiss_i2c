/**
  ******************************************************************************
  * @file    font
  * @author  曹文涛
  * @date    2023-04-07
  * @brief   中英文字模
  *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __asp_font_H
#define __asp_font_H

#include "common.h"

extern const char font3216[][64];

typedef struct
{
    char index[2];
    
    char font[128];
}chinese3232_typ;

extern const chinese3232_typ chinese3232[];

#define CHINESE3232_SIZE (4)

#endif /* __asp_font_H */

/*********************************** END OF FILE ***********************************/
