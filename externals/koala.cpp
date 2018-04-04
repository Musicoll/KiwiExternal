//
//  kawa.hpp
//  kawa
//
//  Created by Pierre on 03/04/2018.
//  Copyright © 2018 Pierre. All rights reserved.
//


#include <random>
#include <Kiwi_External.hpp>

#pragma GCC visibility push(default)

using namespace kiwi::external;

class Koala : public Object
{
public:
    Koala() : Object(2, 1), m_last(0.f) {}
    
    void perform(buffer_t const& input, buffer_t& output) noexcept final
    {
        // y[n] = x[n] - a[n] * x[n-1]
        size_t frames = output[0].size();
        sample_t const* in1 = input[0].data();
        sample_t const* in2 = input[1].data();
        sample_t* out = output[0].data();
        
        sample_t last = m_last;
        while(frames--)
        {
            sample_t next = *in1++;
            sample_t coef = *in2++;
            *out++ = next - coef * last;
            last = next;
        }
        m_last = last;
    }
    
private:
    sample_t m_last;
};

KIWI_LIBRARY_DECLARE(Koala)

#pragma GCC visibility pop

