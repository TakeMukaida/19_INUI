#pragma once

class PID
{
private:
    double Kp;
    double Ki;
    double Kd;
    double En;
    double En_1;
    double En_2;
    double b;
    double integ;

public:
    PID(double, double, double);

    double get_pid(double, double);

    ~PID();
};