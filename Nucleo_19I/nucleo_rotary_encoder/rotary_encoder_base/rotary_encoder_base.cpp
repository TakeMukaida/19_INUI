#include "rotary_encoder_base.hpp"
#include "rotary_encoder.hpp"
#include "rotary_encoder_base_impl.hpp"

rotary_encoder_base::rotary_encoder_base(TIM_TypeDef* timer_type,
                                        uint32_t encoder_mode,
                                        size_t resolution) : rotary_encoder(resolution),
                                                             encoder_(new rotary_encoder_base_impl(timer_type, encoder_mode, resolution)) {}

rotary_encoder_base::~rotary_encoder_base() {
    delete encoder_;
}

int32_t rotary_encoder_base::get_counts() const {
    return encoder_->get_counts();
}

void rotary_encoder_base::reset() {
    encoder_->reset();
}

void rotary_encoder_base::start() {
    encoder_->start();
}

void rotary_encoder_base::stop() {
    encoder_->stop();
}
