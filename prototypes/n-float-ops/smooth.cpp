/**
 * Prototype of applying filters on n-vectors of floats.
 */
#include <iostream>
#include <vector>

typedef std::vector<float> FloatVec;
std::ostream &operator<<(std::ostream &os, const FloatVec &vec);

/**
 * Manages a list for list of floats on which you can do operations, such as smoothing the flow of change.
 */
class FloatCrunsher
{
    public:
        FloatCrunsher(unsigned int message_size, unsigned int vector_size);
        bool calculate(const FloatVec &incoming, FloatVec &result);
        bool setValue(const FloatVec &incoming);
        void resize(unsigned int message_size, unsigned int elements_size);
        unsigned int getMessageSize() const;
        unsigned int getVectorSize() const;
    private:
        unsigned int message_size_;
        unsigned int vector_size_;
        unsigned int current_index_;
        /**
         * When called, the result message is a vector of the message_size_ with 0.0f as values. Also, the current_index_ has been incremented already.
         */
        virtual void doCalculate(const FloatVec &incoming, FloatVec &result) = 0;
    protected:
        std::vector<FloatVec> vec_;
};

FloatCrunsher::FloatCrunsher(
        unsigned int message_size,
        unsigned int vector_size) :
    message_size_(message_size),
    vector_size_(vector_size),
    current_index_(0)
{
    resize(message_size, vector_size);
}

unsigned int FloatCrunsher::getMessageSize() const
{
    return message_size_;
}

unsigned int FloatCrunsher::getVectorSize() const
{
    return vector_size_;
}

void FloatCrunsher::resize(
        unsigned int message_size,
        unsigned int vector_size)
{
    message_size_ = message_size;
    vector_size_ = vector_size;

    vec_.clear();
    
    for (unsigned int i = 0; i < vector_size_; ++i)
    {
        FloatVec item;
        for (unsigned int j = 0; j < message_size_; ++j)
            item.push_back(0.0f);
        vec_.push_back(item);
    }

    current_index_ = current_index_ % vector_size_;
}

bool FloatCrunsher::calculate(
        const FloatVec &incoming,
        FloatVec &result)
{
    if (incoming.size() != message_size_)
    {
        std::cerr << "FloatCrunsher::" << __FUNCTION__ <<
            ": Wrong size for incoming vector<float> " << incoming;
        std::cerr << " Expecting " << message_size_;
        std::cerr << std::endl;
        return false;
    }
    if (result.size() != message_size_)
    {
        result.clear();
        for (unsigned int i = 0; i < message_size_; ++i)
            result.push_back(0.0f);
    }
    if (vector_size_ == 0  || vector_size_ == 1)
    {
        result = incoming;
        return true;
    }
    vec_[current_index_] = incoming;
    current_index_ = (current_index_ + 1) % vector_size_;
    this->doCalculate(incoming, result);
    return true;
}

bool FloatCrunsher::setValue(const FloatVec &incoming)
{
    if (incoming.size() != message_size_)
    {
        std::cerr << "FloatCrunsher::" << __FUNCTION__ <<
            ": Wrong size for incoming vector<float> " << incoming;
        std::cerr << " Expecting " << message_size_;
        std::cerr << std::endl;
        return false;
    }
}

class Smoother : public FloatCrunsher
{
    public:
        Smoother(unsigned int message_size, unsigned int vector_size);
    private:
        virtual void doCalculate(const FloatVec &incoming, FloatVec &result);
};

Smoother::Smoother(unsigned int message_size, unsigned int vector_size) :
    FloatCrunsher(message_size, vector_size)
{
    // pass
}

void Smoother::doCalculate(const FloatVec &incoming, FloatVec &result)
{
    result.clear();
    // make sure it is zeroed
    for (unsigned int j = 0; j < getMessageSize(); ++j)
        result[j] = 0.0f;

    for (unsigned int i = 0; i < getMessageSize(); ++i)
    {
        for (unsigned int j = 0; j < getVectorSize(); ++j)
            result[j] = result[j] + vec_[i][j];
    }
}

std::ostream &operator<<(std::ostream &os, const FloatVec &vec)
{
    os << "FloatVec size=" << vec.size() << " contents=(";
    for (unsigned int i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    os << ")";
}

std::ostream &operator<<(std::ostream &os, const FloatCrunsher &self)
{
    os << "FloatCrunsher: message=" << self.getMessageSize() << " vector=" << self.getVectorSize() << " ";
    os << ")";
}

int main(int argc, char *argv[])
{
    Smoother smoother(3, 10);
    std::cout << smoother << std::endl;
    for (float f = 0.0f; f < 10.0f; f += 1.0f)
    {
        FloatVec vec;
        vec.push_back(f);
        vec.push_back(f);
        vec.push_back(f);

        FloatVec result;
        std::cout << "calculate with " << vec << " : ";
        smoother.calculate(vec, result);
        std::cout << result << std::endl;
    }
    return 0;
}

