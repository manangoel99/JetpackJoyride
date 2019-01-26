#include "main.h"

#ifndef PROJECTILES_H
#define PROJECTILES_H

class SpeedUp {
    public:
        SpeedUp() {}
        SpeedUp(float x, float y);
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void tick();
        float speed_x;
        float speed_y;
        float acc_y;
        float rotation;
        bounding_box_t box;
    private:
        VAO *object;
};

class CoinBoost {
    public:
        CoinBoost() {}
        CoinBoost(float x, float y);
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void tick();
        float speed_x;
        float speed_y;
        float acc_y;
        float rotation;
        bounding_box_t box;

    private:
        VAO *object;
};

class Inverter {
    public:
        Inverter() {}
        Inverter(float x, float y);
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void tick();
        float speed_x;
        float speed_y;
        float acc_y;
        float rotation;
        bounding_box_t box;

    private:
        VAO *object;
};

class Life {
    public:
        Life() {}
        Life(float x, float y);
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void tick();
        float speed_x;
        float speed_y;
        float acc_y;
        float rotation;
        bounding_box_t box;

    private:
        VAO *object;
};

#endif // !PROJECTILES_H
