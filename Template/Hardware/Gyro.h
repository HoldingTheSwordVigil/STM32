#ifndef __GYRO_H
#define __GYRO_H

#include "MY_IIC.h"

/**定义使用的陀螺仪
 * MPU6050_Gyro
 * JY901_Gyro
 */
#define JY901_Gyro

/********************************************************************************************************/
#ifdef MPU6050_Gyro

//****************************************
// 定义MPU6050内部地址
//****************************************
#define	MPU6050_SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	MPU6050_CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	MPU6050_GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	MPU6050_ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

#define	MPU6050Address	        0x68	//IIC从机地址

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(uint8_t *Data);
void MPU6050_DisposeData(uint8_t *DataBuff, float *MPU6050_Data);

#elif defined(JY901_Gyro)

#define SAVE 		0x00	//保存当前配置
#define CALSW 		0x01	//校准
#define RSW 		0x02	//回传数据内容
#define RRATE		0x03	//回传数据速率
#define BAUD 		0x04	//串口波特率

#define AXOFFSET	0x05	//X轴加速度零偏
#define AYOFFSET	0x06	//Y轴加速度零偏
#define AZOFFSET	0x07	//Z轴加速度零偏
#define GXOFFSET	0x08	//X轴角速度零偏
#define GYOFFSET	0x09	//Y轴角速度零偏
#define GZOFFSET	0x0a	//Z轴角速度零偏
#define HXOFFSET	0x0b	//X轴磁场零偏
#define HYOFFSET	0x0c	//Y轴磁场零偏
#define HZOFFSET	0x0d	//Z轴磁场零偏

#define D0MODE		0x0e	//D0模式
#define D1MODE		0x0f	//D1模式
#define D2MODE		0x10	//D2模式
#define D3MODE		0x11	//D3模式
#define D0PWMH		0x12	//D0PWM高电平宽度
#define D1PWMH		0x13	//D1PWM高电平宽度
#define D2PWMH		0x14	//D2PWM高电平宽度
#define D3PWMH		0x15	//D3PWM高电平宽度
#define D0PWMT		0x16	//D0D0PWM周期
#define D1PWMT		0x17	//D1D0PWM周期
#define D2PWMT		0x18	//D2D0PWM周期
#define D3PWMT		0x19	//D3D0PWM周期

#define IICADDR		0x1a	//IIC地址
#define LEDOFF 		0x1b	//关闭LED指示灯

#define YYMM		0x30	//年、月
#define DDHH		0x31	//日、时
#define MMSS		0x32	//分、秒
#define MS			0x33	//毫秒
#define AX			0x34	//X轴加速度
#define AY			0x35	//Y轴加速度
#define AZ			0x36	//Z轴加速度
#define GX			0x37	//X轴角速度
#define GY			0x38	//Y轴角速度
#define GZ			0x39	//Z轴角速度
#define HX			0x3a	//X轴磁场
#define HY			0x3b	//Y轴磁场
#define HZ			0x3c	//Z轴磁场
#define Roll		0x3d	//X轴角度
#define Pitch		0x3e	//Y轴角度
#define Yaw			0x3f	//Z轴角度
#define TEMP		0x40	//模块温度
#define D0Status	0x41	//D0端口状态
#define D1Status	0x42	//D0端口状态
#define D2Status	0x43	//D0端口状态
#define D3Status	0x44	//D0端口状态
#define PressureL	0x45	//气压低字节
#define PressureH	0x46	//气压高字节
#define HeightL		0x47	//高度低字节
#define HeightH		0x48	//高度高字节
#define LonL		0x49	//经度低字节
#define LonH		0x4a	//经度高字节
#define LatL		0x4b	//纬度低字节
#define LatH		0x4c	//纬度高字节
#define GPSHeight  	0x4d	//GPS高度
#define GPSYAW     	0x4e	//GPS航向角
#define GPSVL		0x4f	//GPS地速低字节
#define GPSVH		0x50	//GPS地速高字节
#define q0			0x51	//四元素Q0
#define q1			0x52	//四元素Q1
#define q2			0x53	//四元素Q1
#define q3			0x54	//四元素Q1

//JY901模块的通信地址默认为0x50
//可以通过串口或者IIC配置更改通信地址，JY901模块地址更改后需要上电复位才生效
//一条IIC总线可以挂多个JY901模块，前提是模块地址不能相同
#define JY901Address  0x50	//定义JY901模块通信地址

void JY901_GetData(uint8_t *Data);
void JY901_DisposeData(uint8_t *DataBuff, float *JY901_Data);

#endif
/*******************************************************************************************************/


#endif
