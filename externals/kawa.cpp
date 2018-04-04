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

class Kawa : public Object
{
public:
    Kawa() : Object(0, 1),
    m_random_devive(),
    m_random_generator(m_random_devive()),
    m_random_distribution(-1., 1.) {}
    
    void perform(buffer_t const& input, buffer_t& output) noexcept final
    {
        size_t frames = output[0].size();
        sample_t* out = output[0].data();
        while(frames--)
        {
            *out++ = m_random_distribution(m_random_generator);
        }
    }
    
private:
    std::random_device                       m_random_devive;
    std::mt19937                             m_random_generator;
    std::uniform_real_distribution<sample_t> m_random_distribution;
};

declare(Kawa)

#pragma GCC visibility pop

