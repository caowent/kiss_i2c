/**
 ******************************************************************************
 * @file    asp_ssd1306
 * @author  曹文涛
 * @date    2023-04-05
 * @brief   提供SSD1306 LED屏幕驱动服务
 *****************************************************************************/

#include "asp_ssd1306.h"
#include "msp_i2c.h"
#include "font.h"

/* ssd1306 设备地址 */
#define DEV (0x78)

static uint8_t OLED_GRAM[8][128];

static void ssd1306_draw_point(int x, int y, char point);

/* 写入命令 */
static void ssd1306_write_cmd(uint8_t cmd)
{
    msp_i2c_write_bytes(DEV, 0x00, ADDR_IS_8b, &cmd, 1);
}

/* 写了显存 */
static void ssd1306_write_data(uint8_t *data, int num)
{
    msp_i2c_write_bytes(DEV, 0x40, ADDR_IS_8b, data, num);
}

static void SSD1306_CMD_sequence(void)
{
    ssd1306_write_cmd(0xAE); /*display off*/
    ssd1306_write_cmd(0x20); /*Set Memory Addressing Mode*/
    ssd1306_write_cmd(0x02); /*#Page Addressing Mode*/
    ssd1306_write_cmd(0xB0); /*set page address*/
    ssd1306_write_cmd(0xC8); /*Com scan direction*/
    ssd1306_write_cmd(0x00); /*set lower column address*/
    ssd1306_write_cmd(0x10); /*set higher column address*/
    ssd1306_write_cmd(0x40); /*set display start line*/
    ssd1306_write_cmd(0x81); /*contract control*/
    ssd1306_write_cmd(0xFF);
    ssd1306_write_cmd(0xA1); /*set segment remap*/
    ssd1306_write_cmd(0xA6); /*normal / reverse*/
    ssd1306_write_cmd(0xA8); /*multiplex ratio*/
    ssd1306_write_cmd(0x3F); /*#duty = 1/32*/
    ssd1306_write_cmd(0xD3); /*set display offset*/
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0xD5); /*set osc division*/
    ssd1306_write_cmd(0x80);
    ssd1306_write_cmd(0xD9); /*set pre-charge period*/
    ssd1306_write_cmd(0xF1);
    ssd1306_write_cmd(0xDA); /*#set COM pins*/
    ssd1306_write_cmd(0x12);
    ssd1306_write_cmd(0xDB); /*set vcomh*/
    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(0x8D); /*set charge pump enable*/
    ssd1306_write_cmd(0x14);
}

/* ssd1306 初始化 */
void asp_ssd1306_init(void)
{
    /* i2c初始化 */
    msp_i2c_init();

    /* 校验 ssd1306 通讯 */
    if (msp_i2c_check(DEV))
    {
        while(1);
        print_log("can't find ssd1306\r\n");
    }

    /* 初始化命令序列 */
    SSD1306_CMD_sequence();

    ssd1306_clear_screen();

    ssd1306_write_cmd(0xAF); /*display ON*/
}

/* 刷新屏幕 */
void ssd1306_refresh_gram(void)
{
    for (int i = 0; i < 8; i++)
    {
        ssd1306_write_cmd(0xB0 + i); // 设置行起始地址
        ssd1306_write_cmd(0x00);     // 设置低列起始地址
        ssd1306_write_cmd(0x10);     // 设置高列起始地址

        ssd1306_write_data(OLED_GRAM[i], 128);
    }
}

/* 清除屏幕 */
void ssd1306_clear_screen(void)
{
    memset(OLED_GRAM, 0, 8 * 128);

    ssd1306_refresh_gram();
}

static void ssd1306_draw_point(int x, int y, char point)
{
    uint8_t pos, bit, temp = 0;

    if (x > 127 || y > 63)
    {
        return;
    }
    pos = y / 8;
    bit = y % 8;
    temp = 1 << bit;

    if (point)
    {

        OLED_GRAM[pos][x] |= temp;
    }
    else
    {
        OLED_GRAM[pos][x] &= ~temp;
    }
}

void ssd1306_display_char(int x, int y, char ch)
{
    int i, j, ypos0 = y;
    char temp;

    ch = ch - ' ';
    for (i = 0; i < 64; i++)
    {
        temp = font3216[ch][i];

        for (j = 0; j < 8; j++)
        {
            if (temp & 0x80)
            {
                ssd1306_draw_point(x, y, 1);
            }
            else
            {
                ssd1306_draw_point(x, y, 0);
            }

            temp <<= 1;
            y++;

            if ((y - ypos0) == 32)
            {
                y = ypos0;
                x++;
                break;
            }
        }
    }
}

void ssd1306_display_chinese(int x, int y, char *chs)
{
    int i = 0;

    /* 从字库查询 */
    while (strncmp(chs, chinese3232[i].index, 2))
    {
        i++;
        if (i >= CHINESE3232_SIZE)
            break;
    }

    int pos = i;

    {
        int i, j, ypos0 = y;
        char temp;

        for (i = 0; i < 128; i++)
        {
            temp = chinese3232[pos].font[i];

            for (j = 0; j < 8; j++)
            {
                if (temp & 0x80)
                {
                    ssd1306_draw_point(x, y, 1);
                }
                else
                {
                    ssd1306_draw_point(x, y, 0);
                }

                temp <<= 1;
                y++;

                if ((y - ypos0) == 32)
                {
                    y = ypos0;
                    x++;
                    break;
                }
            }
        }
    }
}

void ssd1306_display_string(int x, int y, char *str)
{
    while (*str != '\0')
    {
        /* 英文字母 */
        if (*str <= 0x7F)
        {
            ssd1306_display_char(x, y, *str);
            x += 16;
            str++;
        }
        else
        /* 中文 */
        {
            ssd1306_display_chinese(x, y, str);
            x += 32;
            str += 2;
        }
    }

    ssd1306_refresh_gram();
}

/*********************************** END OF FILE ***********************************/
