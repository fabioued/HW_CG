#include <cstdio>
#include <cstring>
#include "GL/glut.h"
#include "FreeImage.h"
#include "scene.h"
#include "view.h"
#include "light.h"
#include "camera.h"

#define TEX_NUM 4
#define MIPMAP

extern const char *obj_database;

Scene  *scene;
View   *view;
Light  *light;
Camera *camera;

GLuint texObject[TEX_NUM];

void display();
void reshape(GLsizei , GLsizei);
void _loadTexture(const char *filename, const int texnum);
void loadTexture(const char *filename[]);
void lighting();

void camera_setting(Camera *camera);
void keyboard(unsigned char key, int x, int y) { camera->keyboard(key, x, y); }
void motion(int x, int y){ camera->motion(x, y); }

void environmentMap(const char *filename[6]);

int main(int argc, char** argv){

    char scene_name[100];
    char scene_dir[100];

    //printf("Please input the scene directory : ");
    scanf("%s", scene_dir);

    //printf("Please input the scene name : ");
    scanf("%s", scene_name);

    printf("[%s]\n", scene_name);

    obj_database = scene_dir;
    scene  = new Scene(scene_name, scene_dir);
    light  = new Light(scene_name, scene_dir);
    view   = new View(scene_name, scene_dir);

    camera = new Camera;
    camera_setting(camera);

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Computer Graphic");

    const char* texfile[] = {
        "ChessScene/Room.bmp",
        "ChessScene/Grid.bmp",
        "ChessScene/Wood.bmp",
    };
    loadTexture(texfile);

    const char* env[] = {
        "ChessScene/Env_positive_x.bmp",
        "ChessScene/Env_positive_y.bmp",
        "ChessScene/Env_positive_z.bmp",
        "ChessScene/Env_negative_x.bmp",
        "ChessScene/Env_negative_y.bmp",
        "ChessScene/Env_negative_z.bmp"
    };
    environmentMap(env);

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
    camera->mouse_up    = [](){ view->vat[2]-=1;};
    camera->mouse_down  = [](){ view->vat[2]+=1;};
    camera->mouse_left  = [](){ view->vat[0]-=1;};
    camera->mouse_right = [](){ view->vat[0]+=1;};
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

void _loadTexture(const char *filename, const int texnum){
    glBindTexture(GL_TEXTURE_2D, texObject[texnum]);
    FIBITMAP *bitmap = FreeImage_Load(FreeImage_GetFileType(filename, 0), filename);
    FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
    int nWidth = FreeImage_GetWidth(pImage);
    int nHeight = FreeImage_GetHeight(pImage);

#ifdef MIPMAP
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, nWidth, nHeight, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
#endif

#ifdef MIPMAP
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    FreeImage_Unload(pImage);
}

void loadTexture(const char *filename[]){
    glGenTextures(TEX_NUM, texObject);
    for(unsigned int i = 0; i < TEX_NUM - 1; i++){
        _loadTexture(filename[i], i);
    }
}

void environmentMap(const char *filename[6]){
    // Cube : Bind Once, send six images
    glBindTexture(GL_TEXTURE_CUBE_MAP, texObject[TEX_NUM - 1]);

    // Load Cube Map images
    FIBITMAP *bitmap;
    FIBITMAP *pImage[6];

    for(unsigned int i = 0; i < 6; i++){
        bitmap = FreeImage_Load(FreeImage_GetFileType(filename[i], 0), filename[i]);
        pImage[i] = FreeImage_ConvertTo32Bits(bitmap);
    }

    int nWidth = FreeImage_GetWidth(pImage[0]);
    int nHeight = FreeImage_GetHeight(pImage[0]);

#ifdef MIPMAP
    gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT, GL_RGBA, nWidth, nHeight, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[0]));
    gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT, GL_RGBA, nWidth, nHeight, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[1]));
    gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT, GL_RGBA, nWidth, nHeight, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[2]));
    gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT, GL_RGBA, nWidth, nHeight, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[3]));
    gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT, GL_RGBA, nWidth, nHeight, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[4]));
    gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT, GL_RGBA, nWidth, nHeight, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[5]));

#else
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[0]));

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[1]));

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[2]));

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[3]));

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[4]));

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage[5]));
#endif

#ifdef MIPMAP
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#else
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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

    ////////////////////////////////////////
    // Texture
    ////////////////////////////////////////

    glEnable(GL_NORMALIZE);

    int texnum = -1;
    const char *prev = "tmp";
    for(const auto &i : scene->models){
        if(strcmp(prev, i.obj)){
            texnum++;
        }

        glDisable(GL_TEXTURE_CUBE_MAP);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
        glDisable(GL_TEXTURE0);
        glDisable(GL_TEXTURE1);

        if (texnum == 0) {

            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texObject[texnum]);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        } else if (texnum == 1) {

            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texObject[texnum]);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

            glActiveTexture(GL_TEXTURE1);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texObject[++texnum]);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

        } else if (texnum < 9) {

            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        } else {

            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_GEN_R);
            glEnable(GL_TEXTURE_CUBE_MAP_EXT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TEX_NUM-1);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        }

        prev = i.obj;

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
