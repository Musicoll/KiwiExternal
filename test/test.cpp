//
//  kawa.hpp
//  kawa
//
//  Created by Pierre on 03/04/2018.
//  Copyright © 2018 Pierre. All rights reserved.
//

#include <iostream>
#include <Kiwi_Loader.hpp>

static int load_object(const std::string& path, const std::string& name)
{
    std::cout << "test " << name << "...";
    kiwi::external::Object* obj;
    try
    {
        obj = kiwi::external::Loader::create(path, name);
    }
    catch (kiwi::external::kerror_t& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    kiwi::external::buffer_t inputs{obj->getNumberOfInputs(), {64, 0}};
    kiwi::external::buffer_t outputs{obj->getNumberOfOutputs(), {64, 0}};
    bool error = false;
    try
    {
        obj->prepare(44100, 64);
    }
    catch (kiwi::external::kerror_t& e)
    {
        error = true;
        std::cerr << e.what() << "...";
    }
    
    if(!error)
    {
        int nticks = 128;
        while (--nticks)
        {
            obj->perform(inputs, outputs);
        }
        std::cout << "perform...";
    }
    
    try
    {
        kiwi::external::Loader::dispose(obj);
    }
    catch (kiwi::external::kerror_t& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    std::cout << "ok\n";
    return 0;
}

int main(int argc, const char * argv[])
{
    std::string const path = argc > 1 ?  argv[1] : (argc ? argv[0] : "");
    std::cout << "looking in " << path << ":\n";
    assert(load_object(path, "koala") == 0);
    assert(load_object(path, "kawa") == 0);
    assert(load_object(path, "kirikou") == 0);
    return 0;
}
