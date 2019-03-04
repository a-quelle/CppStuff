/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mutex.cpp
 * Author: anton
 * 
 * Created on April 18, 2018, 7:34 AM
 */

#include "Mutex.h"
using namespace std;

Mutex::Mutex() {
}

Mutex::~Mutex() {
}

void Mutex::lock(){
    while(_lock.test_and_set(memory_order_acquire)){}
    //thread_fence not necessary. Everything before lock should not access critical data.
}

void Mutex::unlock(){
    _lock.clear(memory_order_release);
    //thread_fence not necessary. Everything after unlock should not access critical data.
}

