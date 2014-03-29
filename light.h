// light.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CG_LIGHT_H_
#define CG_LIGHT_H_

#include <vector>

class light {

private:

    class light_unit {
    public:
        light_unit(const char *s);
        float position[3], ambient[3], diffuse[3], specular[3];
    };

public:

    light(const char *file, const char *base_dir);
    light(const char *file);
    light();

    void load(const char *file, const char *base_dir);
    void load(const char *file);
    void load();

    std::vector<light_unit> lights;
    float ambient[3];
};

#endif
