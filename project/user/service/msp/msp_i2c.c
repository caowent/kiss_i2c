/**
  ******************************************************************************
  * @file    msp_i2c
  * @author  ������
  * @date    2020-2-20
  * @brief   �ṩGPIOģ��I2C����
  *****************************************************************************/

#include "msp_i2c.h"

/* д����λ */
#define I2C_W  (0)
/* ������λ */
#define I2C_R  (1)

/**** ���ſ��ƺ� ****/
#define I2C_SCL_H()     HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET)
#define I2C_SCL_L()     HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET)

#define I2C_SDA_H()     HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET)
#define I2C_SDA_L()     HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET)

#define I2C_SDA_READ()  HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin)

/* ��Ƶ32MHz,������>1us,����<400kHz */
static void i2c_delay(void)
{
    // print_log("1.delay");
    
    // __NOP();
    // __NOP();
    
    // print_log("2.delay");
}

/* ���ڵ㷢���I2C�����ź� */
static void i2c_start(void)
{
    /* ����SCL����ǰ�����SDA�½���ǰ��״̬Ϊ�� */
    I2C_SDA_H();
    i2c_delay();

    /* ��SCLΪ��ʱ��SDA�����½��ر�ʾ�������� */
    I2C_SCL_H();
    i2c_delay();
    
    I2C_SDA_L();
    i2c_delay();
}

/* ���ڵ㷢���I2Cֹͣ�ź� */
static void i2c_stop(void)
{
    /* ����SCL����ǰ�����SDA������ǰ��״̬Ϊ�� */
    I2C_SDA_L();
    i2c_delay();

    /* ��SCLΪ��ʱ��SDA���������ر�ʾ����ֹͣ */
    I2C_SCL_H();
    i2c_delay();
    
    I2C_SDA_H();
    i2c_delay();
}

/* ����һ���ֽ� */
static void send_byte(uint8_t byte)
{
    for(uint32_t i=0; i<8; i++)
    {
        /* ����SCK */
        I2C_SCL_L();
        
        /* ׼������ */
        if(byte&0x80)
        {
            I2C_SDA_H();
        }
        else
        {
            I2C_SDA_L();
        }
        i2c_delay();

        /* ����SCK���������ݣ��ȴ��ӽڵ��ȡ */
        I2C_SCL_H();
        i2c_delay();
        
        byte <<= 1;
    }
}

/* ��ȡһ���ֽ� */
static uint8_t read_byte(void)
{
    uint8_t byte = 0;
    
    /* �ͷ�SDA��SDA����Ȩ���ɴӽڵ� */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();

    for(uint32_t i=0; i<8; i++)
    {
        byte <<= 1;
        
        /* ����SCK���ȴ��ӽڵ�д������ */
        I2C_SCL_L();
        i2c_delay();
        
        /* ����SCK���������� */
        I2C_SCL_H();
        i2c_delay();
        
        /* ��ȡ���� */
        if(I2C_SDA_READ())
        {
            byte |= 0x01;
        }
    }
    
    return byte;
}

/* �ȴ��ӽڵ�ack */
static uint8_t wait_ack(void)
{
    uint8_t ack = 0;
    
    /* �ͷ�SDA���ȴ�ack */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
    
    /* ����SCK���������� */
    I2C_SCL_H();
    i2c_delay();
    
    /* ��ȡack */
    ack = I2C_SDA_READ();
    
    /* ֪ͨ�豸�ͷ�SDA */
    I2C_SCL_L();
    i2c_delay();
    
    return ack;
}

/* ��ӽڵ㷢��ack */
static void send_ack(void)
{
    /* ����SCK */
    I2C_SCL_L();
    
    /* ����ack */
    I2C_SDA_L();
    i2c_delay();

    /* ����SCK���������ݣ��ȴ��ӽڵ��ȡ */
    I2C_SCL_H();
    i2c_delay();
    
    /* �ͷ�SDA */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
}

/* ��ӽڵ㷢��nack */
static void send_nack(void)
{
    /* ����SCK */
    I2C_SCL_L();
    
    /* ����nack */
    I2C_SDA_H();
    i2c_delay();

    /* ����SCK���������ݣ��ȴ��ӽڵ��ȡ */
    I2C_SCL_H();
    i2c_delay();
    
    /* �ͷ�SDA */
    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
}

/* i2c��ʼ�� */
void msp_i2c_init(void)
{
    /* �˴������ʼ��GPIO,cube���Զ����� */
    
    /* �ͷ����� */
    i2c_stop();
}

/* У��ӽڵ�
����0��ʾ�豸���� */
uint8_t msp_i2c_check(uint8_t dev)
{
    uint8_t ack = 0;
    
    /* I2C���� */
    i2c_start();
    
    /* ���͵�ַ */
    send_byte(dev|I2C_W);
    
    /* ��ȡack */
    ack = wait_ack();
    
    /* I2Cֹͣ */
    i2c_stop();
    
    return ack;
}

/* ���ڵ�д���� */
uint8_t msp_i2c_write_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b, 
uint8_t* buf, uint16_t len)
{
    uint8_t err = 0;
    
    /* I2C���� */
    i2c_start();
    
    /* �����豸��ַ */
    send_byte(dev|I2C_W);
    
    /* �ȴ�ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* ����Ĵ�����8λ��ַ */
    if(addr_is_8b)
    {
        /* ���ͼĴ�����ַ */
        send_byte(addr);
    }
    else
    /* ����Ĵ�����16λ��ַ */
    {
        /* �ȷ���8λ��ַ */
        send_byte(addr>>8);
        
        /* �ȴ�ack */
        if(wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }
        
        /* �ٷ���8λ��ַ */
        send_byte(addr);
    }
    
    /* �ȴ�ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* �������� */
    for(int i=0; i<len; i++)
    {
        /* ���͵����ֽ� */
        send_byte(*(buf+i));
        
        /* �ȴ�ack */
        if(wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }
    }
    
LABEL_END:
    /* I2Cֹͣ */
    i2c_stop();
    
    return err;
}

/* ���ڵ������ */
uint8_t msp_i2c_read_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b, 
uint8_t* buf, uint16_t len)
{
    uint8_t err = 0;
    
    /* I2C���� */
    i2c_start();
    
    /* �����豸��ַ */
    send_byte(dev|I2C_W);
    
    /* �ȴ�ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* ����Ĵ�����8λ��ַ */
    if(addr_is_8b)
    {
        /* ���ͼĴ�����ַ */
        send_byte(addr);
    }
    else
    /* ����Ĵ�����16λ��ַ */
    {
        /* �ȷ���8λ��ַ */
        send_byte(addr>>8);
        
        /* �ȴ�ack */
        if(wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }
    
        /* �ٷ���8λ��ַ */
        send_byte(addr);
    }
    
    /* �ȴ�ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* I2C���� */
    i2c_start();
    
    /* �л������ڵ��ȡģʽ */
    send_byte(dev|I2C_R);
    
    /* �ȴ�ack */
    if(wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }
    
    /* ��ȡ���� */
    for(int i=0; i<(len-1); i++)
    {
        /* ��ȡ�����ֽ� */
        *(buf+i) = read_byte();
        
        /* ����ack */
        send_ack();
    }
    
    /* ��ȡ���һ���ֽ� */
    *(buf+len-1) = read_byte();
    /* ����nack��������ȡ */
    send_nack();
    
LABEL_END:
    /* I2Cֹͣ */
    i2c_stop();
    
    return err;
}


/*********************************** END OF FILE ***********************************/
