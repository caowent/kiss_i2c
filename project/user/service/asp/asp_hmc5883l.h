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

/* �ź�У׼ */
void asp_hmc5883l_calib(void);

/* ����ʱ�ص� */
void asp_count_down_cb(int count);

#endif /* __asp_hmc5883l_H */

/*********************************** END OF FILE ***********************************/
