#include "File.h"

File::File(const std::string& _filename) : filename(_filename)
{
}

File::File(File &&other): filename(std::move(other.filename)), file_stream(std::move(other.file_stream)), file_size(other.file_size)
{

}

File &File::operator=(File &&other)
{
    std::swap(filename, other.filename);
    std::swap(file_stream, other.file_stream);
    std::swap(file_size, other.file_size);
    return *this;
}

File::~File()
{
    close();
}

bool File::open(std::ios_base::openmode _mode)
{
    file_stream.open(filename.c_str(), _mode);
    if(file_stream.is_open()){
        file_stream.seekg(0, std::ios_base::end);
        file_size = file_stream.tellg();
        file_stream.seekg(0, std::ios_base::beg);
        return true;
    }
    return false;
}

void File::read(ustd::ByteArray &buf,size_t size)
{
    buf.reserve(size);
    file_stream.read((char*)buf.c_str(),size);
    buf.resize(file_stream.gcount());
}

void File::write(const ustd::ByteArray &buf)
{
    file_stream.write((char*)buf.c_str(),buf.size());
}

void File::write(ustd::ByteArray &_buf, size_t size, bool auto_backward)
{
    file_stream.write((char*)_buf.c_str(),size);
    if(auto_backward){
        _buf.seek(size,SEEK_CUR);
    }
}

void File::seek(size_t _pos,const whence _whence)
{
    file_stream.seekg(_pos,_whence);
}

size_t File::tell()
{
    return file_stream.tellg();
}

void File::flush()
{
    file_stream.flush();
}

void File::close()
{
    if(file_stream.is_open()){
        file_stream.close();
    }   
}

size_t File::size()
{
    return file_size;
}

bool File::eof()
{
    return file_stream.eof();
}
