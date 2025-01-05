#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
 #include "ds1302.h"

// ��ʾ�����յĺ����ӿ�
void OLED_ShowDate(uint8_t x, uint8_t y, Time_s time)
{
    uint8_t offset = 0;
    uint8_t size1 = 16;
    uint8_t mode = 1; // 1��ʾ�׵׺��֣�0��ʾ�ڵװ���
    // ��ʾ���
    OLED_ShowNum(x + offset, y, time.year + 2000, 4, size1, mode); // ����time.year�洢����2000��֮������
    offset += 4 * (size1 / 2 + 2); // ����size1Ϊ16��ÿ������ռ��16+4�����ؿ��

    // ��ʾ�ָ������ꡱ
    OLED_ShowChinese(x + offset, y, 0, size1, mode); // ����0��Ӧ���ꡱ��
    offset += size1 + 8;    // ����ռ��size1��ȣ����϶���ļ��

    // ��ʾ�·�
    OLED_ShowNum(x + offset, y, time.month, 2, size1, mode);
    offset += 2 * (size1 / 2 + 2);

    // ��ʾ�ָ������¡�
    OLED_ShowChinese(x + offset, y, 1, size1, mode); // ����1��Ӧ���¡���
    offset += size1 + 8;

    // ��ʾ����
    OLED_ShowNum(x + offset, y, time.date, 2, size1, mode);
    offset += 2 * (size1 / 2 + 2);

    // ��ʾ�ָ������ա�
    OLED_ShowChinese(x + offset, y, 2, size1, mode); // ����2��Ӧ���ա���
}