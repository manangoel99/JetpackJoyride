#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
    public:
        Ball() {}
        Ball(float x, float y, color_t color);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        void propulsion();
        int life;
        double speed;
        float speed_x;
        float speed_y;
        bounding_box_t box;
    private:
        VAO *object;
};

class JetPack {
    public:
        JetPack() {}
        JetPack(float x, float y);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void propel();
        void stop_propel();
        void set_position(float x, float y);
        void tick(float x, float y);
    private:
        VAO *object;
};

#endif // BALL_H
