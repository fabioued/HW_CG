// view.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CG_VIEW_H_
#define CG_VIEW_H_

class view {

private:

public:

    view(const char *file, const char *base_dir);
    view(const char *file);
    view();

    void load(const char *file, const char *base_dir);
    void load(const char *file);
    void load();

    int eye[3], vat[3], vup[3], fovy, dnear, dfar, viewport[4];
};

#endif
