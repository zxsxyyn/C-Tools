#include "ByteArray.h"
#include <cstring>
#include <iomanip>

void ustd::ByteArray::resize(size_t newsize)
{
    if (newsize > thecapacity)
    {
        reserve(newsize * 2);
    }
    thesize = newsize;
}

void ustd::ByteArray::advance(size_t value)
{
    readIndex+=value;
    /*
    if(readIndex>=thesize/2){
        compact();
    }
    */
}

ustd::ByteArray::ByteArray()
{
    array = new char[DEFALTCAPACITY];
}

ustd::ByteArray::ByteArray(const char *buf, size_t size_):thecapacity(size_),thesize(size_)
{
    array = new char[size_];
    memcpy(array, buf, size_);
}

ustd::ByteArray::ByteArray(size_t capacity) : thecapacity(capacity)
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

char &ustd::ByteArray::operator[](size_t index)
{
    if (index >= thesize)
    {
        throw("数组越界");
    }
    return array[index];
}
const char &ustd::ByteArray::operator[](size_t index) const
{
    if (index >= thesize)
    {
        throw("数组越界");
    }
    return array[index];
}

void ustd::ByteArray::reserve(size_t capacity)
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

ustd::ByteArray &ustd::ByteArray::append(const char *buf, size_t size)
{
    unsigned pos = thesize;
    resize(thesize + size);
    memcpy(array, buf, size);
    return *this;
}

void ustd::ByteArray::compact()
{
    size_t surpsize=size();
    memmove(array,array+readIndex,surpsize);
    readIndex=0;
    resize(surpsize);
}

bool ustd::ByteArray::read(void *buf, size_t size_)
{
    if(size()<size_){
        return false;
    }
    memcpy(buf,array+readIndex,size_);
    advance(size_);
    return true;
}

std::string ustd::ByteArray::read(size_t size_)
{
    if(size()<size_)
        return std::string();
    std::string str(array,size_);
    advance(size_);
    return str;
}

size_t ustd::ByteArray::size() const
{
    return thesize-readIndex;
}

size_t ustd::ByteArray::capacity() const
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

bool ustd::ByteArray::seek(long pos, int whence)
{
    switch (whence)
    {
    case SEEKB_SET:
        if (pos > thesize)
        {
            return false;
        }
        readIndex = pos;
        break;
    case SEEKB_CUR:
        if ((pos + readIndex) > thesize)
        {
            return false;
        }
        readIndex += pos;
        break;
    case SEEKB_END:
        if (pos > 0)
        {
            return false;
        }
        readIndex = thesize + pos;
        break;
    default:
        return false;
    }
    return true;
}

size_t ustd::ByteArray::tell() const
{
    return readIndex;
}

namespace ustd
{
    std::ostream& operator<<(std::ostream &os, const ustd::ByteArray &byteArray)
    {
        os << '[';
        for (size_t i = byteArray.readIndex; i < byteArray.thesize-1; ++i)
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
