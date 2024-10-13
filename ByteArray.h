#ifndef BYTEARRAY_H
#define BYTEARRAY_H
#include <string>
#include <iostream>
#include <cstring>

#define SEEKB_SET 0
#define SEEKB_CUR 1
#define SEEKB_END 2
namespace ustd
{
    class ByteArray
    {
    private:
        friend class TcpSocket;
        inline static const int DEFALTCAPACITY=15; //默认容量
        char* array;//指向数据的指针
        size_t thecapacity=DEFALTCAPACITY;//容量
        size_t thesize=0;//已经存放的字节数
        size_t readIndex=0;//读取索引
        
        void advance(size_t value);
    public:
        //inline static const size_t MAX_CAPACITY;
        ByteArray();
        ByteArray(const char* buf,size_t size_);
        ByteArray(size_t capacity_);
        ByteArray(const ByteArray& other);
        ByteArray(ByteArray&& other);
        ByteArray& operator=(const ByteArray& other);
        ByteArray& operator=(ByteArray&& other);
        ~ByteArray();

        char& operator[](size_t index);
        const char& operator[](size_t index)const;

        void reserve(size_t capacity);
        void resize(size_t);
        template<typename T>
        ByteArray& operator<<(const T& value);//对于结构体，注意指针成员以及内存对齐问题
        ByteArray& append(const char* buf,size_t size);
        //auto_backward=true时,会自动将other的流指针后移size个字节
        ByteArray& append(ByteArray& other,size_t size,bool auto_backward=false);
        ByteArray& operator<<(const std::string& str);
        ByteArray& operator<<(const char* str);
        ByteArray& operator<<(const ByteArray& other);

        template<typename T>
        ByteArray& operator>>(T& value);//对于结构体，注意指针成员以及内存对齐问题
        void compact();//压缩已经存放的字节，将剩余空间移动到头部
        bool read(void* buf,size_t size);
        std::string read(size_t size);
        
        size_t size()const;
        size_t capacity()const;
        void clear();
        bool empty()const;
        //返回的是当前流位置的指针，而不是起始点
        const char* c_str()const;
        /*SEEKB_SET 开头位置
        SEEKB_CUR 当前位置
        SEEKB_END 结尾位置*/
        bool seek(long pos,int whence=0);
        size_t tell()const;

        friend std::ostream& operator<<(std::ostream& os,const ByteArray& array);
    };


    template <typename T>
    inline ByteArray &ustd::ByteArray::operator<<(const T &value)
    {
        unsigned pos = thesize;
        resize(thesize + sizeof(T));
        memcpy(array + pos, &value, sizeof(T));
        return *this;
    }
    
    template <typename T>
    inline ByteArray &ByteArray::operator>>(T &value)
    {
        if (size() < sizeof(T))
        {
            throw("剩余空间不足以读取short");
            return *this;
        }
        memcpy(&value, array + readIndex, sizeof(T));
        advance(sizeof(T));
        return *this;
    }
}

#endif