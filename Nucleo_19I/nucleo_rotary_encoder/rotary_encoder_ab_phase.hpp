#ifndef INCLUDED_ROTARY_ENCODER_AB_PHASE_H
#define INCLUDED_ROTARY_ENCODER_AB_PHASE_H

#include "rotary_encoder_base.hpp"

class rotary_encoder_ab_phase : public rotary_encoder_base {
public:
    rotary_encoder_ab_phase(TIM_TypeDef* timer_type, size_t pulse_per_revol) : // AB相での分解能はパルス数の4倍
        rotary_encoder_base(timer_type, TIM_ENCODERMODE_TI12, pulse_per_revol << 2) {}
    virtual ~rotary_encoder_ab_phase() {}
};

#endif
