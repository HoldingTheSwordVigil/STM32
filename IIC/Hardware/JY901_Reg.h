#ifndef __JY901_REG_H
#define __JY901_REG_H

#define SAVE 		0x00	//���浱ǰ����
#define CALSW 		0x01	//У׼
#define RSW 		0x02	//�ش���������
#define RRATE		0x03	//�ش���������
#define BAUD 		0x04	//���ڲ�����

#define AXOFFSET	0x05	//X����ٶ���ƫ
#define AYOFFSET	0x06	//Y����ٶ���ƫ
#define AZOFFSET	0x07	//Z����ٶ���ƫ
#define GXOFFSET	0x08	//X����ٶ���ƫ
#define GYOFFSET	0x09	//Y����ٶ���ƫ
#define GZOFFSET	0x0a	//Z����ٶ���ƫ
#define HXOFFSET	0x0b	//X��ų���ƫ
#define HYOFFSET	0x0c	//Y��ų���ƫ
#define HZOFFSET	0x0d	//Z��ų���ƫ

#define D0MODE		0x0e	//D0ģʽ
#define D1MODE		0x0f	//D1ģʽ
#define D2MODE		0x10	//D2ģʽ
#define D3MODE		0x11	//D3ģʽ
#define D0PWMH		0x12	//D0PWM�ߵ�ƽ���
#define D1PWMH		0x13	//D1PWM�ߵ�ƽ���
#define D2PWMH		0x14	//D2PWM�ߵ�ƽ���
#define D3PWMH		0x15	//D3PWM�ߵ�ƽ���
#define D0PWMT		0x16	//D0D0PWM����
#define D1PWMT		0x17	//D1D0PWM����
#define D2PWMT		0x18	//D2D0PWM����
#define D3PWMT		0x19	//D3D0PWM����

#define IICADDR		0x1a	//IIC��ַ
#define LEDOFF 		0x1b	//�ر�LEDָʾ��

#define YYMM		0x30	//�ꡢ��
#define DDHH		0x31	//�ա�ʱ
#define MMSS		0x32	//�֡���
#define MS			0x33	//����
#define AX			0x34	//X����ٶ�
#define AY			0x35	//Y����ٶ�
#define AZ			0x36	//Z����ٶ�
#define GX			0x37	//X����ٶ�
#define GY			0x38	//Y����ٶ�
#define GZ			0x39	//Z����ٶ�
#define HX			0x3a	//X��ų�
#define HY			0x3b	//Y��ų�
#define HZ			0x3c	//Z��ų�
#define Roll		0x3d	//X��Ƕ�
#define Pitch		0x3e	//Y��Ƕ�
#define Yaw			0x3f	//Z��Ƕ�
#define TEMP		0x40	//ģ���¶�
#define D0Status	0x41	//D0�˿�״̬
#define D1Status	0x42	//D0�˿�״̬
#define D2Status	0x43	//D0�˿�״̬
#define D3Status	0x44	//D0�˿�״̬
#define PressureL	0x45	//��ѹ���ֽ�
#define PressureH	0x46	//��ѹ���ֽ�
#define HeightL		0x47	//�߶ȵ��ֽ�
#define HeightH		0x48	//�߶ȸ��ֽ�
#define LonL		0x49	//���ȵ��ֽ�
#define LonH		0x4a	//���ȸ��ֽ�
#define LatL		0x4b	//γ�ȵ��ֽ�
#define LatH		0x4c	//γ�ȸ��ֽ�
#define GPSHeight  	0x4d	//GPS�߶�
#define GPSYAW     	0x4e	//GPS�����
#define GPSVL		0x4f	//GPS���ٵ��ֽ�
#define GPSVH		0x50	//GPS���ٸ��ֽ�
#define q0			0x51	//��Ԫ��Q0
#define q1			0x52	//��Ԫ��Q1
#define q2			0x53	//��Ԫ��Q1
#define q3			0x54	//��Ԫ��Q1

//JY901ģ���ͨ�ŵ�ַĬ��Ϊ0x50
//����ͨ�����ڻ���IIC���ø���ͨ�ŵ�ַ��JY901ģ���ַ���ĺ���Ҫ�ϵ縴λ����Ч
//һ��IIC���߿��ԹҶ��JY901ģ�飬ǰ����ģ���ַ������ͬ
#define JY901_ADDR  0x50	//����JY901ģ��ͨ�ŵ�ַ



#endif
