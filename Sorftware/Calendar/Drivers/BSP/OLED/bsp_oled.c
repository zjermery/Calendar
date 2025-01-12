/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-11 09:44:40
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-12 18:51:01
 * @FilePath: \Calendar\Drivers\BSP\OLED\bsp_oled.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
#include "ds1302.h"
#include "stdio.h"
#include "dht11.h"



#define SHOW_MODE 1    
#define FONT_SIZE 16



void DisplayShowYearDate(uint8_t x, uint8_t y, Time_s time)
{
    // ���������ַ���
    char dateStr[20];  // ������󳤶Ȳ�����20�ַ�
    snprintf(dateStr, sizeof(dateStr), "%04d��%02d��%02d��", (time.year + 2000), time.month, time.date);
    // ʹ��OLED_ShowStringһ������ʾ�����ַ���
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}

void DisplayShowTime(uint8_t x, uint8_t y, Time_s time)
{
    // ���������ַ���
    char dateStr[20];  // ������󳤶Ȳ�����20�ַ�
    snprintf(dateStr, sizeof(dateStr), "%02dʱ%02d��%02d��", time.hour, time.min, time.sec);
    // ʹ��OLED_ShowStringһ������ʾ�����ַ���
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}


void DisplayShowDay(uint8_t x, uint8_t y, Time_s time)
{
    // ���������ַ���
    char dateStr[20];  // ������󳤶Ȳ�����20�ַ�
    switch (time.day)
    {
    case MONDAY:  
        snprintf(dateStr, sizeof(dateStr), "����һ");
        break;
    case TUESDAY:  
        snprintf(dateStr, sizeof(dateStr), "���ڶ�");
        break;
    case WEDNESDAY:  
        snprintf(dateStr, sizeof(dateStr), "������");
        break;
    case THURSDAY:  
        snprintf(dateStr, sizeof(dateStr), "������");
        break;    
    case FRIDAY:  
        snprintf(dateStr, sizeof(dateStr), "������");
        break;
    case SATURDAY:  
        snprintf(dateStr, sizeof(dateStr), "������");
        break;
    case SUNDAY:  
        snprintf(dateStr, sizeof(dateStr), "������");
        break;
    default:
        snprintf(dateStr, sizeof(dateStr), "������");
        break;
    }
    // ʹ��OLED_ShowStringһ������ʾ�����ַ���
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}



uint8_t DisplayShowTempHumi(uint8_t x, uint8_t y, uint32_t tempAndhumi)
{
    char dateStr[20];  // ������󳤶Ȳ�����20�ַ�
    uint8_t buf[4];
    extern const unsigned char Temperature_and_humidity30x44[];
    if(x > 40 || y > 20 )
    {
        return 1;
    }
      // ���¶�ֵ���Ϊ�ĸ��ֽ�
    buf[0] = (tempAndhumi >> 24) & 0xFF;  // ��ȡ����ֽ�
    buf[1] = (tempAndhumi >> 16) & 0xFF;  // ��ȡ�θ��ֽ�
    buf[2] = (tempAndhumi >> 8) & 0xFF;   // ��ȡ�ε��ֽ�
    buf[3] = tempAndhumi & 0xFF;          // ��ȡ����ֽ�
    OLED_ShowPicture(0,10,30,44,Temperature_and_humidity30x44,1);
    snprintf(dateStr, sizeof(dateStr), "ʪ��:%02d.%02d%%", buf[0],buf[1]);
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
    y += 25;
    snprintf(dateStr, sizeof(dateStr), "�¶�:%02d.%02d��", buf[2],buf[3]);
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
    return 0;
}




