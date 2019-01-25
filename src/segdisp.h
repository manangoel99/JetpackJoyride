#include "main.h"

#ifndef SEGMENT_H
#define SEGMENT_H

class Segment {
    public:
        Segment() {}
        Segment (float x, float y, float rotation);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void TurnWhite();
        void TurnBlue();
    private:
        VAO *object;

}; 

#endif // !SEGMENT_H
