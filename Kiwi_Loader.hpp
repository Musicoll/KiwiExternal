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

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <dlfcn.h>
#endif

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
#ifdef _WIN32
                    lib_t lib_handle = LoadLibrary(TEXT(fullpath.c_str()));
#else
                    lib_t lib_handle = dlopen(fullpath.c_str(), RTLD_LOCAL|RTLD_LAZY);
#endif
                    if (!lib_handle)
                    {
                        throw kerror_t(std::string("Unable to open library: ") + fullpath);
                    }
                    else
                    {
                        olib.lib = lib_handle;
                    }
                }
                if(!olib.ctor)
                {
#ifdef _WIN32
                    object_creator* ctor = (object_creator*)GetProcAddress(olib.lib, "create_object");
#else
                    object_creator* ctor = (object_creator*)dlsym(olib.lib, "create_object");
#endif
                    
                    if(!ctor)
                    {
                        throw kerror_t(std::string("Unable to find create_object method from: ") + fullpath);
                    }
                    else
                    {
                        olib.ctor = ctor;
                    }
                }
                if(!olib.dspr)
                {
#ifdef _WIN32
                    object_creator* ctor = (object_creator*)GetProcAddress(olib.lib, "free_object");
#else
                    object_disposer* dspr = (object_disposer*)dlsym(olib.lib, "free_object");
#endif
                    if(!dspr)
                    {
                        throw kerror_t(std::string("Unable to find free_object method from: ") + fullpath);
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
#ifndef _WIN32
                    if(olib.second.lib)
                    {
                        if (dlclose(olib.second.lib) != 0)
                        {
                            
                        }
                    }
#endif
                    assert(olib.second.objs.empty() && "objects not freed");
                }
            }
        private:
            
            static Loader& get()
            {
                static Loader singleton;
                return singleton;
            }
            
#ifdef _WIN32
            typedef HINSTANCE lib_t;
#else
            typedef void* lib_t;
#endif
            struct object_lib
            {
                lib_t               lib = nullptr;
                object_creator*     ctor = nullptr;
                object_disposer*    dspr = nullptr;
                std::set<Object*>   objs;
            };
            
            std::map<std::string, object_lib> libs;
        };
    }
}



