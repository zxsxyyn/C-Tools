#ifndef BYTEARRAY_H
#define BYTEARRAY_H
#include <string>
#include <iostream>

namespace ustd
{
    class ByteArray
    {
    private:
        friend class TcpSocket;
        inline static const int DEFALTCAPACITY=15; //默认容量
        char* array;//指向数据的指针
        unsigned short thecapacity=DEFALTCAPACITY;//容量
        unsigned short thesize=0;//已经存放的字节数
        unsigned short readIndex=0;//读取索引
        void resize(unsigned short);
        void advance(unsigned short value);
    public:
        inline static const unsigned short MAX_CAPACITY=65535;
        ByteArray();
        ByteArray(unsigned short capacity_);
        ByteArray(const ByteArray& other);
        ByteArray(ByteArray&& other);
        ByteArray& operator=(const ByteArray& other);
        ByteArray& operator=(ByteArray&& other);
        ~ByteArray();

        void reserve(unsigned short capacity);

        ByteArray& operator<<(const std::string& str);
        ByteArray& operator<<(const char* str);
        ByteArray& operator<<(short value);
        ByteArray& operator<<(int value);
        ByteArray& operator<<(long value);
        ByteArray& append(const char* buf,unsigned short size);

        void compact();//压缩空间
        short readShort();
        int readInt();
        long readLong();
        bool read(void* buf,unsigned short size);
        std::string read(unsigned short size);

        ByteArray& operator>>(short& value);
        ByteArray& operator>>(int& value);
        ByteArray& operator>>(long& value);
        ByteArray& operator>>(char& c);
        
        unsigned short size()const;
        unsigned short capacity()const;
        void clear();
        bool empty()const;
        const char* c_str()const;

        friend std::ostream& operator<<(std::ostream& os,const ByteArray& array);
    };
}

#endif