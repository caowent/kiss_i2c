/**
 ******************************************************************************
 * @file    asp_hmc5883l
 * @author  ������
 * @date    2023-04-11
 * @brief   �ṩhmc5883l ����������������
 *****************************************************************************/

#include "asp_hmc5883l.h"
#include "msp_i2c.h"
#include "math.h"

/* hmc5883l �豸��ַ */
#define DEV (0x3C)

/* hmc5883l��ʼ�� */
void asp_hmc5883l_init(void)
{
    /* i2c��ʼ�� */
    msp_i2c_init();

    /* У�� hmc5883l ͨѶ */
    if (msp_i2c_check(DEV))
    {
        print_log("can't find hmc5883l\r\n");
        while(1);
    }
}

/* ��ȡ��λ�Ƕ� */
float asp_hmc5883l_get_direction(void)
{
    static uint8_t buff[13];

    /* ��ȡ�������� */
    msp_i2c_read_bytes(DEV, 0x00, ADDR_IS_8b, buff, 13);

    int x = buff[3] << 8 | buff[4];
    int y = buff[7] << 8 | buff[8];

    float dirction = atan2(y, x) / 3.14 * 180 + 180;

    return dirction;
}

/*********************************** END OF FILE ***********************************/
