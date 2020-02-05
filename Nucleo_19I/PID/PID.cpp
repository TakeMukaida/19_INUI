#include "PID.h"

PID::PID( double p, double i, double d ) {
    Kp = p;
    Ki = i;
    Kd = d;
    integ = 0;
    En_1 = 0;
    En_2 = 0;
}

double PID::get_pid(double target, double now) {
    En = target - now;
    integ = Ki * En_2 + Ki * En_2 + Ki * En;
    b = integ + Kp * En + Kd * ((En - En_1) - (En_1 - En_2));
    En_2 = En_1;
    En_1 = En;
    if(b > 127){
        b = 127;
    }
    else if(b < -127){
        b = -127;
    }
    return b;
}

PID::~PID() {

}