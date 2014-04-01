#include <cstdio>
#include "scene.h"
#include "view.h"
#include "light.h"
#include "GL/glut.h"
#include "camera.h"

extern const char *obj_database;

Scene  *scene;
View   *view;
Light  *light;
Camera *camera;

void display();
void reshape(GLsizei , GLsizei);
void lighting();

void camera_setting(Camera *camera);
void keyboard(unsigned char key, int x, int y) { camera->keyboard(key, x, y); }
void motion(int x, int y){ camera->motion(x, y); }

int main(int argc, char** argv){

#ifdef TEST1
    obj_database = "TestScene1/";
    scene  = new Scene("scene.scene", "TestScene1");
    light  = new Light("light.light", "TestScene1");
    view   = new View("view.view", "TestScene1");
#elif TEST2
    obj_database = "TestScene2/";
    scene  = new Scene("house.scene", "TestScene2");
    light  = new Light("house.light", "TestScene2");
    view   = new View("house.view", "TestScene2");
#endif

    camera = new Camera;
    camera_setting(camera);

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Computer Graphic");
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(motion);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}

void camera_setting(Camera *camera){
    // keyboard
    camera->keyboard_up    = [](){ view->eye[2]-=5;};
    camera->keyboard_down  = [](){ view->eye[2]+=5;};
    camera->keyboard_left  = [](){ view->eye[0]-=5;};
    camera->keyboard_right = [](){ view->eye[0]+=5;};

    // mouse
    camera->mouse_stop = [](){};
    camera->mouse_up    = [](){ view->vat[2]-=5;};
    camera->mouse_down  = [](){ view->vat[2]+=5;};
    camera->mouse_left  = [](){ view->vat[0]-=5;};
    camera->mouse_right = [](){ view->vat[0]+=5;};
}

void lighting(){
    ////////////////////////////////////////
    // Lighting
    ////////////////////////////////////////
    glEnable(GL_LIGHTING);

    auto gl_light = GL_LIGHT0;

    glPushMatrix();
        for(const auto &i : light->lights){
            glLightfv(gl_light, GL_AMBIENT, i.ambient);
            glLightfv(gl_light, GL_SPECULAR, i.specular);
            glLightfv(gl_light, GL_DIFFUSE, i.diffuse);
            glLightfv(gl_light, GL_POSITION, i.position);
            glEnable(gl_light);
            gl_light++;
        }
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light->ambient);
    glPopMatrix();
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    // z buffer enable
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);

    ////////////////////////////////////////
    // Camera
    ////////////////////////////////////////
    // Perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view->fovy, view->viewport[2]/view->viewport[3], view->dnear, view->dfar);

    // LookAt
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  view->eye[0], view->eye[1], view->eye[2],  //eye
                view->vat[0], view->vat[1], view->vat[2],  //vat
                view->vup[0], view->vup[1], view->vup[2]); //vup

    ////////////////////////////////////////
    // Lighting
    ////////////////////////////////////////

    lighting();

    ////////////////////////////////////////
    // Scene
    ////////////////////////////////////////

    glMatrixMode(GL_MODELVIEW);
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_3D);

    glEnable(GL_NORMALIZE);

    for(const auto &i : scene->models){

        glPushMatrix();
            glTranslatef(i.T[0], i.T[1], i.T[2]);
            glRotatef(i.Angle, i.R[0], i.R[1], i.R[2]);
            glScalef(i.S[0], i.S[1], i.S[2]);


            //copy from example ...
            int lastMaterial = -1;
            for(size_t j=0;j < i.object->fTotal;++j)
            {
                // set material property if this face used different material
                if(lastMaterial != i.object->faceList[j].m)
                {
                    lastMaterial = (int)i.object->faceList[j].m;
                    glMaterialfv(GL_FRONT, GL_AMBIENT  , i.object->mList[lastMaterial].Ka);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE  , i.object->mList[lastMaterial].Kd);
                    glMaterialfv(GL_FRONT, GL_SPECULAR , i.object->mList[lastMaterial].Ks);
                    glMaterialfv(GL_FRONT, GL_SHININESS, &i.object->mList[lastMaterial].Ns);

                    //glColor3fv(i.object->mList[lastMaterial].Kd);

                    //you can obtain the texture name by object->mList[lastMaterial].map_Kd
                    //load them once in the main function before mainloop
                    //bind them in display function here
                }

                glBegin(GL_TRIANGLES);
                for (size_t k=0;k<3;++k)
                {
                    //textex corrd. object->tList[object->faceList[j][k].t].ptr
                    glTexCoord3fv(i.object->tList[i.object->faceList[j][k].t].ptr);

                    glNormal3fv(i.object->nList[i.object->faceList[j][k].n].ptr);
                    glVertex3fv(i.object->vList[i.object->faceList[j][k].v].ptr);
                }
                glEnd();
            }


        glPopMatrix();
    }

    ////////////////////////////////////////

    glFlush();
    glFinish();
    glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h){
    // keep the ratio
    view->viewport[2] = w;
    view->viewport[3] = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
