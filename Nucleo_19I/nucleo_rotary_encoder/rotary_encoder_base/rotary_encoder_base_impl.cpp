#include "rotary_encoder_base_impl.hpp"
#include "rotary_encoder.hpp"
#include "mbed.h"

rotary_encoder_base_impl::rotary_encoder_base_impl(TIM_TypeDef* timer_type,
                                                   uint32_t encoder_mode,
                                                   size_t resolution) : rotary_encoder(resolution) {
    timer_handler_.Instance              = timer_type;
    timer_handler_.Init.Period           = max_counts_;
    timer_handler_.Init.CounterMode      = TIM_COUNTERMODE_UP;
    timer_handler_.Init.Prescaler        = 0;
    timer_handler_.Init.ClockDivision    = TIM_CLOCKDIVISION_DIV1;
    
    TIM_Encoder_InitTypeDef encoder;
    encoder.EncoderMode = encoder_mode;
    
    encoder.IC1Filter       = 0x0F;
    encoder.IC1Polarity     = TIM_INPUTCHANNELPOLARITY_RISING;
    encoder.IC1Prescaler    = TIM_ICPSC_DIV4;
    encoder.IC1Selection    = TIM_ICSELECTION_DIRECTTI;

    encoder.IC2Filter       = 0x0F;
    encoder.IC2Polarity     = TIM_INPUTCHANNELPOLARITY_FALLING;
    encoder.IC2Prescaler    = TIM_ICPSC_DIV4;
    encoder.IC2Selection    = TIM_ICSELECTION_DIRECTTI;
    
    if (HAL_TIM_Encoder_Init(&timer_handler_, &encoder) != HAL_OK) {
        error("couldn't init encoder\n");
    }
}

rotary_encoder_base_impl::~rotary_encoder_base_impl() {}

int32_t rotary_encoder_base_impl::get_counts() const {
    int32_t counts = timer_handler_.Instance->CNT;
    
    if (counts > (max_counts_ >> 1)) {
        return counts - max_counts_;
    }
    return  counts;
}

void rotary_encoder_base_impl::reset() {
    timer_handler_.Instance->CNT = 0;
}

void rotary_encoder_base_impl::start() {
    if(HAL_TIM_Encoder_Start(&timer_handler_, TIM_CHANNEL_1) != HAL_OK) {
        error("couldn't start encoder\r\n");
    }
}

void rotary_encoder_base_impl::stop() {
    if(HAL_TIM_Encoder_Stop(&timer_handler_, TIM_CHANNEL_1) != HAL_OK) {
        error("couldn't start encoder\r\n");
    }
}
