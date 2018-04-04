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

class Kroko : public Object
{
public:
    Kroko() : Object(0, 0) {}
    
    void perform(buffer_t const& input, buffer_t& output) noexcept final {}
};


#pragma GCC visibility pop

