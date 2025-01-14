#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"


static uint8_t OLED_GRAM[144][8];


/**
  * @brief  向OLED写入数据
  * @param  usData :要写入的数据
  * @retval 无
  */	
static void OLED_WR_Byte(uint8_t usData,uint8_t cmd)
{	
	uint8_t i;			  
	if(cmd)
		OLED_DC_Set();
	else
	  	OLED_DC_Clr();

	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		if(usData & 0x80)
		   OLED_SDA_Set();
		else 
		   OLED_SDA_Clr();
		OLED_SCL_Set();
		usData <<= 1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}


/**
  * @brief  反显函数
  * @param  enable :enable :0 正常显示 1：反转显示
  * @retval 无
  */
static void OLED_ColorTurn(uint8_t enable)
{
	if(!enable)
	{
		OLED_WR_Byte(0xA6,OLED_WRITE_CMD);//正常显示
	}
	else
	{
		OLED_WR_Byte(0xA7,OLED_WRITE_CMD);//反色显示
	}
}


/**
  * @brief  屏幕旋转180度
  * @param  enable :0 正常显示 1：反转显示
  * @retval 无
  */	
static void OLED_DisplayTurn(uint8_t enable)
{
	if(!enable)
	{
		OLED_WR_Byte(0xC8,OLED_WRITE_CMD);//正常显示
		OLED_WR_Byte(0xA1,OLED_WRITE_CMD);
	}
	else
	{
		OLED_WR_Byte(0xC0,OLED_WRITE_CMD);//反转显示
		OLED_WR_Byte(0xA0,OLED_WRITE_CMD);
	}
}

/**
  * @brief  开启OLED显示
  * @retval 无
  */
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_WRITE_CMD);	//电荷泵使能
	OLED_WR_Byte(0x14,OLED_WRITE_CMD);	//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_WRITE_CMD);	//点亮屏幕
}

/**
  * @brief  关闭OLED显示
  * @retval 无
  */
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_WRITE_CMD);	//电荷泵使能
	OLED_WR_Byte(0x10,OLED_WRITE_CMD);	//关闭电荷泵
	OLED_WR_Byte(0xAE,OLED_WRITE_CMD);	//关闭屏幕
}


/**
  * @brief  更新显存到OLED
  * @retval 无
  */	
void OLED_Refresh(void)
{
	uint8_t i,n;
	for(i = 0; i < 8; i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_WRITE_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_WRITE_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_WRITE_CMD);   //设置高列起始地址
	   for(n = 0; n < 128; n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_WRITE_DATA);
  }
}

/**
  * @brief  清屏函数
  * @retval 无
  */
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i = 0; i < 8; i++)
	{
	   for(n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i]=0;//清除所有数据
		}
  	}
	OLED_Refresh();//更新显示
}
	
/**
  * @brief  画点
  * @param  x :起始X轴坐标
  * @param  y :起始y轴坐标
  * @param  set : 1：填充，0：清空
  * @retval 无
  */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t set)
{
	uint8_t i,m,n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	if(set)
	{
		OLED_GRAM[x][i] |= n;
	}
	else
	{
		OLED_GRAM[x][i] =~ OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] =~ OLED_GRAM[x][i];
	}
}


/**
  * @brief  画线
  * @param  x1 :起始X轴坐标
  * @param  y1 :起始y轴坐标
  * @param  x2 :结束X轴坐标
  * @param  y2 :结束y轴坐标
  * @param  mode : 1：填充，0：清空
  * @retval 无
  */
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	uint16_t t; 
	int xerr = 0, yerr = 0, delta_x, delta_y,distance;
	int inc_x,inc_y,uRow,uCol;

	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1;
	uRow = x1;//画线起点坐标
	uCol = y1;
	if(delta_x > 0)
		inc_x = 1; 	//设置单步方向 
	else if (delta_x == 0)
		inc_x = 0;	//垂直线 
	else 
	{
		inc_x = -1;
		delta_x = -delta_x;
	}

	if(delta_y > 0)
		inc_y = 1;
	else if (delta_y == 0)
		inc_y = 0;	//水平线 
	else {
		inc_y = -1;
		delta_y = -delta_x;
	}

	if(delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴 
	else
		distance = delta_y;
	for(t = 0; t < distance + 1; t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//画点
		xerr += delta_x;
		yerr += delta_y;
		if(xerr > distance)
		{
			xerr -= distance;
			uRow += inc_x;
		}
		if(yerr > distance)
		{
			yerr -= distance;
			uCol += inc_y;
		}
	}
}

