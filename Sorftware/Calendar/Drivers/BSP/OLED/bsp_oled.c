/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-11 09:44:40
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-11 14:42:37
 * @FilePath: \Calendar\Drivers\BSP\OLED\bsp_oled.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
 #include "ds1302.h"
 #include "stdio.h"

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

