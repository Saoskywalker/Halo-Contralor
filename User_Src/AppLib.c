#include "AppLib.h"
#include "stdlib.h"

//music command
const u8 _MusicInit[] = {0XAA, 0X0A, 0X00}; //model Init, check Drive
const u8 _MusicVol[] = {0xAA, 0x13, 0x01, 0x1E}; //Vol sets biggest
u8 _MusicPlay[] = {0xAA, 0x07, 0x02, 0x00, 0x01};
const u8 _MusicStop[] = {0XAA, 0X10, 0X00};

//distinguish command
const u8 _DistInit[] = {0XA0, 0XA0, 0XA0}; //model Init
const u8 _DistInitRe[] = {0X50, 0X50, 0X16};
u8 _DistGroup[] = {0XA9, 0X01, 0X00}; //Select group
u8 _DistGroupRe[] = {0X59, 0X01, 0X00};
u8 _DistStartOnce[] = {0XAA, 0X0A, 0X00}; //start once work, keep on 10s
u8 DistStartOnceRe[] = {0X5A, 0XFF, 0XFF};

//RGB driver
static const u16 RGB_Table[] = {0X0000, 0xF800, 0x07E0, 0x001F};
//RGB565 decode
void RGB_PWM(u8 *i, u8 *p, u8 *g)
{
	static u8 _TimeCnt = 0;
	static u8 _RModRenew = 0, _GModRenew = 0, _BModRenew = 0;
	
	if(++_TimeCnt>=160) //cycle 16ms
	{
		_TimeCnt = 0;
		_RModRenew = *i;
		_GModRenew = *p;
		_BModRenew = *g;
	}
	
	//red
	if(_TimeCnt<_RModRenew)
	{	//OPEN
		LED_RED_PIN = 1;	
	}
	else	//COLSE
	{
		LED_RED_PIN = 0;
	}
	
	//green
	if(_TimeCnt<_GModRenew)
	{	//OPEN
		LED_GREEN_PIN = 1;	
	}
	else	//COLSE
	{
		LED_GREEN_PIN = 0;
	}
	
	//blue
	if(_TimeCnt<_BModRenew)
	{	//OPEN
		LED_BLUE_PIN = 1;	
	}
	else	//COLSE
	{
		LED_BLUE_PIN = 0;
	}
}

void RGB_Decode(const u16 *i)
{
	u8 R, G, B;
	
	R = *i>>11;
	G = (*i&0x07E0)>>5;
	B = *i&0x001F;
	
	RGB_Renew(R, G, B);
}

void RGB_Renew(u8 R, u8 G, u8 B)
{
	_CR = R;
	_CG = G;
	_CB = B;
}

void MusicCommand(u8 *i, u8 size)
{
	u8 j = 1, k = 0;

	for (; j<=size; j++, i++)
	{
		uasrt3SendByte(*i);
		k += *i; 
	}
	uasrt3SendByte(k);
}

//init return 0:OK other:ERROR
u8 MusicInitialization(void)
{
	u8 i = 0;

	while (i < 2)
	{
		MusicCommand((u8 *)&_MusicInit[0], sizeof(_MusicInit));
		delay_ms(100);
		if (UART3_RX_Cnt != 0)
		{
			if ((0X01 == UART3_RX_Cache[2]) &&
				(0X02 == UART3_RX_Cache[3]))
			{
				UART3_RX_Cnt = 0;
				return 0; //success
			}
			else
			{
				UART3_RX_Cnt = 0;
			}
		}
		i++;
	}
	MusicCommand((u8 *)&_MusicVol[0], sizeof(_MusicVol));
	return 1; //error
}

u8 VoiceTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
void MusicStart(u8 i)
{
	_MusicPlay[4] = i;
	MusicCommand(&_MusicPlay[0], sizeof(_MusicPlay));
}

void DistCommand(u8 *i, u8 size)
{
	u8 j = 1;

	for (; j<=size; j++, i++)
		uasrt2SendByte(*i);
}

//init return 0:OK other:ERROR
u8 DistInitialization(void)
{
	u8 i = 0;

	while (i < 2)
	{
		DistCommand((u8 *)&_DistInit[0], sizeof(_DistInit));
		delay_ms(100);
		if (UART2_RX_Cnt != 0)
		{
			if ((_DistInitRe[0] == UART2_RX_Cache[0]) &&
				(_DistInitRe[1] == UART2_RX_Cache[1]))
			{
				UART2_RX_Cnt = 0;

				//Set group
				DistCommand((u8 *)&_DistGroup[0], sizeof(_DistGroup));
				delay_ms(200);
				UART2_RX_Cnt = 0;
				return 0; //success
			}
			else
			{
				UART2_RX_Cnt = 0;
			}
		}
		i++;
	}
	return 1; //error
}

//Head Motor
void HeadMotorUp(u16 Speed)
{
	MOTOR1_PIN = 1; //up
	TIM_SetCompare1(TIM1, 7199-Speed);
}

void HeadMotorDown(u16 Speed)
{
	MOTOR1_PIN = 0; //down
	TIM_SetCompare1(TIM1, Speed);
}

void HeadMotorStop(void)
{
	MOTOR1_PIN = 0; //stop
	TIM_SetCompare1(TIM1, 0);
}

