//
//  kawa.hpp
//  kawa
//
//  Created by Pierre on 03/04/2018.
//  Copyright © 2018 Pierre. All rights reserved.
//

#include <iostream>
#include <Kiwi_Loader.hpp>

using namespace kiwi::external;

static int load_object(const std::string& name)
{
    std::cout << "test " << name << "...";
    Object* obj;
    try
    {
        obj = Loader::create("", name);
    }
    catch (error_t& e)
    {
        std::cerr << e.what();
        return 1;
    }
    buffer_t inputs{obj->getNumberOfInputs(), {64, 0}};
    buffer_t outputs{obj->getNumberOfOutputs(), {64, 0}};
    bool error = false;
    try
    {
        obj->prepare(44100, 64);
    }
    catch (error_t& e)
    {
        std::cerr << e.what();
    }
    
    if(!error)
    {
        int nticks = 128;
        while (--nticks)
        {
            obj->perform(inputs, outputs);
        }
    }
    
    try
    {
        Loader::dispose(obj);
    }
    catch (error_t& e)
    {
        std::cerr << e.what();
        return 1;
    }
    std::cout << "ok\n";
    return 0;
}

int main(int argc, const char * argv[])
{
    int error = 0;
    error += load_object("koala");
    error += load_object("kawa");
    return error;
}
