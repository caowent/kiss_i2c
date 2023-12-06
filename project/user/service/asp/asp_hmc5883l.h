/**
 ******************************************************************************
 * @file    asp_hmc5883l
 * @author  曹文涛
 * @date    2023-04-11
 * @brief   提供hmc5883l 电子罗盘驱动服务
 *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __asp_hmc5883l_H
#define __asp_hmc5883l_H

#include "common.h"

/* hmc5883l初始化 */
void asp_hmc5883l_init(void);

/* 获取方位角度 */
float asp_hmc5883l_get_direction(void);

#endif /* __asp_hmc5883l_H */

/*********************************** END OF FILE ***********************************/