//Middle Motor
void MiddleMotorRight(u16 Speed)
{
	MOTOR2_PIN = 1; //right
	TIM_SetCompare4(TIM1, 7199-Speed);
}

void MiddleMotorLeft(u16 Speed)
{
	MOTOR2_PIN = 0; //left
	TIM_SetCompare4(TIM1, Speed);
}

void MiddleMotorStop(void)
{
	MOTOR2_PIN = 0; //stop
	TIM_SetCompare4(TIM1, 0);
}

//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++) send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++) send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++) uasrt1SendByte(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
}  

void Interaction(void)
{
	RGB_Renew(0, 20, 160);
	MusicStart(V_HELLO);
	delay_ms(300);
	HeadMotorDown(7199);
	MOUTH_PIN = 0;
	delay_ms(250);
	HeadMotorUp(7199);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorDown(7199);
	MOUTH_PIN = 0;
	MusicStart(V_HELLO);
	delay_ms(250);
	HeadMotorUp(7199);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorStop();
	MOUTH_PIN = 0;
	RGB_Renew(160, 160, 0);
	/*
	listening
	*/
	DistCommand((u8 *)&_DistStartOnce[0], sizeof(_DistStartOnce));

	// if(calendar.min>=55) //close next 5 min
	// 	RTC_Alarm_Set(calendar.w_year, calendar.w_month, calendar.w_date,
	// 				calendar.hour+1, calendar.min-55, calendar.sec);
	// else
		// RTC_Alarm_Set(calendar.w_year, calendar.w_month, calendar.w_date,
		// 			calendar.hour, calendar.min+5, calendar.sec);
	srand(SysRunTime);	
	RandTime[0] = rand()%100/2*10;
	srand(RandTime[0]);
	RandTime[1] = rand()%100/2*10;
	if(RandTime[0]>RandTime[1])
	{
		RandTime[0] = RandTime[0]^RandTime[1];
		RandTime[1] = RandTime[0]^RandTime[1];
		RandTime[0] = RandTime[0]^RandTime[1];
	}
	printf("%d, %d\n", RandTime[0], RandTime[1]);
}

void TimeAction(void)
{
	u8 j = 0;

	RTCAlarm = 0;

	//number to voice
	j = calendar.hour/10;
	if(j!=0)
	{
		VoiceTable[0] = j; 
		VoiceTable[1] = V_TEN;
	}
	else
	{
		VoiceTable[0] = V_NULL; 
		VoiceTable[1] = V_NULL;
	}	
	j = calendar.hour%10;
	if(j!=0)
	{
		VoiceTable[2] = j; 
	}
	else
	{
		VoiceTable[2] = V_ZERO;
	}
	
	j = calendar.min/10;
	if(j!=0)
	{
		VoiceTable[3] = j; 
		VoiceTable[4] = V_TEN;
	}
	else
	{
		VoiceTable[3] = V_NULL; 
		VoiceTable[4] = V_NULL;
	}
	j = calendar.min%10;
	if(j!=0)
	{
		VoiceTable[5] = j; 
	}
	else
	{
		VoiceTable[5] = V_ZERO;
	}
	VoiceTable[6] = 0;
	
	RGB_Renew(160, 160, 160);
	MusicStart(V_TIME_NOW);
	delay_ms(1000);
	delay_ms(500);
	HeadMotorDown(7199);
	MusicStart(VoiceTable[0]);
	MOUTH_PIN = 0;
	delay_ms(250);
	HeadMotorUp(7199);
	MusicStart(VoiceTable[1]);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorDown(7199);
	MusicStart(VoiceTable[2]);
	MOUTH_PIN = 0;
	delay_ms(250);
	HeadMotorUp(7199);
	MusicStart(VoiceTable[3]);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorDown(7199);
	MusicStart(VoiceTable[4]);
	MOUTH_PIN = 0;
	delay_ms(250);
	HeadMotorUp(7199);
	MusicStart(VoiceTable[5]);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorDown(7199);
	MusicStart(V_MIN);
	MOUTH_PIN = 0;
	delay_ms(250);
	HeadMotorUp(7199);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorStop();
	MOUTH_PIN = 0;
	RGB_Renew(160, 160, 0);

	if(CloseTime)
	{
		CloseTime = 0;
		Sys_Enter_Standby();
	}		
}

void RemindAction(void)
{
	u8 temperature;  	    
	u8 humidity;  
	// float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	// short temp;					//�¶�	

	DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ
	printf("Temp:%d; Hum:%d\n", temperature, humidity);

	// temp = MPU_Get_Temperature();	//�õ��¶�ֵ
	//mpu_dmp_get_data(&pitch,&roll,&yaw); //������̬��ȡ
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
	//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
	mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);
	//����������̬
	// usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,
	// 				gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
	
	MusicStart(V_HELLO);
}

void RandomAction(void)
{
	RGB_Renew(1, 80, 20);
	RandActionRun = 0;
	MusicStart(V_HELLO);
	delay_ms(300);
	HeadMotorDown(7199);
	MOUTH_PIN = 0;
	delay_ms(250);
	HeadMotorUp(7199);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorDown(7199);
	MOUTH_PIN = 0;
	MusicStart(V_HELLO);
	delay_ms(250);
	HeadMotorUp(7199);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorStop();
	MOUTH_PIN = 0;
	RGB_Renew(160, 160, 0);
}
