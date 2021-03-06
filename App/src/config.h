#ifndef __CONFIG_H 
#define __CONFIG_H

//变量类型定义(这一段无需改动)
typedef unsigned char  	uint8;                  /* defined for unsigned 8-bits integer variable 	       无符号8位整型变量  */
typedef signed   char  	int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short  uint16;               /* defined for unsigned 16-bits integer variable 	       无符号16位整型变量 */
typedef signed   short  int16;                  /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int    uint32;               /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int    int32;                  /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          	fp32;                   /* single precision floating point variable (32bits)     单精度浮点数（32位长度） */
typedef double          fp64;                   /* double precision floating point variable (64bits)   双精度浮点数（64位长度） */

typedef unsigned char 	bool;

#ifndef TRUE
#define TRUE  (uint8)1
#endif

#ifndef true
#define true  (uint8)1
#endif

#ifndef FALSE
#define FALSE (uint8)0
#endif
#ifndef false
#define false (uint8)0
#endif

#ifndef NULL
 #define NULL ((void *)0)
#endif
#ifndef null
#define null ((void *)0)
#endif

#ifndef VALID_DATA
#define VALID_DATA (uint8)2
#endif

#ifndef INVALID_DATA
#define INVALID_DATA (uint8)-1
#endif

/********************************/
/*      ARM specital code      			*/
/*      ARM的特殊代码            	*/
/********************************/

//系统头文件(这一段无需改动)
#include    "LPC2294.h"
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

/********************************/
/*Application Program Configurations*/
/*     应用程序配置             */
/********************************/
//This segment could be modified as needed.
//以下根据需要改动

/********************************/
/*Configuration of the example */
/*     本例子的配置             */
/********************************/
/* System configuration .Fosc、Fcclk、Fcco、Fpclk must be defined */
/* 系统设置, Fosc、Fcclk、Fcco、Fpclk必须定义*/
#define Fosc            11059200                    //Crystal frequence,10MHz~25MHz，should be the same as actual status. 
						    //应当与实际一至晶振频率,10MHz~25MHz，应当与实际一至
#define Fcclk           (Fosc * 4)                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
						    //系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO frequence,should be 2、4、8、16 multiples of Fcclk, ranged from 156MHz to 320MHz. 
						    //CCO频率，必须为Fcclk的2、4、8、16倍，范围为156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 4             //VPB clock frequence , must be 1、2、4 multiples of (Fcclk / 4).
						    //VPB时钟频率，只能为(Fcclk / 4)的1、2、4倍

#define KillWatchDog()   {IRQDisable();WDFEED =  0XAA;WDFEED =  0X55; IRQEnable();}


// 用户头文件
#include    "target.h"              //This line may not be deleted 这一句不能删除
#include    "comm.h"
#include    "Timer.h"
#include    "Mifare.h"
#include    "Lcd.h"
#include    "rtc.h"
#include    "M25PE16.h"
#include    "board.h"
#include 	"Hd7279a.h"
#include 	"psam.h"


#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
