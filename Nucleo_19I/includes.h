#pragma once

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mbed.h"

//#include "coordinates.h"
//#include "gains.h"

#include "PID/PID.h"
#include "OLED/OLED.h"
#include "OMNI/OMNI.h"

#include "rotary_encoder_ab_phase.hpp"

#define CONTROL_CYCLE   0.010       // 制御周期 [sec]
#define M_PI            3.1415926

#define P_HEADER 0xA6       //Primary
#define S_HEADER 0xB6       //Secondary
#define D_HEADER 0xF5       //Data

#define LED_ADDRESS 0x20

#define MAIN_MICOM   0x00   //0000
#define INTAKE       0x01   //1000
#define ELEVATING    0x02   //0100
#define COLOR_SENSOR 0x03   //1100  
#define ROLLER       0x04   //0010

#define NUMBER_OF_TOWELS 3

void can_transmission(char, char);
void can_reception();
void gyro_handler();
void pc_handler();
double R6093U_get(char [], char);
void init();
void go_processing();

//Functions of each phase for INUI
//0:バスタオル・シーツ回収
//1:バスタオル・シーツ排出
//2:Tシャツ回収
//3:Tシャツ排出
//4:Tシャツ掛け                     

//Functions of flags array
//[0]:往路で前段土台を検知する          :現在凍結中
//[1]:復路で前段土台を検知する          :現在凍結中
//[6]:画像処理

//Functions of order array
//[0]:ハンガー(blue)
//[1]:しわ伸ばし
//[2]:往路                         
//[3]:復路
//[4]:ハンガー(red)
//[5]:しわ伸ばし
//[6]:往路                          
//[7]:復路

//Functions of can_data array
//[0]:取込機構
//[1]:昇降機構
//[2]:カラーセンサ機構

//Functions of can_address
//0x00:取込機構
//0x01:昇降機構
//0x02:カラーセンサ機構
//0x03:ローラー機構

//Functions of Intake Machinism 
//0x00:get
//0x01:get
//0x02:initial position
//0x03:reservation
