#ifndef INCLUDED_ROTARY_ENCODER_A_PHASE_H
#define INCLUDED_ROTARY_ENCODER_A_PHASE_H

#include "rotary_encoder_base.hpp"

class rotary_encoder_a_phase : public rotary_encoder_base {
public:
    rotary_encoder_a_phase(TIM_TypeDef* timer_type, size_t pulse_per_revol) : // A相での分解能はパルス数の倍
        rotary_encoder_base(timer_type, TIM_ENCODERMODE_TI1, pulse_per_revol << 1) {}
    virtual ~rotary_encoder_a_phase() {}
};

#endif
