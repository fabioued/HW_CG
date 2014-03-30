#include <cstdio>
#include <cstring>
#include "light.h"

Light::LightUnit::LightUnit(const char *s){
    sscanf(s, "light %f %f %f %f %f %f %f %f %f %f %f %f", &position[0], &position[1], &position[2], &ambient[0], &ambient[1] , &ambient[2], &diffuse[0], &diffuse[1], &diffuse[2], &specular[0], &specular[1], &specular[2]);
}

void Light::load(const char *file, const char *base_dir){
    char path[100];
    sprintf(path, "%s/%s", base_dir, file);

    FILE *f = fopen(path , "r");

	if (!f){
		printf("Can't open light file '%s' !", path);
		return;
	}

    char s_light[100];

	while(fgets(s_light, sizeof(s_light), f) > 0){
        if(strncmp(s_light, "light", 5) == 0)
            lights.emplace_back(s_light);
        else
            sscanf(s_light, "ambient %f %f %f", &ambient[0], &ambient[1], &ambient[2]);
    }


    if(f)
        fclose(f);
}

void Light::load(const char *file){
    load(file, "light");
}

void Light::load(){
    load("test.light", "light");
}


Light::Light(const char *file, const char *base_dir){
    load(file, base_dir);
}

Light::Light(const char *file){
    load(file, "light");
}

Light::Light(){
    load("test.light", "light");
}
