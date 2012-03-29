#include <mapper/mapper.h>
#include <tr1/memory>
#include <iostream>
#include <string>
#include <typeinfo>

class AbstractMapperPad
{
    public:
        AbstractMapperPad(const char *name, int length);
        const std::string &getName() const { return name_; }
        int getLength() const { return length_; }
        char getType() const { return type_; }
    protected:
        template <typename T>
        bool setType()
        {
            if (typeid(T) == typeid(float))
                type_ = 'f';
            else if (typeid(T) == typeid(int))
                type_ = 'i';
            else
            {
                std::cerr << "ERROR: " << __FUNCTION__ << ": Unsupported type " << typeid(T) << std::endl;
                return false;
            }
            return true;
        }
    private:
        std::string name_;
        int length_;
        char type_;
        // std::string unit_;
        // T minimum_;
        // T maximum_;
        // bool has_range_;
}

AbstractMapperPad::AbstractMapperPad(const char *name, int length)
{
    name_ = std::string(name);
    length_ = length;
    type_ = 'f'; // default
}

class AbstractMapperSink : public AbstractMapperPad
{
    public:
        typedef std::tr1::shared_ptr<AbstractMapperSink> ptr;
        AbstractMapperSink(const char *name, int length) :
            AbstractMapperPad(name, length)
        {
        }
};

template<typename T>
struct MapperSink : public AbstractMapperSink
{
    public:
        MapperSink(const char *name, int length) :
            AbstractMapperPad(name, length)
        {
            setType<T>();
        }
};

class AbstractMapperSource : public AbstractMapperPad
{
    public:
        typedef std::tr1::shared_ptr<AbstractMapperSource> ptr;
        AbstractMapperSource(const char *name, int length, mapper_signal_handler *handler) :
            AbstractMapperPad(name, length)
        {
            handler_ = handler;
        }
        mapper_signal_handler *getHandler() { return handler_; }
    private:
        mapper_signal_handler *handler_;
}

template<typename T>
struct MapperSource : public AbstractMapperSource
{
    public:
        MapperSource(const char *name, int length, mapper_signal_handler *handler) :
            AbstractMapperSource(name, length, handler)
        {
            setType<T>();
        }
};

class MapperDevice
{
    public:
        MapperDevice();
        ~MapperDevice();
        bool poll();
        bool addSink(MapperSink::ptr sink);
        bool addSource(MapperSource::ptr source);
    private:
        std::string name_prefix_;
        int initial_port_;
        int polling_duration_ms_;
        mapper_device device_;
        std::map<std::string, MapperSink::ptr> sinks_;
        std::map<std::string, MapperSource::ptr> sources_;
}; 

MapperDevice::MapperDevice() :
    name_prefix_("foo_"),
    initial_port_(9000),
    polling_duration_ms_(0) // non-blocking
{
    device_ = mdev_new(name_prefix_.c_str(), initial_port_, 0);
}

bool MapperDevice::poll()
{
    // TODO: check if mdev_ready?
    //int num_messages_handled = 
    mdev_poll(device_, polling_duration_ms_);
}

MapperDevice::~MapperDevice()
{
    mdev_free(device_);
}

bool MapperDevice::addSink(MapperSink::ptr sink)
{
    if (sinks_.find(sink.getName() == sinks_.end()))
    {
        sinks_[sink.getName()] = sink;
        // mapper_signal sig =
        mdev_add_output(
            device_,
            sink->getName().c_str(),
            sink->getLength(), sink->getType(),
            "", // unit
            0, 0); // range
        return true;
    }
    else
    {
        std::cerr << "Already got sink " << sink->getName() << std::endl;
        return false;
    }
}

bool MapperDevice::addSource(MapperSource::ptr source)
{
    if (sources_.find(sink.getName() == sources_.end()))
    {
        sources_[source.getName()] = source;
        // mapper_signal sig =
        mdev_add_output(
            device_,
            source->getName().c_str(),
            source->getLength(), source->getType(),
            "", // unit
            0, 0,
            source->getHandler()
            ); // range
        return true;
    }
    else
    {
        std::cerr << "Already got source " << source->getName() << std::endl;
        return false;
    }
}
        // sink->setMapperSignal(sig);

static void handle_float(mapper_signal sig, float *f, void *user_data)
{
    //set_freq(*pfreq);
}

static void handle_int(mapper_signal sig, int *i, void *user_data)
{
    //set_freq(*pfreq);
}


int main()
{
    signal(SIGINT, ctrlc);

    mapper_device dev = mdev_new("pwm", 9000, 0);

    float min0 = 0;
    float max1 = 1;
    float max1000 = 1000;

    mdev_add_input(dev, "/freq", 1, 'f', "Hz", &min0, &max1000,
                   (mapper_signal_handler*)handler_freq, 0);
    mdev_add_input(dev, "/gain", 1, 'f', "Hz", &min0, &max1,
                   (mapper_signal_handler*)handler_gain, 0);
    mdev_add_input(dev, "/duty", 1, 'f', "Hz", &min0, &max1,
                   (mapper_signal_handler*)handler_duty, 0);

    run_synth();

    set_duty(0.1);
    set_freq(110.0);
    set_gain(0.1);

    printf("Press Ctrl-C to quit.\n");

    while (!done)
        mdev_poll(dev, 10);

    mdev_free(dev);

    set_freq(0);
    sleep(1);

    stop_synth();
}
