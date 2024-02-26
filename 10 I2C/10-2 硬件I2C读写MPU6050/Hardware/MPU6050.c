#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT) // �ȴ��¼���ʱ����
{
	uint32_t Timeout;
	Timeout = 10000;									// ��ʱ����ʱ��
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	
	{
		Timeout --;										
		if (Timeout == 0)								
		{
			
			break;										
		}
	}
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	uint32_t Timeout;
	
	I2C_GenerateSTART(I2C2, ENABLE); // S ��ʼ����
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // EV5 �¼�
	
	// ����д��ַ: �Դ�Ӧ��
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // EV6 �¼�
	
	// ��������
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // EV8 �¼�
	
	// ��������
	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // EV8_2 �¼�
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Byte;
	
	// дʱ��
	I2C_GenerateSTART(I2C2, ENABLE); // S ��ʼ����
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // EV5 �¼�
	
	// ���͵�ַ: �Դ�Ӧ��
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // EV6 �¼�
	
	// ��������
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // EV8_2 �¼�
	
	
	// ��ʱ��
	I2C_GenerateSTART(I2C2, ENABLE); // S ��ʼ����
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // EV5 �¼�
	
	// ���Ͷ���ַ: �Դ�Ӧ��
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // EV6 �¼�
	
	// ���յ����һ���ֽ�����ǰ��Ҫȡ��Ӧ��
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // EV7 �¼�
	Byte = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE); // ʹ��Ӧ��(Ϊ�˼��ݶ��ֽڽ���)
	
	return Byte;
}

void MPU6050_Init()
{
	// ���� APB1 �ϵ� I2C2 �� APB2 �ϵ� GPIOB �� RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// GPIO ��ʼ�� PB10, PB11
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD ;  // ���ÿ�©���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	// I2C2 ��ʼ��
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable; // �Ƿ�Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit; // 7λ��ַ
	I2C_InitStructure.I2C_ClockSpeed=50000; // I2C �ٶ�: 50KHz
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2; // SCL ռ�ձ� 0:1 = 2:1
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C; // I2C ģʽ
	I2C_InitStructure.I2C_OwnAddress1=0x00; // �޹�: ��Ƭ����Ϊ�ӻ��ĵ�ַ
	I2C_Init(I2C2, &I2C_InitStructure);
	
	// ʹ�� I2C2
	I2C_Cmd(I2C2, ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 10 ��Ƶ
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID()
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
