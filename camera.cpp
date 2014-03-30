#include "camera.h"

void Camera::motion(int x, int y){
    if(x == position[0] && y == position[1]){
        stop();
        return;
    }

    if(x > position[0])
        right();
    else if(x < position[0])
        left();

    if(y > position[1])
        down();
    else if (y < position[1])
        up();

    position[0] = x;
    position[1] = y;
}

//void motion(int x, int y){
//    static Camera camera;
//    camera.motion(x, y);
//}
