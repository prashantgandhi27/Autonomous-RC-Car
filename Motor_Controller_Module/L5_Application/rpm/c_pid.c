#include "c_pid.h"

void createPID(s_pid_t *_pid)
{
    _pid->dt = DT;
    _pid->max = MAX;
    _pid->min = MIN;
    _pid->kp = KP;
    _pid->ki = KI;
    _pid->kd = KD;
    _pid->pre_error = 0.0;
    _pid->integral = 0.0;
}

double calculate(double targetSpeed, double actualSpeed, s_pid_t *_pid)
{
    // Calculate error
    double error = targetSpeed - actualSpeed;

    // Calculate proportional term
    double pTerm = _pid->kp * error;

    // Calculate integral term
    _pid->integral += error * _pid->dt;
    double iTerm = _pid->ki * _pid->integral;

    // Calculate derivative term
    double derivative = (error - _pid->pre_error) / _pid->dt;
    double dTerm = _pid->kd * derivative;

    // Calculate total output
    double output = pTerm + iTerm + dTerm;

    if (output > _pid->max)
        output = _pid->max;
    else if (output < _pid->min)
        output = _pid->min;

    _pid->pre_error = error;
    return output;
}
