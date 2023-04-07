/**
  ******************************************************************************
  * @file    msp_i2c
  * @author  曹文涛
  * @date    2020-2-20
  * @brief   提供GPIO模拟I2C驱动
  *****************************************************************************/

#include "msp_i2c.h"

/* 写控制位 */
#define I2C_W  (0)
/* 读控制位 */
#define I2C_R  (1)

/**** 引脚控制宏 ****/
#define I2C_SCL_H()     HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET)
#define I2C_SCL_L()     HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET)

#define I2C_SDA_H()     HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET)
#define I2C_SDA_L()     HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET)

#define I2C_SDA_READ()  HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin)

/* 主频32MHz,半周期>1us,速率<400kHz */
static void i2c_delay(void)
{
    // print_log("1.delay");
    
    // __NOP();
    // __NOP();
    
    // print_log("2.delay");
}

/* 主节点发起的I2C启动信号 */
static void i2c_start(void)
{
    /* 需在SCL拉高前，输出SDA下降沿前置状态为高 */
    I2C_SDA_H();
    i2c_delay();

    /* 当SCL为高时，SDA产生下降沿表示总线启动 */
    I2C_SCL_H();
    i2c_delay();
    
    I2C_SDA_L();
    i2c_delay();
}

/* 主节点发起的I2C停止信号 */
static void i2c_stop(void)
{
    /* 需在SCL拉高前，输出SDA上升沿前置状态为低 */
    I2C_SDA_L();
    i2c_delay();

    /* 当SCL为高时，SDA产生上升沿表示总线停止 */
    I2C_SCL_H();
    i2c_delay();
    
    I2C_SDA_H();
    i2c_delay();
}

/* 发送一个字节 */
static void send_byte(uint8_t byte)
{
    for(uint32_t i=0; i<8; i++)
    {
        /* 拉低SCK */
        I2C_SCL_L();
        
        /* 准备数据 */
        if(byte&0x80)
        {
            I2C_SDA_H();
        }
        else
        {
            I2C_SDA_L();
        }
        i2c_delay();

        /* 拉高SCK，锁存数据，等待从节点读取 */
        I2C_SCL_H();
        i2c_delay();
        
        byte <<= 1;
    }
}

/* 读取一个字节 */
static uint8_t read_byte(void)
{
    uint8_t byte = 0;
    
    /* 释放SDA，SDA控制权交由从节点 */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();

    for(uint32_t i=0; i<8; i++)
    {
        byte <<= 1;
        
        /* 拉低SCK，等待从节点写入数据 */
        I2C_SCL_L();
        i2c_delay();
        
        /* 拉高SCK，锁存数据 */
        I2C_SCL_H();
        i2c_delay();
        
        /* 读取数据 */
        if(I2C_SDA_READ())
        {
            byte |= 0x01;
        }
    }
    
    return byte;
}

/* 等待从节点ack */
static uint8_t wait_ack(void)
{
    uint8_t ack = 0;
    
    /* 释放SDA，等待ack */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
    
    /* 拉高SCK，锁存数据 */
    I2C_SCL_H();
    i2c_delay();
    
    /* 获取ack */
    ack = I2C_SDA_READ();
    
    /* 通知设备释放SDA */
    I2C_SCL_L();
    i2c_delay();
    
    return ack;
}

/* 向从节点发送ack */
static void send_ack(void)
{
    /* 拉低SCK */
    I2C_SCL_L();
    
    /* 生成ack */
    I2C_SDA_L();
    i2c_delay();

    /* 拉高SCK，锁存数据，等待从节点读取 */
    I2C_SCL_H();
    i2c_delay();
    
    /* 释放SDA */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
}

/* 向从节点发送nack */
static void send_nack(void)
{
    /* 拉低SCK */
    I2C_SCL_L();
    
    /* 生成nack */
    I2C_SDA_H();
    i2c_delay();

    /* 拉高SCK，锁存数据，等待从节点读取 */
    I2C_SCL_H();
    i2c_delay();
    
    /* 释放SDA */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
}

/* i2c初始化 */
void msp_i2c_init(void)
{
    /* 此处无需初始化GPIO,cube已自动生成 */
    
    /* 释放总线 */
    i2c_stop();
}

/* 校验从节点
返回0表示设备正常 */
uint8_t msp_i2c_check(uint8_t dev)
{
    uint8_t ack = 0;
    
    /* I2C启动 */
    i2c_start();
    
    /* 发送地址 */
    send_byte(dev|I2C_W);
    
    /* 获取ack */
    ack = wait_ack();
    
    /* I2C停止 */
    i2c_stop();
    
    return ack;
}

/* 主节点写数据 */
uint8_t msp_i2c_write_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b, 
uint8_t* buf, uint16_t len)
{
    uint8_t err = 0;
    
    /* I2C启动 */
    i2c_start();
    
    /* 发送设备地址 */
    send_byte(dev|I2C_W);
    
    /* 等待ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* 如果寄存器是8位地址 */
    if(addr_is_8b)
    {
        /* 发送寄存器地址 */
        send_byte(addr);
    }
    else
    /* 如果寄存器是16位地址 */
    {
        /* 先发高8位地址 */
        send_byte(addr>>8);
        
        /* 等待ack */
        if(wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }
        
        /* 再发低8位地址 */
        send_byte(addr);
    }
    
    /* 等待ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* 发送数据 */
    for(int i=0; i<len; i++)
    {
        /* 发送单个字节 */
        send_byte(*(buf+i));
        
        /* 等待ack */
        if(wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }
    }
    
LABEL_END:
    /* I2C停止 */
    i2c_stop();
    
    return err;
}

/* 主节点读数据 */
uint8_t msp_i2c_read_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b, 
uint8_t* buf, uint16_t len)
{
    uint8_t err = 0;
    
    /* I2C启动 */
    i2c_start();
    
    /* 发送设备地址 */
    send_byte(dev|I2C_W);
    
    /* 等待ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* 如果寄存器是8位地址 */
    if(addr_is_8b)
    {
        /* 发送寄存器地址 */
        send_byte(addr);
    }
    else
    /* 如果寄存器是16位地址 */
    {
        /* 先发高8位地址 */
        send_byte(addr>>8);
        
        /* 等待ack */
        if(wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }
    
        /* 再发低8位地址 */
        send_byte(addr);
    }
    
    /* 等待ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* I2C启动 */
    i2c_start();
    
    /* 切换成主节点读取模式 */
    send_byte(dev|I2C_R);
    
    /* 等待ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* 读取数据 */
    for(int i=0; i<(len-1); i++)
    {
        /* 读取单个字节 */
        *(buf+i) = read_byte();
        
        /* 发送ack */
        send_ack();
    }
    
    /* 读取最后一个字节 */
    *(buf+len-1) = read_byte();
    /* 发送nack，结束读取 */
    send_nack();
    
LABEL_END:
    /* I2C停止 */
    i2c_stop();
    
    return err;
}


/*********************************** END OF FILE ***********************************/
