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

class Kirikou : public Object
{
public:
    Kirikou() : Object(0, 0) {}
    
    void prepare(const size_t samplerate, const size_t blocksize) { throw kerror_t("Graou"); }
    
    void perform(buffer_t const& input, buffer_t& output) noexcept final {}
};

KIWI_LIBRARY_DECLARE(Kirikou)

#pragma GCC visibility pop

