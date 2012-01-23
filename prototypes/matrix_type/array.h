#ifndef __TEMPI_ARRAY_H__
#define __TEMPI_ARRAY_H__

#include <stdint.h>
#include <typeinfo>
#include <iostream>
#include <boost/shared_array.hpp>

namespace tempi {

template <typename T>
class Array
{
    public:
        typedef std::ptrdiff_t index_t;
        Array() :
            data_(new T[0]) // of size 0
        {}
        Array(T *data) :
            data_(data)
        {}
        T *get()
        {
            return data_.get();
        }
        void set(T *data)
        {
            data_.reset(data);
        }
        bool setElement(index_t index, T value) // TODO: T &value
        {
            if (hasIndex(index))
            {
                data_[index] = value;
                return true;
            }
            return false;
        }
        void resize(index_t size)
        {
            data_.reset(new T[size]);
        }
        bool hasIndex(index_t index)
        {
            if (data_.get() == 0)
                return false;
            if (index < size())
                return true;
            return false;
        }
        T &getElement(index_t index)
        {
            if (hasIndex(index))
            {
                return data_[index];
            }
            else
            {
                std::cerr << "Invalid index " << index << std::endl;
                std::cerr << "It is very likely to crash" << std::endl;
                return data_[index];
            }
        }
        index_t size() const
        {
            if (data_.get() == 0)
                return 0;
            return sizeof(data_.get()) / sizeof(T);
        }
        void print()
        {
            std::cout << "Array<" << typeid(T).name() << ">[" << size() << "]:" << std::endl;
            for (index_t i = 0; i < size(); ++i)
            {
                std::cout << getElement(i) << " ";
            }
            std::cout << std::endl;
        }
    private:
        boost::shared_array<T> data_;
};

typedef Array<uint8_t> ArrayUint8;
typedef Array<int8_t> ArrayInt8;
typedef Array<uint16_t> ArrayUint16;
typedef Array<int16_t> ArrayInt16;
typedef Array<uint32_t> ArrayUint32;
typedef Array<int32_t> ArrayInt32;

} // end of namespace

#endif // ifndef

