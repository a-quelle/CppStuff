#pragma once
#include <iostream>
#include "AnyClass.h"

constexpr auto MAX_CONSTRUCTOR_ARGS = 10;

template<typename...>
struct is_constructible;

template<typename T, std::size_t... Ns>
struct is_constructible<T, std::index_sequence<Ns...>> : std::is_constructible<T, AnyClass<T, Ns>...>
{};

template <typename T, std::size_t... Ns>
constexpr size_t getCnstructorSize (std::index_sequence<Ns...>) noexcept {
	size_t size = 0;
	((is_constructible<T, std::make_index_sequence<Ns>>::value? size = Ns : size), ...);
	return size;
}

class Injector {  

    template <typename TParent, std::size_t t>
        friend class AnyClass;
    
    Injector();

    template<typename T, size_t... Args>
    static T inline callConstructor(std::index_sequence<Args...>){              
        
        return T(AnyClass<T, Args>{}...);
    }

    template <typename T>
    struct Singleton{        

        static T* getInstance()
        {
            static T t = makeObject<T>();
            return &t;
        }
        static bool value;
        Singleton();
        void operator = (const T&);
    };

    public:

    template<typename T>
    static void makeSingleton(){
        Singleton<T>::value = true;
    }

    template<typename T>
    static inline T makeObject(){
        return callConstructor <T>(
            std::make_index_sequence< getCnstructorSize<T>
            (std::make_index_sequence<MAX_CONSTRUCTOR_ARGS>{})>{}
        );
    }
};

template<typename T>
bool Injector::Singleton<T>::value = false;