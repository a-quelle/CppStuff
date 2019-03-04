#include "LargeNumber.h"
#include <algorithm>
using namespace std;

size_t LargeNumber::getSize() const
{
    return size;
}    

const uint8_t* LargeNumber::getData() const{
    return data;
}

LargeNumber::~LargeNumber(){
    delete[] data;
}

LargeNumber::LargeNumber(){}

LargeNumber::LargeNumber(unsigned char* inputData, size_t size): size(size), data(inputData){};

LargeNumber::LargeNumber(const LargeNumber& orig){
    size = orig.size;
    data =  new unsigned char[size];
    copy(orig.data, orig.data + size, data);
}

LargeNumber::LargeNumber(LargeNumber&& orig){
    data = orig.data;
    orig.data = nullptr;
    size = orig.size;
}

LargeNumber& LargeNumber::operator=(LargeNumber& rhs){
    size = rhs.size;
    delete[] data;
    data =  new unsigned char[size];
    copy(rhs.data, rhs.data + size, data);
    return *this;
}

LargeNumber& LargeNumber::operator=(LargeNumber&& rhs){
    delete[] data;
    data = rhs.data;
    rhs.data = nullptr;
    size = rhs.size;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const LargeNumber& n){
    for(int i = 0; i < n.size; i++)
        os << static_cast<char>(n.data[i]);
    return os;
    }

bool operator<(const LargeNumber& a, const LargeNumber& b){
    if(a.size > b.size) return false;
    if(a.size < b.size) return true;
    for(int i = a.size-1; i >= 0; i--){
        if(b.data[i] > a.data[i]) 
            return true;
        else if(b.data[i] < a.data[i])
            return false;
    }
    return false;
}

bool operator>(const LargeNumber& a, const LargeNumber& b){
    if(a.size < b.size) return false;
    if(a.size > b.size) return true;
    for(int i = a.size-1; i >= 0; i--){
        if(b.data[i] > a.data[i]) 
            return false;
        else if(b.data[i] < a.data[i])
            return true;
    }        
    return false;     
}

bool operator==(const LargeNumber& a, const LargeNumber& b){
        if(a.size > b.size) return false;
        if(a.size < b.size) return false;
        for(int i = 0; i < a.size; i++)
            if(a.data[i] != b.data[i]) return true;
        return false;        

}

size_t getDataSize(uint8_t *data, size_t capacity){    
    while(! data[capacity-1])
        capacity--;
        return capacity;
}



LargeNumber& LargeNumber::operator++(){
    uint8_t overflow = 0;
    int counter = -1;
    while(++counter < size && ++data[counter] == 0)
        __noop;
    if(counter == size){
        uint8_t *tempArray = new uint8_t[size+1];
        copy(data, data + size, tempArray);
        tempArray[size] = 1;
        delete[] data;
        data = tempArray;
        ++size;
    }
    return *this;    
}

LargeNumber& LargeNumber::operator--(){
    uint8_t overflow = 0;
    int counter = -1;
    while(++counter < size && --data[counter] == 0xff)
        __noop;
    size = getDataSize(data, size);
    return *this;    
}

LargeNumber operator+(const LargeNumber& lhs, const LargeNumber& rhs){
    LargeNumber out;
    size_t shortSize, longSize;
    bool lhsLarger;
    if(lhs.size > rhs.size) {
        longSize = lhs.size;
        shortSize = rhs.size;
        lhsLarger = true;
    }
    else {
        longSize = rhs.size;
        shortSize = lhs.size;
        lhsLarger = false;
    }
    out.data = new unsigned char[longSize+1];
    unsigned short tempsum = 0;
    unsigned char overflow = 0;
    for(int i = 0; i < shortSize; i++){
        tempsum = (unsigned short)lhs.data[i] + (unsigned short)rhs.data[i] + overflow;
        out.data[i] = tempsum;
        overflow = (unsigned char)(tempsum >> 8);
    }
    if(lhsLarger){
        for(int i = shortSize; i < longSize; i++){
            tempsum = (uint16_t)lhs.data[i] + overflow;
            out.data[i] = tempsum;
            overflow = (unsigned char)(tempsum >> 8);
        }
    }else{
        for(int i = shortSize; i < longSize; i++){
            tempsum = (uint16_t)rhs.data[i] + overflow;
            out.data[i] = tempsum;     
            overflow = (unsigned char)(tempsum >> 8);   
        }
    }
    out.data[longSize] = overflow;
    if(overflow){
        out.size = longSize+1;
    } else{
        out.size = longSize;
    }
    return out;
}

