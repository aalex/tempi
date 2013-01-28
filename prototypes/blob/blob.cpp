#include <iostream>
#include <stdlib.h>
#include <cstring>

/**
 * Dynamic-size binary large object.
 */
class Blob
{
    public:
        Blob(int initial_allocation_size = 1);
        ~Blob();
        /**
         * Appends a single byte to the large object.
         */
        void append(char data);
        /**
         * Appends some bytes to the large object.
         *
         * Note that the sizeof() operator does not give you the number of
         * elements in an array, it gives you the number of bytes a thing
         * occupies in memory.
         * This causes some confusion, because the first time many beginners
         * try to use sizeof() is often with a character string, and a char
         * is always one byte long. An int, however, will be something different.
         */
        void append(const char *data, int length);
        /**
         * Returns the number of bytes stored.
         */
        int getSize() const;
        /**
         * Returns the number of bytes allocated for this large object.
         * Should be a power of two.
         */
        int getAllocated() const;
        /**
         * Clears the blob, free its memory and sets its contents to zero.
         */
        void clear();
        /**
         * Retrieves the pointer to the bytes.
         */
        char * get() const;
        /**
         * Prints potentially usefull - but very verbose - information about
         * this blob.
         */
        void debugPrint() const;
        static int findNextLargerPowerOfTwo(int n);
    private:
        char *value_;
        int size_;
        int allocated_;
        // should be a power of two
        int initial_allocation_size_;
        /**
         * Returns 256, 512, 1024, 2048, etc... that is closest smaller to target.
         */
        int findSizeToStore(int target);
        void increaseSize(int size);
};


Blob::Blob(int initial_allocation_size) :
    size_(0)
{
    initial_allocation_size_ = findNextLargerPowerOfTwo(initial_allocation_size);
    allocated_ = initial_allocation_size_;
    value_ = (char *) malloc(initial_allocation_size_);
    memset(value_, 0, initial_allocation_size_);
}

char * Blob::get() const
{
    return value_;
}

Blob::~Blob()
{
    free(value_);
}

void Blob::append(char data)
{
    char array[1];
    array[0] = data;
    append(array, 1);
}

void Blob::append(const char *data, int length)
{
    int newsize = size_ + length;
    if (newsize > allocated_)
    {
        int resize_to = findSizeToStore(newsize);
        increaseSize(resize_to);
    }
    memcpy((void *) (value_ + size_), (const void *) data, length);
    //memcpy((void *) (& value_[size_]), (const void *) data, length);
    size_ = size_ + length;
}

void Blob::increaseSize(int size)
{
    int how_many_byte_more = size - allocated_;
    value_ = (char *) realloc((void *) value_, size);
    memset(value_ + allocated_, 0, how_many_byte_more);
    allocated_ = size;
}

int Blob::findSizeToStore(int target)
{
    int ret = allocated_;
    while (ret < target)
        ret = ret * 2;
    return ret;
}

int Blob::findNextLargerPowerOfTwo(int n)
{
    int ret = 1;
    while (ret < n)
        ret *= 2;
    return ret;
}

int Blob::getSize() const
{
    return size_;
}

int Blob::getAllocated() const
{
    return allocated_;
}

void Blob::clear()
{
    free(value_);
    value_ = (char *) malloc(initial_allocation_size_);
    memset(value_, 0, initial_allocation_size_);
    allocated_ = initial_allocation_size_;
}

void Blob::debugPrint() const
{
    std::cout << "-----------" << std::endl;
    std::cout << "Blob info: " << std::endl;
    std::cout << " size: " << getSize() << std::endl;
    std::cout << " allocated: " << getAllocated() << std::endl;
    std::cout << " value: ";
    for (int i = 0; i < getSize(); i++)
    {
        std::cout << ( (int) value_[i] ) << " ";
    }
    for (int i = getSize(); i < getAllocated(); i++)
    {
        std::cout << "_ ";
    }
    std::cout << std::endl;
    std::cout << " text: " << value_ << std::endl; // maybe it doesnt end with zero!
    std::cout << "-----------" << std::endl;
}


void debugText(const char *text, int size)
{
    std::cout << "appending : ";
    for (int i = 0; i < size; i++)
    {
        std::cout << ( (int) text[i] ) << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    Blob *blob = new Blob;

    char foo[] = { 'f', 'o', 'o' };
    char bar[] = { 'b', 'a', 'r' };
    char egg[] = { 'e', 'g', 'g' };
    char spam[] = { 's', 'p', 'a', 'm' };

    blob->debugPrint();
    debugText(foo, sizeof(foo));
    blob->append(foo, sizeof(foo));
    blob->debugPrint();
    debugText(bar, sizeof(bar));
    blob->append(bar, sizeof(bar));
    blob->debugPrint();
    debugText(egg, sizeof(egg));
    blob->append(egg, sizeof(egg));
    blob->append('x');
    blob->debugPrint();
    debugText(spam, sizeof(spam));
    blob->append(spam, sizeof(spam));
    blob->debugPrint();

    std::cout << "text: " << blob->get() << std::endl;

    delete blob;
    
    return 0;
}