/**
  * @brief  画圆
  * @param  x :圆心坐标
  * @param  y :圆心坐标
  * @param  r : 半径
  * @retval 无
  */
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
	uint8_t i,m,temp,size2,chr1;
	uint8_t x0 = x,y0 = y;
	if(size1 == 8)
		size2 = 6;
	else
		 size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);  	//得到字体一个字符对应点阵集所占的字节数
	chr1 = chr - ' ';  	//计算偏移后的值
	// chr1 = NUM_TO_CHAR(chr) - ' ';
	for(i = 0; i < size2; i++)
	{
		if(size1 == 8){
			temp = asc2_0806[chr1][i];	 	//调用0806字体
		}
		else if(size1 == 12){
			temp = asc2_1206[chr1][i];		//调用1206字体
		} 
		else if(size1 == 16){
			temp = asc2_1608[chr1][i];		//调用1608字体
		} 	
		else if(size1 == 24){
			temp = asc2_2412[chr1][i];		//调用2412字体
		}
		else 
			return;
		
		for(m = 0; m < 8; m++)
		{
			if(temp & 0x01)
				OLED_DrawPoint(x,y,mode);
			else 
				OLED_DrawPoint(x,y,!mode);
			temp >>= 1;
			y++;
		}
		x++;
		if((size1 != 8) && ((x - x0) == size1 / 2))
		{	
			x = x0;
			y0 += 8;
		}
		y = y0;
  }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode)
{
	while((*chr >= ' ') && (*chr <= '~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)
			x += 6;
		else 
			x += size1/2;
		chr++;
  }
}

//m^n
static uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result *= m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
	uint8_t t,temp,m=0;
	if(size1 == 8) 
		m = 2;
	for(t = 0;t < len; t++)
	{
		temp = (num / OLED_Pow(10, len - t - 1)) % 10;
//		printf("shownum:%d\n",temp);
		if(temp == 0)
		{
			OLED_ShowChar(x + (size1 / 2 + m) * t, y,'0', size1, mode);
      	}		
		else 
		{
		  OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
		}
  }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//mode:0,反色显示;1,正常显示
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode)
{
	uint8_t m,temp;
	uint8_t x0 = x,y0 = y;
	uint16_t i,size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;  //得到字体一个字符对应点阵集所占的字节数
	for(i = 0; i < size3; i++)
	{
		if(size1 == 16){
			temp = calendar_16x16[num][i];			//调用16*16字体
		}
		else if(size1 == 24){
			temp = calendar_24x24[num][i];			//调用24*24字体
		}
		else if(size1==32){
			temp = calendar_32x32[num][i];			//调用32*32字体
		}
		else if(size1 == 64){
			temp = calendar_64x64[num][i];			//调用64*64字体
		}
		else return;

		for(m = 0; m < 8; m++)
		{
			if(temp & 0x01)
				OLED_DrawPoint(x,y,mode);
			else 
				OLED_DrawPoint(x,y,!mode);
			temp >>= 1;
			y++;
		}
		x++;
		if((x-x0) == size1){
			x = x0;
			y0 += 8 ;
		}
		y=y0;
	}
}

static uint8_t find_index(uint16_t Index)
{
	uint8_t i;
	// printf("Index = 0X%X\r\n",Index);
	for(i = 0; i < sizeof(GB16_FONT_MASK) / 34; i++){//查数组
		if(Index == GB16_FONT_MASK[i].Index){
			return i;
		}
	}
	return NOT_FOUND;
}

static void OLED_ShowChineseGbk(uint8_t x,uint8_t y,uint16_t Index,uint8_t size1,uint8_t mode)
{
	uint16_t i,j;
	uint8_t x0 = x,y0 = y;
	uint8_t temp, size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1; 
	uint8_t index = find_index(Index);
	// printf("index = %d\r\n",index);
	
	for(i = 0; i < size2; i++)
	{
		temp = GB16_FONT_MASK[index].MASK[i];
		for(j = 0; j < 8; j++)
		{
			if(temp & 0x01)
				OLED_DrawPoint(x,y,mode);
			else 
				OLED_DrawPoint(x,y,!mode);
			temp >>= 1;
			y++;
		}
		x++;
		if((x - x0) == size1){
			x = x0;
			y0 += 8 ;
		}
		y = y0;
	}
}

//坐标设置
// static void OLED_Set_Pos(unsigned char x, unsigned char y) 
// { 	OLED_WR_Byte(0xb0 + y,OLED_WRITE_CMD);
// 	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10,OLED_WRITE_CMD);
// 	OLED_WR_Byte((x & 0x0f),OLED_WRITE_CMD); 
// }


/*
	x:横坐标，0-127
	y:0-7(中景园OLED)
    *s:要显示的字符（中英文均可，前提是数组中包含的汉字）
*/
void OLED_ShowChineseString(uint8_t x, uint8_t y, char *str,uint8_t size1,uint8_t mode)
{
	unsigned char k,length;
	unsigned short Index = 0;
	length = strlen(str);//取字符串总长

	for(k = 0; k < length; k++)
	{
		//printf("k = %d\r\n",k);
		if(*(str + k) <= 127)		//小于128是ASCII符号
		{
			//printf("ascii\r\n");
			OLED_ShowChar(x,y,*(str + k),size1,mode);
			x += 8;
		}
		else if(*(str + k) > 127)	//大于127，为汉字，前后两个组成汉字内码
		{
			//printf("汉字\r\n");
			Index = (*(str + k) << 8) | (*(str + k + 1));//取汉字的内码
			// printf("Index1 = 0X%X\r\n",Index);
			OLED_ShowChineseGbk(x,y,Index,size1,mode);	
			x += 16;//x坐标右移16
			k += 1; //汉字占2B,跳过一个
		}
	}
}

