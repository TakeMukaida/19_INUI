#pragma once

#include "includes.h"

#define M_ID_1 0x10
#define M_ID_2 0x12
#define M_ID_3 0x14
#define M_ID_4 0x16

#define MAX 50

class OMNI
{
private:
    I2C *myi2c;
    PID *mypid;
    OLED *myoled;
    int m[4];
    int z;
    int max;
    int pid;
    char m1[1];
    char m2[1];
    char m3[1];
    char m4[1];

public:
    OMNI(I2C *instance_i2c, PID *instance_pid, OLED *instance_oled);

    void omni(float, int, int, float, int);
    
    void only_disp_omni(float, int, int, float, int);

    ~OMNI();
};
