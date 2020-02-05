#include "includes.h"

OMNI::OMNI(I2C *instance_i2c, PID *instance_pid, OLED *instance_oled){
    myi2c = instance_i2c;
    mypid = instance_pid;
    myoled = instance_oled;
}

void OMNI::omni(float deg, int power, int turn, float gyro, int keep){
    power = abs(power);
    if(power > 127){
        power = 127;
    }
    z = abs(turn);
    pid = (signed short)-mypid->get_pid(keep, gyro);
    m[0] = sin(((double)-deg +  52.71 + (double)keep) * M_PI / 180.0) * power;
    m[1] = sin(((double)-deg + 127.29 + (double)keep) * M_PI / 180.0) * power;
    m[2] = sin(((double)-deg -  52.71 + (double)keep) * M_PI / 180.0) * power;
    m[3] = sin(((double)-deg - 127.29 + (double)keep) * M_PI / 180.0) * power;
    
    max = abs(m[0]);
    for(int i = 1; i < 4; i++){
        if(abs(m[i]) > max){
            max = abs(m[i]);
        }
    }
    for(int i = 0; i < 4; i++){
        m[i] = (m[i]*power/max+(-turn)) * power / (power+z) + pid;
    }
    
    //m[0] = m[0] * 1.2;
    //m[1] = m[1] * 1.3;
    
    m[0] = m[0] * 1;
    m[1] = m[1] * 1;
    m[2] = m[2] * 1.3;
    m[3] = m[3] * 1;
    
    max = abs(m[0]);
    for(int i = 1; i < 4; i++){
        if(abs(m[i]) > max){
            max = abs(m[i]);
        }
    }
    if(max > MAX){
        for(int i = 0; i < 4; i++){
            m[i] = m[i]*MAX/max;
        }
    }


    m1[0] = (signed char)m[0];
    m2[0] = (signed char)m[1];
    m3[0] = (signed char)m[3];
    m4[0] = (signed char)m[2];

    myi2c->write(M_ID_1, m1, 1, 0);
    myi2c->write(M_ID_2, m2, 1, 0);
    myi2c->write(M_ID_3, m3, 1, 0);
    myi2c->write(M_ID_4, m4, 1, 0);
    
    char moji[10]={0};
    sprintf(moji, "4:%4d 1:%4d", (signed char)-m4[0], (signed char)-m1[0]);
    myoled->OLED_printf(moji, 3);
    sprintf(moji, "3:%4d 2:%4d", (signed char)-m3[0], (signed char)-m2[0]);
    myoled->OLED_printf(moji, 4);
    
    return;
}

void OMNI::only_disp_omni(float deg, int power, int turn, float gyro, int keep){
    power = abs(power);
    if(power > 127){
        power = 127;
    }
    z = abs(turn);
    pid = (signed short)-mypid->get_pid(keep, gyro);
    m[0] = sin(((double)deg +  52.71 + (double)keep) * M_PI / 180.0) * power;
    m[1] = sin(((double)deg + 127.29 + (double)keep) * M_PI / 180.0) * power;
    m[2] = sin(((double)deg -  52.71 + (double)keep) * M_PI / 180.0) * power;
    m[3] = sin(((double)deg - 127.29 + (double)keep) * M_PI / 180.0) * power;
    
    max = abs(m[0]);
    for(int i = 1; i < 4; i++){
        if(abs(m[i]) > max){
            max = abs(m[i]);
        }
    }
    for(int i = 0; i < 4; i++){
        m[i] = (m[i]*power/max+(-turn)) * power / (power+z) + pid;
    }
    
    max = abs(m[0]);
    for(int i = 1; i < 4; i++){
        if(abs(m[i]) > max){
            max = abs(m[i]);
        }
    }
    if(max > 127){
        for(int i = 0; i < 4; i++){
            m[i] = m[i]*127/max;
        }
    }

    for(int i = 0; i < 4; i++){
        if(m[i] > 127){
          m[i] = 127;
        }
        if(m[i] < -127){
          m[i] = -127;
        }
    }

    m1[0] = (signed char)-m[0];
    m2[0] = (signed char)-m[1];
    m3[0] = (signed char)-m[2];
    m4[0] = (signed char)-m[3];
 
    char moji[10]={0};
    sprintf(moji, "3:%4d 1:%4d", (signed char)-m3[0], (signed char)-m1[0]);
    myoled->OLED_printf(moji, 4);
    sprintf(moji, "4:%4d 2:%4d", (signed char)-m4[0], (signed char)-m2[0]);
    myoled->OLED_printf(moji, 5);

    return;
}

OMNI::~OMNI(){
    
}