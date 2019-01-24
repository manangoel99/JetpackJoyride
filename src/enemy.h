#include "main.h"
#include <bits/stdc++.h>

#ifndef ENEMY_H
#define  ENEMY_H

class FireBeam {
    public:
        FireBeam() {};
        FireBeam(float x, float y, float length);
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void tick();
        bounding_box_t box;
        float speed_x;
        float speed_y;
        float rotation;
        float length;
        std::string direction;
    private:
        VAO *object;
};

class FireLine {
    public:
        FireLine() {};
        FireLine(float x, float y, float angle, float length);
        glm::vec3 position;
        glm::vec3 final_position;
        float speed_x;
        void draw(glm::mat4 VP);
        void tick();
        float rotation;
        float angle;
        float length;
    private:
        VAO *object;
};

class Boomerang {
    public:
        Boomerang(float x, float y);
        glm::vec3 position;
        float speed_x;
        float speed_y;
        float acc_x;
        void draw(glm::mat4 VP);
        float rotation;
        void tick();

    private:
        VAO *object;
};


#endif // !ENEMY_H