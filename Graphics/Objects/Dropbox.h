#ifndef DROPBOX_H
#define DROPBOX_H

#include "raylib.h"
#include "Vector.h"
#include "Box.h"

class Dropbox {
private:
    float textSize;
    bool selected;
    int curIndex;
    Box curBox;

    Vector2 pos;
    Vector2 size;

    Vector<Box> options;

public:
    float roundness = box_const::roundness;
    Color fill_color = box_const::fill_color, border_color = box_const::border_color;
    Color hover_color = button_const::hover_color, press_color = button_const::press_color;
    Color text_color = text_const::color;

    Dropbox();
    void refresh();
    
    void setLabel(std::string label);
    void setX(float x);
    void setY(float y);

    void setWidth(float width);
    void setHeight(float height);
    
    void setPos(Vector2 pos);
    void setSize(Vector2 size);

    void add(std::string label);
    int getSelected();

    void render(const Vector2 &mouse);
    void process(const Vector2 &mouse);
};

#endif //DROPBOX_H