//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode)
{
	uint8_t i,n,t = 0,m = 0, r;
	while(1)
	{
		if(m == 0)
		{
	    	OLED_ShowChinese(128,24,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t == num)
		{
			for(r = 0;r < 16*space; r++)      //显示间隔
			 {
				for(i = 1;i < 144; i++)
				{
					for(n = 0;n < 8; n++)
					{
						OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
					}
				}
           		OLED_Refresh();
			}
        	t=0;
        }
		m++;
		if(m == 16){
			m=0;
		}
		for(i=1;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,const unsigned char BMP[],uint8_t mode)
{
	uint16_t j=0;
	uint8_t i,n,temp,m;
	uint8_t x0 = x,y0 = y;
	sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
	for(n = 0; n < sizey; n++)
	{
		 for(i = 0;i < sizex; i++)
		 {
				temp = BMP[j];
				j++;
				for(m = 0;m < 8; m++)
				{
					if(temp & 0x01)
						OLED_DrawPoint(x,y,mode);
					else
						OLED_DrawPoint(x,y,!mode);
					temp >>= 1;
					y++;
				}
				x++;
				if((x - x0) == sizex)
				{
					x = x0;
					y0 += 8;
				}
				y = y0;
     }
	 }
}

static void OLED_GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能时钟 */
	OLED_CS_GPIO_CLK_ENABLE();
	OLED_DC_GPIO_CLK_ENABLE();
	OLED_RES_GPIO_CLK_ENABLE();
	OLED_SDA_GPIO_CLK_ENABLE();
	OLED_SCL_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Pin = OLED_CS_GPIO_PIN|OLED_SCL_GPIO_PIN|OLED_SDA_GPIO_PIN;	 
 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;//速度50MHz
 	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);	  

	GPIO_InitStructure.Pin = OLED_DC_GPIO_PIN;
 	HAL_GPIO_Init(OLED_DC_GPIO_PORT, &GPIO_InitStructure);	 
	
	GPIO_InitStructure.Pin = OLED_RES_GPIO_PIN;
 	HAL_GPIO_Init(OLED_RES_GPIO_PORT, &GPIO_InitStructure);	 
	OLED_CS_Set();
	OLED_SCL_Set();
	OLED_SDA_Set();
}

static void _oled_reset(void)
{
	OLED_RES_Clr();
	delay_ms(200);
	OLED_RES_Set();
}

static void OLED_InitREG_Configuration(void)
{
	OLED_WR_Byte(0xAE,OLED_WRITE_CMD);				//--turn off oled panel
	OLED_WR_Byte(0xFD,OLED_WRITE_CMD);				
	OLED_WR_Byte(0x12,OLED_WRITE_CMD);				
	OLED_WR_Byte(0xd5,OLED_WRITE_CMD);				//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0xA0,OLED_WRITE_CMD);				
	OLED_WR_Byte(0xA8,OLED_WRITE_CMD);				//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_WRITE_CMD);				//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_WRITE_CMD);				//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_WRITE_CMD);				//-not offset
	OLED_WR_Byte(0x40,OLED_WRITE_CMD);				//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0xA1,OLED_WRITE_CMD);				//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_WRITE_CMD);				//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xDA,OLED_WRITE_CMD);				//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_WRITE_CMD);				
	OLED_WR_Byte(0x81,OLED_WRITE_CMD);				//--set contrast control register
	OLED_WR_Byte(0xBF,OLED_WRITE_CMD);				// Set SEG Output Current Brightness
	OLED_WR_Byte(0xD9,OLED_WRITE_CMD);				//--set pre-charge period
	OLED_WR_Byte(0x25,OLED_WRITE_CMD);				//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDB,OLED_WRITE_CMD);				//--set vcomh
	OLED_WR_Byte(0x34,OLED_WRITE_CMD);				//Set VCOM Deselect Level
	OLED_WR_Byte(0xA4,OLED_WRITE_CMD);				// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_WRITE_CMD);				// Disable Inverse Display On (0xa6/a7)
	OLED_Clear();				
	OLED_WR_Byte(0xAF,OLED_WRITE_CMD);
}

//OLED的初始化
void OLED_Init(void)
{
	OLED_GPIO_Configuration();
	
	_oled_reset();

	OLED_InitREG_Configuration();
	OLED_ColorTurn(0);		//0:正常显示，1:反色显示
	OLED_DisplayTurn(0);	//0:正常显示，1:屏幕翻转显示
}

