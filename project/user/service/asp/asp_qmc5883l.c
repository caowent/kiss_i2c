/**
 ******************************************************************************
 * @file    asp_hmc5883l(qmc和hmc 选其一编译)
 * @author  曹文涛
 * @date    2023-04-11
 * @brief   提供hmc5883l 电子罗盘驱动服务
 *****************************************************************************/

#include "asp_hmc5883l.h"
#include "msp_i2c.h"
#include "math.h"

/* hmc5883l 设备地址 */
#define DEV (0x1A)

/* hmc5883l初始化 */
void asp_hmc5883l_init(void)
{
    /* i2c初始化 */
    msp_i2c_init();

    /* 校验 hmc5883l 通讯 */
    if (msp_i2c_check(DEV))
    {
        print_log("can't find hmc5883l\r\n");
        while (1)
            ;
    }

    /* 配置寄存器 */
    uint8_t mode = 0x0D;

    msp_i2c_write_bytes(DEV, 9, ADDR_IS_8b, &mode, 1);
}

/* 获取方位角度 */
float asp_hmc5883l_get_direction(void)
{
    uint8_t buff[6];

    /* 读取轴向数据 */
    msp_i2c_read_bytes(DEV, 00, ADDR_IS_8b, buff, 6);

    int16_t x, y;

    x = buff[1] << 8 | buff[0];
    y = buff[3] << 8 | buff[2];

    float dirction;

    dirction = atan2(y, x) / 3.14 * 180 + 180;

    if (dirction < 0)
        dirction += 360;

    /* 单次模式 */
    uint8_t mode = 0x01;

    msp_i2c_write_bytes(DEV, 02, ADDR_IS_8b, &mode, 1);

    return dirction;
}

/*********************************** END OF FILE ***********************************/
