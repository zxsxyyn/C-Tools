#include "ByteArray.h"
#include <cstring>
#include <iomanip>

void ustd::ByteArray::resize(unsigned short newsize)
{
    if (newsize > thecapacity)
    {
        reserve(newsize * 2);
    }
    thesize = newsize;
}

void ustd::ByteArray::advance(unsigned short value)
{
    readIndex+=value;
    if(readIndex>=thesize/2){
        compact();
    }
}

ustd::ByteArray::ByteArray()
{
    array = new char[DEFALTCAPACITY];
}

ustd::ByteArray::ByteArray(unsigned short capacity) : thecapacity(capacity)
{
    array = new char[thecapacity];
}

ustd::ByteArray::ByteArray(const ByteArray &other) : thecapacity(other.thecapacity), thesize(other.thesize),
readIndex(other.readIndex)
{
    array = new char[thecapacity];
    memcpy(array, other.array, thesize);
}

ustd::ByteArray::ByteArray(ByteArray &&other) : array(other.array),thecapacity(other.thecapacity), thesize(other.thesize),
readIndex(other.readIndex)
{
    other.array = nullptr;
    other.thecapacity=0;
    other.thesize=0;
    other.readIndex=0;
}

ustd::ByteArray &ustd::ByteArray::operator=(const ByteArray &other)
{
    ByteArray temp(other);
    std::swap(*this,temp);
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator=(ByteArray &&other)
{
    std::swap(array,other.array);
    std::swap(thecapacity,other.thecapacity);
    std::swap(thesize,other.thesize);
    std::swap(readIndex,other.readIndex);
    return *this;
}

ustd::ByteArray::~ByteArray()
{
    delete[] array;
    array = nullptr;
}

void ustd::ByteArray::reserve(unsigned short capacity)
{
    if (capacity <= thecapacity)
    {
        return;
    }
    thecapacity = capacity;
    char *old = array;
    array = new char[thecapacity];
    memcpy(array, old, thesize);
    delete old;
}

ustd::ByteArray &ustd::ByteArray::operator<<(const std::string &str)
{
    unsigned pos = thesize;
    resize(thesize + str.length());
    memcpy(array + pos, str.c_str(), str.length());
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator<<(const char *str)
{
    unsigned strsize = 0;
    const char *ptr = str;
    while (*ptr++)
    {
        strsize++;
    }
    unsigned pos = thesize;
    resize(thesize + strsize);
    memcpy(array + pos, str, strsize);
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator<<(short value)
{
    unsigned pos = thesize;
    resize(thesize + sizeof(short));
    memcpy(array + pos, &value, sizeof(short));
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator<<(int value)
{
    unsigned pos = thesize;
    resize(thesize + sizeof(int));
    memcpy(array + pos, &value, sizeof(int));
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator<<(long value)
{
    unsigned pos = thesize;
    resize(thesize + sizeof(long));
    memcpy(array + pos, &value, sizeof(long));
    return *this;
}

ustd::ByteArray &ustd::ByteArray::append(const char *buf, unsigned short size)
{
    unsigned pos = thesize;
    resize(thesize + size);
    memcpy(array, buf, size);
    return *this;
}

void ustd::ByteArray::compact()
{
    unsigned short surpsize=size();
    memmove(array,array+readIndex,surpsize);
    readIndex=0;
    resize(surpsize);
}

short ustd::ByteArray::readShort()
{
    if(size()<sizeof(short)){
        throw("剩余空间不足以读取short");
        return 0;
    }
    short value;
    memcpy(&value,array+readIndex,sizeof(short));
    advance(sizeof(short));
    return value;
}

int ustd::ByteArray::readInt()
{
    if(size()<sizeof(int)){
        throw("剩余空间不足以读取int");
        return 0;
    }
    int value;
    memcpy(&value,array+readIndex,sizeof(int));
    advance(sizeof(int));
    return value;
}

long ustd::ByteArray::readLong()
{
    if(size()<sizeof(long)){
        throw("剩余空间不足以读取long");
        return 0;
    }
    long value;
    memcpy(&value,array+readIndex,sizeof(long));
    advance(sizeof(long));
    return value;
}

bool ustd::ByteArray::read(void *buf, unsigned short size_)
{
    if(size()<size_){
        return false;
    }
    memcpy(buf,array+readIndex,size_);
    advance(size_);
    return true;
}

std::string ustd::ByteArray::read(unsigned short size_)
{
    if(size()<size_)
        return std::string();
    std::string str(array,size_);
    advance(size_);
    return str;
}

ustd::ByteArray &ustd::ByteArray::operator>>(short &value)
{
    if(size()<sizeof(short)){
        throw("剩余空间不足以读取short");
        return *this;
    }
    memcpy(&value,array+readIndex,sizeof(short));
    advance(sizeof(short));
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator>>(int &value)
{
    if(size()<sizeof(int)){
        throw("剩余空间不足以读取short");
        return *this;
    }
    memcpy(&value,array+readIndex,sizeof(int));
    advance(sizeof(int));
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator>>(long &value)
{
    if(size()<sizeof(long)){
        throw("剩余空间不足以读取short");
        return *this;
    }
    memcpy(&value,array+readIndex,sizeof(long));
    advance(sizeof(long));
    return *this;
}

ustd::ByteArray &ustd::ByteArray::operator>>(char &c)
{
    if(size()<sizeof(char)){
        throw("剩余空间不足以读取short");
        return *this;
    }
    memcpy(&c,array+readIndex,sizeof(char));
    advance(sizeof(char));
    return *this;
}

unsigned short ustd::ByteArray::size() const
{
    return thesize-readIndex;
}

unsigned short ustd::ByteArray::capacity() const
{
    return thecapacity;
}

void ustd::ByteArray::clear()
{
    thesize=0;
    readIndex=0;
}

bool ustd::ByteArray::empty() const
{
    return size()==0;
}

const char *ustd::ByteArray::c_str() const
{
    return array;
}

namespace ustd
{
    std::ostream& operator<<(std::ostream &os, const ustd::ByteArray &byteArray)
    {
        os << '[';
        for (unsigned short i = byteArray.readIndex; i < byteArray.thesize-1; ++i)
        {
            // 设置为16进制输出，每个字节占两位，前导补0
            os << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(static_cast<unsigned char>(byteArray.array[i])) << " ";
        }
        os << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(static_cast<unsigned char>(byteArray.array[byteArray.thesize]));
        os << ']';
        // 恢复流的格式
        os << std::dec; // 回到十进制模式
        return os;
    }
}
