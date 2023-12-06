/**
 ******************************************************************************
 * @file    asp_hmc5883l(qmc��hmc ѡ��һ����)
 * @author  ������
 * @date    2023-04-11
 * @brief   �ṩhmc5883l ����������������
 *****************************************************************************/

#include "asp_hmc5883l.h"
#include "msp_i2c.h"
#include "math.h"

/* hmc5883l �豸��ַ */
#define DEV (0x1A)

/* hmc5883l��ʼ�� */
void asp_hmc5883l_init(void)
{
    /* i2c��ʼ�� */
    msp_i2c_init();

    /* У�� hmc5883l ͨѶ */
    if (msp_i2c_check(DEV))
    {
        print_log("can't find hmc5883l\r\n");
        while (1)
            ;
    }

    /* ���üĴ��� */
    uint8_t mode = 0x0D;

    msp_i2c_write_bytes(DEV, 9, ADDR_IS_8b, &mode, 1);
}

/* ��ȡ��λ�Ƕ� */
float asp_hmc5883l_get_direction(void)
{
    uint8_t buff[6];

    /* ��ȡ�������� */
    msp_i2c_read_bytes(DEV, 00, ADDR_IS_8b, buff, 6);

    int16_t x, y;

    x = buff[1] << 8 | buff[0];
    y = buff[3] << 8 | buff[2];

    float dirction;

    dirction = atan2(y, x) / 3.14 * 180 + 180;

    if (dirction < 0)
        dirction += 360;

    /* ����ģʽ */
    uint8_t mode = 0x01;

    msp_i2c_write_bytes(DEV, 02, ADDR_IS_8b, &mode, 1);

    return dirction;
}

/*********************************** END OF FILE ***********************************/
