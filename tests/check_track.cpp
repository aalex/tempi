#include <iostream>
#include "tempi/track.h"

using namespace tempi;

bool check_simple()
{
    Track<float> track = Track<float>(3.14159f);
    track.setName("example");

    track.addKeyFrame(0L, Track<float>::KeyFramePtr(new KeyFrame<float>(0.0f)));
    track.addKeyFrame(1L, Track<float>::KeyFramePtr(new KeyFrame<float>(1.0f)));
    track.addKeyFrame(2L, Track<float>::KeyFramePtr(new KeyFrame<float>(2.0f)));
    track.addKeyFrame(3L, Track<float>::KeyFramePtr(new KeyFrame<float>(3.0f)));

    std::vector<Track<float>::KeyFramePtr> keyFrames = track.getKeyFrames(1L, 2L);

    if (keyFrames.size() != 2)
    {
        std::cout << "Vector of keyFrames should have two elements but has " << keyFrames.size() << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (not check_simple())
        return 1;
    return 0;
}

