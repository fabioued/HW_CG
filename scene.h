// scene.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CG_SCENE_H_
#define CG_SCENE_H_

#include <cstdio>
#include <vector>

class scene {

private:

    class model {
    public:
        model(){
            S[0] = S[1] = S[2] = Angle = R[0] = R[1] = R[2] = T[0] = T[1] = T[2] = 0;
            sscanf(obj, "none");
        }
        model(const char *s){
            sscanf(s, "model %s %f %f %f %f %f %f %f %f %f %f", obj, &S[0], &S[1], &S[2], &Angle, &R[0], &R[1], &R[2], &T[0], &T[1], &T[2]);
        }

        float S[3], Angle, R[3], T[3];
        char obj[100];
    };

public:

    scene(const char *file, const char *base_dir);
    scene(const char *file);
    scene();

    void load(const char *file, const char *base_dir);
    void load(const char *file);
    void load();

    std::vector<model> models;
};

#endif
