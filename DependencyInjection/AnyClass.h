#pragma once

#include <iostream>
#include "injector.h"

template <typename TParent, std::size_t>
struct AnyClass {	
    
    template<typename T, typename =
		std::enable_if_t < !std::is_same<TParent, T>::value >
	> inline operator T() {        
        return Injector::makeObject<T>();
    }

    template<typename T, typename =
		std::enable_if_t < !std::is_same<TParent, T>::value >
	> inline operator T*() {
        if (Injector::Singleton<T>::value)
            return Injector::Singleton<T>::getInstance();
        
        return nullptr;
    }
};
