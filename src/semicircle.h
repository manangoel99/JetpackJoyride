#include "main.h"

#ifndef RING_H
#define RING_H

class Ring {
    public:
        Ring() {};
        Ring(float x, float y);
        glm::vec3 position;
        float radius;
        void draw(glm::mat4 VP);
        void tick();
        float speed_x;
        float rotation;
    private:
        VAO *object;
};

#endif // !RING_H