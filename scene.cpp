#include "scene.h"

void scene::load(const char *file, const char *base_dir){
    char path[100];
    sprintf(path, "%s/%s", base_dir, file);

    FILE *f = fopen(path , "r");

	if (!f){
		printf("Can't open scene file '%s' !", path);
		return;
	}

    char s_obj[100];

	while(fgets(s_obj, sizeof(s_obj), f) > 0){
        models.emplace_back(s_obj);
    }

    if(f)
        fclose(f);
}

void scene::load(const char *file){
    load(file, "scene");
}

void scene::load(){
    load("test.scene", "scene");
}


scene::scene(const char *file, const char *base_dir){
    load(file, base_dir);
}

scene::scene(const char *file){
    load(file, "scene");
}

scene::scene(){
    load("test.scene", "scene");
}
