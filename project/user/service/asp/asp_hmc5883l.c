/**
 ******************************************************************************
 * @file    asp_hmc5883l
 * @author  曹文涛
 * @date    2023-04-11
 * @brief   提供hmc5883l 电子罗盘驱动服务。
 * 注意：传感器要远离金属和磁性物体，否则方位不准。
 *****************************************************************************/

#include "asp_hmc5883l.h"
#include "msp_i2c.h"
#include "math.h"

#define min(x, y) x < y ? x : y
#define max(x, y) x > y ? x : y

/* hmc5883l 设备地址 */
#define DEV (0x3C)

void asp_hmc5883l_calib(void);

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

    /* 校验ID */
    uint8_t buff[3] = {0};

    msp_i2c_read_bytes(DEV, 10, ADDR_IS_8b, buff, 3);

    const uint8_t id[4] = "H43";

    if (memcmp(id, buff, 3))
    {
        /* 识别寄存器比对失败 */
        print_log("hmc5883l id register check failed\r\n");
        while (1)
            ;
    }

    /* 配置寄存器 */
    uint8_t mode[3] = {0xF0, 0x20, 0x01};

    msp_i2c_write_bytes(DEV, 00, ADDR_IS_8b, mode, 3);

    asp_hmc5883l_calib();

}

int16_t Xoffset, Yoffset;

/* 信号校准 */
void asp_hmc5883l_calib(void)
{
    int tick_start = HAL_GetTick();

    uint8_t buff[6];

    int16_t x, y;

    static int16_t Xmin, Xmax;
    static int16_t Ymin, Ymax;

    msp_i2c_read_bytes(DEV, 03, ADDR_IS_8b, buff, 6);

    Xmin = Xmax = buff[0] << 8 | buff[1];
    Ymin = Ymax = buff[4] << 8 | buff[5];

    uint8_t mode = 0x01;

    int count;

    while (1)
    {
        msp_i2c_write_bytes(DEV, 02, ADDR_IS_8b, &mode, 1);

        msp_i2c_read_bytes(DEV, 03, ADDR_IS_8b, buff, 6);

        x = buff[0] << 8 | buff[1];
        y = buff[4] << 8 | buff[5];

        Xmin = min(x, Xmin);
        Xmax = max(x, Xmax);

        Ymin = min(y, Ymin);
        Ymax = max(y, Ymax);

        count = HAL_GetTick() - tick_start;

        if (count > 10 * 1000)
            break;
    }

    Xoffset = (Xmax - Xmin) / 2 - Xmax;
    Yoffset = (Ymax - Ymin) / 2 - Ymax;
}

/* 获取方位角度 */
float asp_hmc5883l_get_direction(void)
{
    uint8_t buff[6];

    /* 读取轴向数据 */
    msp_i2c_read_bytes(DEV, 03, ADDR_IS_8b, buff, 6);

    int16_t x, y;

    x = buff[0] << 8 | buff[1];
    y = buff[4] << 8 | buff[5];

    x += Xoffset;
    y += Yoffset;

    float dirction = atan2(y, x) / 3.14 * 180;

    if (dirction < 0)
        dirction += 360;

    dirction -= 110;

    if (dirction < 0)
        dirction += 360;

    /* 单次模式 */
    uint8_t mode = 0x01;

    msp_i2c_write_bytes(DEV, 02, ADDR_IS_8b, &mode, 1);

    return dirction;
}

/*********************************** END OF FILE ***********************************/
