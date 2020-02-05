#include "mbed.h"
#include "rotary_encoder.hpp"

const size_t rotary_encoder::max_counts_ = 0xFFFF;

rotary_encoder::rotary_encoder(size_t resolution) :
        resolution_(resolution) {}
rotary_encoder::~rotary_encoder() {}

float rotary_encoder::get_revol_num() const {
    return static_cast<float>(get_counts()) / resolution_;
}

float rotary_encoder::read() const {
    return get_revol_num();
}
