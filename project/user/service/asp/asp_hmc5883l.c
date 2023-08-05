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

    /* У��ID */
    uint8_t buff[3] = {0};

    msp_i2c_read_bytes(DEV, 10, ADDR_IS_8b, buff, 3);

    const uint8_t id[4] = "H43";

    if(memcmp(id, buff, 3))
    {
        /* ʶ��Ĵ����ȶ�ʧ�� */
        print_log("hmc5883l id register check failed\r\n");
        while(1);
    }

    /* ���üĴ��� */
    uint8_t mode[3] = {0xF0, 0x20, 0x00};

    /* continue ģʽ */
    msp_i2c_write_bytes(DEV, 00, ADDR_IS_8b, mode, 3);

}

/* ��ȡ��λ�Ƕ� */
float asp_hmc5883l_get_direction(void)
{
    static uint8_t buff[6];

    /* ��ȡ�������� */
    msp_i2c_read_bytes(DEV, 03, ADDR_IS_8b, buff, 6);

    static int16_t x, y;

    x = buff[0] << 8 | buff[1];
    y = buff[4] << 8 | buff[5];

    static float dirction;
    dirction = atan2(y, x) / 3.14 * 180 + 180;

    return dirction;
}

/*********************************** END OF FILE ***********************************/
