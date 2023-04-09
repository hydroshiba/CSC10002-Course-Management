#include "DropBox.h"

DropBox::DropBox() {
    curIndex = -1;
    selected = false;

    current.label = "Choose an option";
    current.label.color = border_color;

    pos = {0, 0};
    size = {box_const::width, box_const::height};

    refresh();
}

void DropBox::refresh() {
    current.setPos(pos);
    current.setSize(size);

    for(int i = 0; i < options.size(); ++i) {
        options[i].setSize(size);

        Vector2 curPos;
        if(i) curPos = options[i - 1].getPos();
        else curPos = pos;

        options[i].setPos({curPos.x, curPos.y + size.y - box_const::thickness});
    }
}

void DropBox::setLabel(std::string label) {
    current.label = label;
}

void DropBox::setX(float x) {
    pos.x = x;
    refresh();
}

void DropBox::setY(float y) {
    pos.y = y;
    refresh();
}

void DropBox::setWidth(float width) {
    size.x = width;
    refresh();
}

void DropBox::setHeight(float height) {
    size.y = height;
    refresh();
}

void DropBox::setPos(Vector2 pos) {
    this->pos = pos;
    refresh();
}

void DropBox::setSize(Vector2 size) {
    this->size = size;
    refresh();
}

void DropBox::add(std::string label) {
    Option* last = nullptr;
    Option option;

    if(options.size() == 0) last = &current;
    else {
        last = (options.end() - 1);
        last->roundness = 0;
    }
    
    option.setPos({pos.x, last->getPos().y + size.y - box_const::thickness});
    option.setSize(size);
    option.label = label;
    option.roundness = roundness;

    option.fill_color = fill_color;
    option.border_color = border_color;
    option.hover_color = hover_color;
    option.press_color = press_color;

    options.append(option);
}

int DropBox::getSelected() {
    return curIndex;
}

void DropBox::render(const Vector2 &mouse) {
    current.render(mouse);

    if(selected) {
        for(int i = options.size() - 1; i > -1; --i)
            options[i].render(mouse);
    }
}

void DropBox::process(const Vector2 &mouse) {
    if(selected) {
        for(int i = 0; i < options.size(); ++i) if(options[i].clicked(mouse)){
            options[curIndex].fill_color = fill_color;
            curIndex = i;

            selected = false;
            current.label = options[i].label;
            current.label.color = text_color;

            options[i].fill_color = hover_color;
        }
    }

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !current.clicked(mouse)) selected = false;
	if(current.clicked(mouse)) selected = !selected;
}