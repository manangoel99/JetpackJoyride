#include "main.h"

#ifndef COIN_H
#define COIN_H

class Coin {
    public:
        Coin() {}
        Coin(float x, float y, color_t color);
        int type;
        int score;
        glm::vec3 position;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        float rotation;
        float speed_x;
        bounding_box_t box;
      private:
        VAO *object;
};

#endif // !COIN_H