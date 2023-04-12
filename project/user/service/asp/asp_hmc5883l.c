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

    /* 校验ID */
    uint8_t buff[3] = {0};

    msp_i2c_read_bytes(DEV, 10, ADDR_IS_8b, buff, 3);

    const uint8_t id[4] = "H43";

    if(memcmp(id, buff, 3))
    {
        /* 识别寄存器比对失败 */
        print_log("hmc5883l id register check failed\r\n");
        while(1);
    }

    /* 配置寄存器 */
    uint8_t mode[3] = {0xF0, 0x20, 0x00};

    /* continue 模式 */
    msp_i2c_write_bytes(DEV, 00, ADDR_IS_8b, mode, 3);

}

/* 获取方位角度 */
float asp_hmc5883l_get_direction(void)
{
    static uint8_t buff[6];

    /* 读取轴向数据 */
    msp_i2c_read_bytes(DEV, 03, ADDR_IS_8b, buff, 6);

    static int16_t x, y;

    x = buff[0] << 8 | buff[1];
    y = buff[4] << 8 | buff[5];

    static float dirction;
    dirction = atan2(y, x) / 3.14 * 180 + 180;

    return dirction;
}

/*********************************** END OF FILE ***********************************/
