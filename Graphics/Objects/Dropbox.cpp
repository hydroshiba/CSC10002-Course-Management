#include "Dropbox.h"
#include "math.h"
#include <iostream>

Dropbox::Dropbox() {}

void Dropbox::setLabel(std::string label) {
    select.label = label;
}

void Dropbox::setX(float x) {
    position.x = x;
    select.setX(x);
}

void Dropbox::setY(float y) {
    position.y = y;
    select.setY(y);
}

void Dropbox::setWidth(float width) {
    size.x = width;
    select.setWidth(width);
}

void Dropbox::setHeight(float height) {
    size.y = height;
    select.setHeight(height);
    total_height = height;
}

void Dropbox::setPos(Vector2 pos) {
    position = pos;
    select.setPos(pos);
}

void Dropbox::setSize(Vector2 sz) {
    size = sz;
    select.setSize(sz);
    total_height = sz.y;
}

void Dropbox::addNewButton(std::string label) {
    Button temp;
    temp.label = label;
    temp.setSize(size);
    temp.setPos({position.x, position.y + total_height});
    options.append(temp);
    total_height += size.y;
}

void Dropbox::renderAllOptions(const Vector2 &mouse) {
    int number_of_buttons = options.size();
    for (int i = 0; i < number_of_buttons; ++i) {
        options[i].render(mouse);
    }
}

void Dropbox::render(const Vector2 &mouse) {
    select.render(mouse);
    if (select.clicked(mouse)) {
        if (clicked == false) clicked = true;
        else clicked = false;
    }
    if (clicked) renderAllOptions(mouse);
}