LargeNumber operator-(const LargeNumber& lhs, const LargeNumber& rhs){
    LargeNumber out;
    size_t shortSize, longSize;
    bool lhsLarger;
    if(lhs.size > rhs.size) {
        longSize = lhs.size;
        shortSize = rhs.size;
        lhsLarger = true;
    }
    else {
        longSize = rhs.size;
        shortSize = lhs.size;
        lhsLarger = false;
    }
    out.data = new uint8_t[longSize];
    uint16_t tempsum = 0;
    uint8_t overflow = 0;
    for(int i = 0; i < shortSize; i++){
        tempsum = (uint16_t)lhs.data[i] - (uint16_t)rhs.data[i] - overflow;
        out.data[i] = tempsum;
        overflow = -(uint8_t)(tempsum >> 8);
    }
    if(lhsLarger){
        for(int i = shortSize; i < longSize; i++){
            tempsum = (uint16_t)lhs.data[i] - overflow;
            out.data[i] = tempsum;
            overflow = -(uint8_t)(tempsum >> 8);
        }
    }else{
        for(int i = shortSize; i < longSize; i++){
            tempsum = -(uint16_t)rhs.data[i] - overflow;
            out.data[i] = tempsum;        
            overflow = -(uint8_t)(tempsum >> 8);
        }
    }
    
    out.size = getDataSize(out.data, longSize);
    return out;
}

bool LargeNumber::halve(){
    if(data[0] & 1) return false;
    uint16_t tempValue = 0;
    uint8_t newOverflow = 0, prevOverflow = 0;
    for(int i = size-1; i >= 0; i--)
    {
        prevOverflow = newOverflow;
        newOverflow = (data[i] & 1) << 7;
        data[i] = (data[i] >> 1) + prevOverflow;
    }
    size = getDataSize(data, size);
    return true;
}

void LargeNumber::twice(){
    uint8_t prevOverflow = 0, newOverflow = 0;
    uint8_t* tempArray;
    if(data[size-1]&0x80){
        tempArray = new uint8_t[size+1];
        copy(data, data+size, tempArray);
        tempArray[size] = 0;
        delete[] data;
        size = size+1;
    }
    else 
        tempArray = data;
    data = tempArray;
    for(int i = 0; i < size; i++)
    {
        prevOverflow = newOverflow;
        newOverflow = (data[i] & 128) >> 7;
        data[i] = (data[i] << 1) + prevOverflow;
    }
}

LargeNumber operator*(const LargeNumber& lhs, LargeNumber rhs){
    LargeNumber out(lhs);
    LargeNumber one({(char)1});
    LargeNumber acc({char(0)});
    while(rhs > one){
        if(rhs.halve()){
            out.twice();            
        } else{
            --rhs;
            acc = out + acc;
        }
    }
    out = out + acc;
    return out;
}

void print(LargeNumber& number){
    for(int i = 0; i < number.getSize(); i++)
            std::cout << (int) number.getData()[i] << ",";
        std::cout << std::endl;
}

LargeNumber operator%(const LargeNumber& lhs, const LargeNumber& rhs){
    LargeNumber out(lhs);
    if(rhs.size > out.size) return out;
    if(rhs.size == out.size) {
        LargeNumber::modHelper(out, rhs);
        return out;    
    }
    LargeNumber mod;
    mod.size = out.size;
    size_t diff = out.size - rhs.size;
    uint8_t* modData = new uint8_t[out.size]{0};
    mod.data = modData;
    copy(rhs.data, rhs.data + rhs.size, mod.data + diff);
    while(rhs < out){
        LargeNumber::modHelper(out, mod);
        ++mod.data;
        --mod.size;        
    }
    mod.data = modData;
    return out;       
}



void LargeNumber::modHelper(LargeNumber& out, const LargeNumber& mod){
    while(out > mod){
        uint16_t tempsum = 0;
        uint8_t overflow = 0;
        for(int i = 0; i < mod.size; i++){
            tempsum = (uint16_t)out.data[i] - (uint16_t)mod.data[i] - overflow;
            out.data[i] = tempsum;
            overflow = -(uint8_t)(tempsum >> 8);
        }
        for(int i = mod.size; i < out.size; i++){
            tempsum = (uint16_t)out.data[i] - overflow;
            out.data[i] = tempsum;
            overflow = -(uint8_t)(tempsum >> 8);
        }
        out.size = getDataSize(out.data, out.size);
    }
}

LargeNumber LargeNumber::modularPower(const LargeNumber& base, LargeNumber exp, const LargeNumber& modulus){
    LargeNumber out(base);
    LargeNumber one({(char) 1});
    LargeNumber acc(one);
    while(exp > one){
        if(exp.halve()){
            out = out*out; 
            out = out % modulus;
        } else{
            --exp;
            acc = acc * out;
            acc = acc % modulus;
        }
    }    
    out = out*acc;
    out = out % modulus;
    return out;
}
