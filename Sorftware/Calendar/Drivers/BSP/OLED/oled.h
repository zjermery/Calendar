#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	

#define NUM_TO_CHAR(n)    ((char)('0' + (n)))

#define OLED_WRITE_CMD  0	//写命令
#define OLED_WRITE_DATA 1	//写数据

/********************************** OLED端口定义 ********************************************************/
/* 引脚 定义 */
//SCL
#define OLED_SCL_GPIO_PORT              GPIOA   
#define OLED_SCL_GPIO_PIN               GPIO_PIN_5
#define OLED_SCL_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//SDA
#define OLED_SDA_GPIO_PORT              GPIOA   
#define OLED_SDA_GPIO_PIN               GPIO_PIN_7
#define OLED_SDA_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//RES
#define OLED_RES_GPIO_PORT              GPIOC   
#define OLED_RES_GPIO_PIN               GPIO_PIN_4            
#define OLED_RES_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//DC
#define OLED_DC_GPIO_PORT               GPIOA   
#define OLED_DC_GPIO_PIN                GPIO_PIN_6
#define OLED_DC_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

//CS    
#define OLED_CS_GPIO_PORT               GPIOA   
#define OLED_CS_GPIO_PIN                GPIO_PIN_4
#define OLED_CS_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */


/********************************** OLED端口定义END ********************************************************/

#define OLED_SCL_Clr() HAL_GPIO_WritePin(OLED_SCL_GPIO_PORT,OLED_SCL_GPIO_PIN,GPIO_PIN_RESET)//SCL
#define OLED_SCL_Set() HAL_GPIO_WritePin(OLED_SCL_GPIO_PORT,OLED_SCL_GPIO_PIN,GPIO_PIN_SET)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(OLED_SDA_GPIO_PORT,OLED_SDA_GPIO_PIN,GPIO_PIN_RESET)//SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(OLED_SDA_GPIO_PORT,OLED_SDA_GPIO_PIN,GPIO_PIN_SET)

#define OLED_RES_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_PORT,OLED_RES_GPIO_PIN,GPIO_PIN_RESET)//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_PORT,OLED_RES_GPIO_PIN,GPIO_PIN_SET)

#define OLED_DC_Clr()  HAL_GPIO_WritePin(OLED_DC_GPIO_PORT,OLED_DC_GPIO_PIN,GPIO_PIN_RESET)//DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(OLED_DC_GPIO_PORT,OLED_DC_GPIO_PIN,GPIO_PIN_SET)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_PORT,OLED_CS_GPIO_PIN,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_PORT,OLED_CS_GPIO_PIN,GPIO_PIN_SET)


/********************************** oled函数定义 ********************************************************/

void OLED_ClearPoint(uint8_t x,uint8_t y);
// void OLED_ColorTurn(uint8_t i);
// void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_Init(void);

#endif

