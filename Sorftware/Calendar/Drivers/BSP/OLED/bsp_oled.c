#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
 #include "ds1302.h"
 #include "stdio.h"

#define SHOW_MODE 1    
#define FONT_SIZE 16

// ��ʾ�����յĺ����ӿ�
void OLED_ShowDate(uint8_t x, uint8_t y, Time_s time)
{
    uint8_t offset = 0;
    
    // ��ʾ���
    OLED_ShowNum(x + offset, y, time.year + 2000, 4, FONT_SIZE, SHOW_MODE); // ����time.year�洢����2000��֮������
    offset += 4 * (FONT_SIZE / 2 + 1); // ����FONT_SIZEΪ16��ÿ������ռ��16+4�����ؿ��

    // ��ʾ�ָ������ꡱ
    OLED_ShowChinese(x + offset, y, 0, FONT_SIZE, SHOW_MODE); // ����0��Ӧ���ꡱ��
    offset += FONT_SIZE + 4;    // ����ռ��FONT_SIZE��ȣ����϶���ļ��

    // ��ʾ�·�
    OLED_ShowNum(x + offset, y, time.month, 2, FONT_SIZE, SHOW_MODE);
    offset += 2 * (FONT_SIZE / 2 + 1);

    // ��ʾ�ָ������¡�
    OLED_ShowChinese(x + offset, y, 1, FONT_SIZE, SHOW_MODE); // ����1��Ӧ���¡���
    offset += FONT_SIZE + 4;

    // ��ʾ����
    OLED_ShowNum(x + offset, y, time.date, 2, FONT_SIZE, SHOW_MODE);
    offset += 2 * (FONT_SIZE / 2 + 1);

    // ��ʾ�ָ������ա�
    OLED_ShowChinese(x + offset, y, 2, FONT_SIZE, SHOW_MODE); // ����2��Ӧ���ա���
}

void OLED_ShowYearDate(uint8_t x, uint8_t y, Time_s time)
{
    // ���������ַ���
    char dateStr[20];  // ������󳤶Ȳ�����20�ַ�
    snprintf(dateStr, sizeof(dateStr), "%04d��%02d��%02d��", (time.year + 2000), time.month, time.date);
    
    // for(int i=0;i<ARRAY_SIZE(dateStr);i++)
    // {
    //     printf("%c",dateStr[i]);
    // }
    // printf("\n");
    // ʹ��OLED_ShowStringһ������ʾ�����ַ���
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}

