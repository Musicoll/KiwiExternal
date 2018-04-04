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

static int load_object(const std::string& path, const std::string& name)
{
    std::cout << "test " << name << "...";
    Object* obj;
    try
    {
        obj = Loader::create(path, name);
    }
    catch (error_t& e)
    {
        std::cerr << e.what() << "\n";
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
        std::cerr << e.what() << "\n";
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
        std::cerr << e.what() << "\n";
        return 1;
    }
    std::cout << "ok\n";
    return 0;
}

int main(int argc, const char * argv[])
{
    int error = 0;
    std::string const path = argc > 1 ?  argv[1] : (argc ? argv[0] : "");

    std::cout << "looking in " << path << ":\n";
    error += load_object(path, "koala");
    error += load_object(path, "kawa");
    return error;
}
