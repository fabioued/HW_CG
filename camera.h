#ifndef CG_CAMERA_H_
#define CG_CAMERA_H_

#include <functional>

class Camera {

public:

    void motion(int x, int y);

    std::function<void()> mouse_stop   = [](){ printf("Stop!\n"); };
    std::function<void()> mouse_left   = [](){ printf("Left!\n"); };
    std::function<void()> mouse_right  = [](){ printf("Right!\n"); };
    std::function<void()> mouse_up     = [](){ printf("Up!\n"); };
    std::function<void()> mouse_down   = [](){ printf("Down!\n"); };
    int position[2];
};

void motion(int x, int y);

#endif
