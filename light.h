// light.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CG_LIGHT_H_
#define CG_LIGHT_H_

#include <vector>

class Light {

private:

    class LightUnit {
    public:
        LightUnit(const char *s);
        float position[3], ambient[3], diffuse[3], specular[3];
    };

public:

    Light(const char *file, const char *base_dir);
    Light(const char *file);
    Light();

    void load(const char *file, const char *base_dir);
    void load(const char *file);
    void load();

    std::vector<LightUnit> lights;
    float ambient[3];
};

#endif
