#include <cstdio>
#include "view.h"

void View::load(const char *file, const char *base_dir){
    char path[100];
    sprintf(path, "%s/%s", base_dir, file);

    FILE *f = fopen(path , "r");

	if (!f){
		printf("Can't open view file '%s' !", path);
		return;
	}

    fscanf(f, "eye %lf %lf %lf\n", &eye[0], &eye[1], &eye[2]);
    fscanf(f, "vat %lf %lf %lf\n", &vat[0], &vat[1], &vat[2]);
    fscanf(f, "vup %lf %lf %lf\n", &vup[0], &vup[1], &vup[2]);
    fscanf(f, "fovy %lf\n", &fovy);
    fscanf(f, "dnear %lf\n", &dnear);
    fscanf(f, "dfar %lf\n", &dfar);
    fscanf(f, "viewport %lf %lf %lf %lf\n", &viewport[0], &viewport[1], &viewport[2], &viewport[3]);

    if(f)
        fclose(f);
}

void View::load(const char *file){
    load(file, "view");
}

void View::load(){
    load("test.view", "view");
}


View::View(const char *file, const char *base_dir){
    load(file, base_dir);
}

View::View(const char *file){
    load(file, "view");
}

View::View(){
    load("test.view", "view");
}
