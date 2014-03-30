#ifndef CG_CAMERA_H_
#define CG_CAMERA_H_

#include <functional>

class Camera {

public:

    void motion(int x, int y);
    void keyboard(unsigned char key, int x, int y);

    std::function<void()> mouse_stop   = [](){ printf("Stop!\n"); };
    std::function<void()> mouse_left   = [](){ printf("Left!\n"); };
    std::function<void()> mouse_right  = [](){ printf("Right!\n"); };
    std::function<void()> mouse_up     = [](){ printf("Up!\n"); };
    std::function<void()> mouse_down   = [](){ printf("Down!\n"); };

    std::function<void()> keyboard_up       = [](){ printf("Key Up!\n"); };
    std::function<void()> keyboard_down     = [](){ printf("Key Down!\n"); };
    std::function<void()> keyboard_left     = [](){ printf("Key Left!\n"); };
    std::function<void()> keyboard_right    = [](){ printf("Key Right!\n"); };

    int position[2];
};

void motion(int x, int y);

#endif
