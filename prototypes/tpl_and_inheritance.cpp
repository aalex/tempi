#include <iostream>
#include <vector>
#include <tr1/memory>
#include <typeinfo>

class AbstractTrack
{
    public:
        AbstractTrack(int id) :
            id_(id)
        {}
        int getId() { return id_; }
        //template <typename U>
        //virtual bool isA<U>() = 0;
        //virtual void getValue() const {}
        //virtual type_name getType() const = 0;
    private:
        int id_;
};

template <typename T>
class Track : public AbstractTrack
{
    public:
        Track(int id, T value) :
            AbstractTrack(id)
        {
            setValue(value);
        }
        T &getValue() const { return value_; }
        //template <typename U>
        //virtual bool isA<U>()
        //{
        //    return typeid(T) == typeid(U);
        //}
        void setValue(T value) { value_ = value; }
        std::type_info getType() const { return typeid(value_); }
    private:
        T value_;
};

int main(int argc, char *argv[])
{
    typedef std::tr1::shared_ptr<AbstractTrack> TrackPtr;

    std::vector<TrackPtr> tracks;
    tracks.push_back(TrackPtr(new Track<int>(1, 1)));
    tracks.push_back(TrackPtr(new Track<float>(2, 2.0)));

    typename std::vector<TrackPtr>::iterator iter;
    for (iter = tracks.begin(); iter != tracks.end(); ++iter)
    {
        AbstractTrack *track = iter->get();
        std::cout << track->getId() << ": ";
        //std::cout << track->getValue();
        std::cout << std::endl;
    }
    return 0;
}
