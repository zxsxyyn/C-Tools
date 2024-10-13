#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <string>
#include "ByteArray.h"

class File {
private:
    std::string filename;
    size_t file_size=0;
    std::fstream file_stream;
public:
    typedef std::ios_base::openmode openmode;
    static const openmode R=std::ios_base::in;
    static const openmode W=std::ios_base::out;
    static const openmode RW=R|W;
    static const openmode A=std::ios_base::app;
    static const openmode TRUCK=std::ios_base::trunc;
    static const openmode BINARY=std::ios_base::binary;

    typedef std::ios_base::seekdir whence;
    static const whence SET=std::ios_base::beg;
    static const whence CUR=std::ios_base::cur;
    static const whence END=std::ios_base::end;
    inline static const size_t default_size=65535; 
    explicit File(const std::string& _filename);
    File(const File& other)=delete;
    File& operator=(const File& other)=delete;
    File(File&& other);
    File& operator=(File&& other);
    ~File();
    bool open(const openmode _mode);
    //如果不指定size，则默认读取65535字节
    void read(ustd::ByteArray& _buf,size_t _size=default_size);
    void write(const ustd::ByteArray& _buf);
    void write(ustd::ByteArray& _buf,size_t size,bool auto_backward=false);
    void seek(size_t _pos,const whence _whence=SET);
    size_t tell();
    void flush();
    size_t size();
    void close();
    bool eof();
};
#endif