/**
 ******************************************************************************
 * @file    asp_hmc5883l
 * @author  曹文涛
 * @date    2023-04-11
 * @brief   提供hmc5883l 电子罗盘驱动服务
 *****************************************************************************/

#include "asp_hmc5883l.h"
#include "msp_i2c.h"
#include "math.h"

/* hmc5883l 设备地址 */
#define DEV (0x3C)

/* hmc5883l初始化 */
void asp_hmc5883l_init(void)
{
    /* i2c初始化 */
    msp_i2c_init();

    /* 校验 hmc5883l 通讯 */
    if (msp_i2c_check(DEV))
    {
        print_log("can't find hmc5883l\r\n");
        while(1);
    }
}

/* 获取方位角度 */
float asp_hmc5883l_get_direction(void)
{
    static uint8_t buff[13];

    /* 读取轴向数据 */
    msp_i2c_read_bytes(DEV, 0x00, ADDR_IS_8b, buff, 13);

    int x = buff[3] << 8 | buff[4];
    int y = buff[7] << 8 | buff[8];

    float dirction = atan2(y, x) / 3.14 * 180 + 180;

    return dirction;
}

/*********************************** END OF FILE ***********************************/
