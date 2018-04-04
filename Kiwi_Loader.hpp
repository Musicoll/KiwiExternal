//
//  Kiwi_External.h
//  Kiwi_External
//
//  Created by Pierre on 04/04/2018.
//  Copyright © 2018 Pierre. All rights reserved.
//

#pragma once

#include "Kiwi_External.hpp"
#include <map>
#include <set>
#include <cassert>
#include <dlfcn.h>

namespace kiwi
{
    namespace external
    {
        class Loader
        {
        public:
            
            static Object* create(std::string const& path, std::string const& name)
            {
                const std::string fullpath = path.empty() ? name + ".kiwix" : path + "/" + name + ".kiwix";
                auto& olib = get().libs[fullpath];
                if(!olib.lib)
                {
                    void* lib_handle = dlopen(fullpath.c_str(), RTLD_LOCAL|RTLD_LAZY);
                    if (!lib_handle)
                    {
                        throw kerror_t(std::string("[") + std::string(__FILE__) + std::string("] main: Unable to open library: ") + std::string(dlerror()));
                    }
                    else
                    {
                        olib.lib = lib_handle;
                    }
                }
                if(!olib.ctor)
                {
                    object_creator* ctor = (object_creator*)dlsym(olib.lib, "createObject");
                    if(!ctor)
                    {
                        throw kerror_t(std::string("[") + std::string(__FILE__) + std::string("] main: Unable to find createObject method: ") + std::string(dlerror()));
                    }
                    else
                    {
                        olib.ctor = ctor;
                    }
                }
                if(!olib.dspr)
                {
                    object_disposer* dspr = (object_disposer*)dlsym(olib.lib, "freeObject");
                    if(!dspr)
                    {
                        throw kerror_t(std::string("[") + std::string(__FILE__) + std::string("] main: Unable to find freeObject method: ") + std::string(dlerror()));
                    }
                    else
                    {
                        olib.dspr = dspr;
                    }
                }
                Object* obj = olib.ctor();
                if(obj)
                {
                    olib.objs.insert(obj);
                }
                return obj;
            }
            
            
            static void dispose(Object* obj)
            {
                for(auto& olib : get().libs)
                {
                    const size_t n = olib.second.objs.size();
                    if(olib.second.objs.erase(obj))
                    {
                        olib.second.dspr(obj);
                        assert(olib.second.objs.size() == n-1);
                        return;
                    }
                }
                throw kerror_t("can't find the disposer.");
            }
            
            ~Loader()
            {
                for(auto& olib : libs)
                {
                    if(olib.second.lib)
                    {
                        if (dlclose(olib.second.lib) != 0)
                        {
                            
                        }
                    }
                    assert(olib.second.objs.empty() && "objects not freed");
                }
            }
        private:
            
            static Loader& get()
            {
                static Loader singleton;
                return singleton;
            }
            
            struct object_lib
            {
                void*          lib = nullptr;
                object_creator* ctor = nullptr;
                object_disposer* dspr = nullptr;
                std::set<Object*> objs;
            };
            std::map<std::string, object_lib> libs;
        };
    }
}



