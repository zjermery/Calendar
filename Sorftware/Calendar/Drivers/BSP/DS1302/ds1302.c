#include "stdio.h"
#include "ds1302.h"
#include "delay.h"
#include "sys.h"


uint8_t read[] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};//���롢�֡�ʱ���ա��¡��ܡ���ļĴ�����ַ
uint8_t write[] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};//д�롢�֡�ʱ���ա��¡��ܡ���ļĴ�����ַ
/*******************��������******************************************/
static void ds1302_GPIO_Configuration(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

	DS1302_RST_GPIO_CLK_ENABLE();
	DS1302_DAT_GPIO_CLK_ENABLE();
	DS1302_CLK_GPIO_CLK_ENABLE();

	//RST
	GPIO_InitStructure.Pin 		= DS1302_RST_GPIO_PIN;
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP; 		 
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;		//�ٶ�50MHz
	HAL_GPIO_Init(DS1302_RST_GPIO_PORT, &GPIO_InitStructure);
	//DATA
	GPIO_InitStructure.Pin 		= DS1302_DAT_GPIO_PIN;		//E5-DATA����ΪҪ��˫���䣬��������Ϊ��©
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DS1302_DAT_GPIO_PORT, &GPIO_InitStructure);
	//SCLK 	
	GPIO_InitStructure.Pin 		= DS1302_CLK_GPIO_PIN;
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;	
	GPIO_InitStructure.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DS1302_CLK_GPIO_PORT, &GPIO_InitStructure);
}
/*****************************************************************/
/********************���Ͱ�λ����*********************************/
void Sendchar(uint8_t dat)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		DS1302_CLK_Clr();			//����ʱ���ߣ�׼����������
		if(dat & 0x01)				//�ж��������һλ�Ǹ߻��ǵ�
			DS1302_DAT_Set();
		else
			DS1302_DAT_Clr();
		dat >>= 1;					//����׼��������һλ
		DS1302_CLK_Set();			//����ʱ���ߣ����ݳɹ�����
	}
}
/********************************************************************/
/*************************************************��ȡ��λ����******************************************************************/
uint8_t Readchar()
{
	uint8_t Return_dat,i;
	for(i=0;i<8;i++)
	{																						
		if(HAL_GPIO_ReadPin(DS1302_DAT_GPIO_PORT,DS1302_DAT_GPIO_PIN) == GPIO_PIN_SET){		//�ж�������״̬�Ǹ߻��ǵͣ�ע��һ����			
			Return_dat = Return_dat | 0x80;													//����Ǹߵ�ƽ��ȡ�����ݣ����ǵĻ�����Ҫȡ��Ĭ���ǵ͵�ƽ
		}
		DS1302_CLK_Set();						//ʱ�������ߣ�׼��ȡ������
		Return_dat >>= 1;						//����һλ׼��������һλ����
		DS1302_CLK_Clr();						//ʱ�������ͣ�ȡ������
	}
	DS1302_DAT_Clr();							//�������Ҫ���ڽ��ܰ�λ����֮��һ��Ҫ�ſ������ߣ��������ȫ��0����85��datasheet����ûд��һ�㡣�ܶ��˶���������
	return Return_dat;							//���ؽ��յ��İ�λ����
}
/********************************************************************************************************************************/
/*****************��ָ����λ��ַ��д���λ����******************/
void write_data_to_addr(uint8_t add,uint8_t dat)											
{	 	
	DS1302_RST_Clr();				//��λ������
	DS1302_CLK_Clr();				//ʱ��������
	DS1302_RST_Set();				//��λ�����ߣ�׼������
	Sendchar(add);					//���Ͱ�λ��ַ
	Sendchar(dat);					//���Ͱ�λ�����������
	DS1302_CLK_Set();				//�����ذ���������
	DS1302_RST_Clr();				//���͸�λ�ߣ��������
}
/**************************************************************/
/******************DS1302��ʼ������ʼ��ʱ��********************/
void ds1302_init()				
{	
	ds1302_GPIO_Configuration();

	write_data_to_addr(0x8e,0x00);							//�ر�д����
	write_data_to_addr(0x80,((21/10)<<4|(21%10)));			//��
	write_data_to_addr(0x82,((28/10)<<4|(28%10)));			//����
	write_data_to_addr(0x84,((15/10)<<4|(15%10)));			//Сʱ
	write_data_to_addr(0x86,((3/10)<<4|(3%10)));			//��
	write_data_to_addr(0x88,((4/10)<<4|(4%10)));			//��
	write_data_to_addr(0x8a,3);								//����
	write_data_to_addr(0x8c,((19/10)<<4|(19%10)));			//��              
	write_data_to_addr(0x8e,0x80);							//��д����
}
/**************************************************************/
/***************��DS1302�ж�ȡ��λ���ݣ�addΪĿ���ַ**********/
uint8_t read_data_from_1302(uint8_t add)										          	
{
	uint8_t i,Return_dat;
	DS1302_RST_Clr();				//��λ������
	DS1302_CLK_Clr();				//ʱ��������
	DS1302_RST_Set();				//��λ�����ߣ�׼������
	Sendchar(add);					//���Ͱ�λ��ַ
	Return_dat=Readchar();			//�Ӹոշ��͵İ�λ��ַ�ж�������
	i=Return_dat/16;				//�����ʮ�����Ƶĸ���λ
	Return_dat=Return_dat%16;		//�����ʮ�����Ƶĵ���λ
	Return_dat=i*10+Return_dat;		//���ϳ�ʮ����
	return Return_dat;				//��������
	}
/**************************************************************/
extern uint8_t gewei[7],shiwei[7];
/**************��DS1302�ж�ȡ��λ���ݲ��ô��ڴ�ӡ����*************/
void ds1302_print_data(uint8_t *read)												 
{
	uint8_t i=0;
	for(i=0;i<7;i++)			//ѭ���ߴζ���ʱ��Ĵ���������
	{
		shiwei[i]=read_data_from_1302(read[i]);												
	}
	for(i=0;i<7;i++)			//�����ʮλ�͸�λ
	{								
		gewei[i]=shiwei[i]%10;
		shiwei[i]=shiwei[i]/10;
		printf("shiwei[%d]:%d\tgewei[%d]:%d\n",i,shiwei[i],i,gewei[i]);
	}
}
