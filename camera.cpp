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

void Camera::keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'w':
            keyboard_up();
            break;
        case 'a':
            keyboard_left();
            break;
        case 's':
            keyboard_down();
            break;
        case 'd':
            keyboard_right();
            break;
        case 27:    //ESC
            exit(0);
            break;
    }
}

//void motion(int x, int y){
//    static Camera camera;
//    camera.motion(x, y);
//}
