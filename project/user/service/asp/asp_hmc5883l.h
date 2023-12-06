/**
 ******************************************************************************
 * @file    asp_hmc5883l
 * @author  ������
 * @date    2023-04-11
 * @brief   �ṩhmc5883l ����������������
 *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __asp_hmc5883l_H
#define __asp_hmc5883l_H

#include "common.h"

/* hmc5883l��ʼ�� */
void asp_hmc5883l_init(void);

/* ��ȡ��λ�Ƕ� */
float asp_hmc5883l_get_direction(void);

#endif /* __asp_hmc5883l_H */

/*********************************** END OF FILE ***********************************/
