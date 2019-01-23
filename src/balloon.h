#include "main.h"

#ifndef BALLOON_H
#define BALLOON_H

class Balloon {
    public:
        Balloon() {}
        Balloon(float x, float y);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void tick();
        float speed_x;
        float speed_y;
        float acc_y;
        bounding_box_t box;
    private:
        VAO *object;
};

#endif