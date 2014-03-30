#include "camera.h"

void Camera::motion(int x, int y){
    if(x == position[0] && y == position[1]){
        mouse_stop();
        return;
    }

    if(x > position[0])
        mouse_right();
    else if(x < position[0])
        mouse_left();

    if(y > position[1])
        mouse_down();
    else if (y < position[1])
        mouse_up();

    position[0] = x;
    position[1] = y;
}

//void motion(int x, int y){
//    static Camera camera;
//    camera.motion(x, y);
//}
