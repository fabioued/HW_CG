// view.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CG_VIEW_H_
#define CG_VIEW_H_

class View {

private:

public:

    View(const char *file, const char *base_dir);
    View(const char *file);
    View();

    void load(const char *file, const char *base_dir);
    void load(const char *file);
    void load();

    int eye[3], vat[3], vup[3], fovy, dnear, dfar, viewport[4];
};

#endif
