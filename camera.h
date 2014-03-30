#ifndef CG_CAMERA_H_
#define CG_CAMERA_H_

#include <functional>

class Camera {

public:

    void motion(int x, int y);

    std::function<void()> stop   = [](){ printf("Stop!\n"); };
    std::function<void()> left   = [](){ printf("Left!\n"); };
    std::function<void()> right  = [](){ printf("Right!\n"); };
    std::function<void()> up     = [](){ printf("Up!\n"); };
    std::function<void()> down   = [](){ printf("Down!\n"); };
    int position[2];
};

void motion(int x, int y);

#endif
