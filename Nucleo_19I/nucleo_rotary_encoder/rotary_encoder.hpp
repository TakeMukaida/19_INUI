#ifndef INCLUDED_ROTARY_ENCODER_H
#define INCLUDED_ROTARY_ENCODER_H

#include <stdint.h>
#include <stddef.h>

class rotary_encoder {
public:
    rotary_encoder(size_t resolution);
    virtual ~rotary_encoder() = 0;
    
    virtual int32_t get_counts() const = 0;
    float get_revol_num() const;
    float read() const;
    
    virtual void reset() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    static const size_t max_counts_;
    const size_t resolution_;
};

#endif
