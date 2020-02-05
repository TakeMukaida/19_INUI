#ifndef INCLUDED_ROTARY_ENCODER_BASE_IMPL_H
#define INCLUDED_ROTARY_ENCODER_BASE_IMPL_H

#include "mbed.h"
#include "rotary_encoder.hpp"

class rotary_encoder_base_impl : public rotary_encoder {
public:
    rotary_encoder_base_impl(TIM_TypeDef* timer_type,
                             uint32_t encoder_mode,
                             size_t resolution);
    virtual ~rotary_encoder_base_impl();
    
    virtual int32_t get_counts() const;
    
    virtual void reset();
    virtual void start();
    virtual void stop();
    
protected:
    TIM_HandleTypeDef timer_handler_;
};

#endif
