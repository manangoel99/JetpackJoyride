#include "coins.h"
#include "main.h"

#define ll long long

Coin::Coin(float x, float y, color_t color){
    this->position = glm::vec3(x, y, 0);
    //this->rotation = 0;
    //speed = 1;

    this->box.height = 0.1;
    this->box.width = 0.1;
    this->box.x = this->position.x;
    this->box.y = this->position.y;
    this->speed_x = 0;

    int n = 1000;
    float r = 0.1;

    GLfloat coord[9 * n];

    float theta = (2 * M_PI) / n;

    for (ll i = 0; i < 9 * n; i += 9)
    {
        coord[i] = r * cos(theta);
        coord[i + 1] = r * sin(theta);
        coord[i + 2] = 0;
        coord[i + 3] = 0;
        coord[i + 4] = 0;
        coord[i + 5] = 0;
        coord[i + 6] = r * cos(theta + (2 * M_PI) / n);
        coord[i + 7] = r * sin(theta + (2 * M_PI) / n);
        coord[i + 8] = 0;

        theta += ((2 * M_PI) / n);
    }

    this->object = create3DObject(GL_TRIANGLES, n*3, coord, color, GL_FILL);
}

void Coin::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
} 

void Coin::tick() {
    this->position.x -= (speed_x);
    this->rotation += 3;
}