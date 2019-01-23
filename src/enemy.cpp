#include "main.h"
#include "enemy.h"

FireBeam::FireBeam(float x, float y, float length) {
    this->position = glm::vec3(x, y, 0);
    this->speed_x = 0.075;
    this->speed_y = 0.025;
    this->box.height = 0.5;
    this->box.width = length + 1;
    this->rotation = 0;
    this->direction = "up";

    static const GLfloat vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.3f, 0.0f,
        0.3f, 0.0f, 0.0f,
        0.0f, 0.3f, 0.0f,
        0.3f, 0.0f, 0.0f,
        0.3f, 0.3f, 0.0f,
        0.3f, 0.1f, 0.0f,
        0.3f + length, 0.1f, 0.0f,
        0.3f, 0.2f, 0.0f,
        0.3f, 0.2f, 0.0f,
        0.3f + length, 0.2f, 0.0f,
        0.3f + length, 0.1f, 0.0f,
        0.3f + length, 0.0f, 0.0f,
        0.3f + length, 0.3f, 0.0f,
        0.6f + length, 0.0f, 0.0f,
        0.3f + length, 0.3f, 0.0f,
        0.6f + length, 0.0f, 0.0f,
        0.6f + length, 0.3f, 0.0f
    };

    this->box.x = this->position.x;
    this->box.y = this->position.y;

    this->object = create3DObject(GL_TRIANGLES, 6 * 3, vertex_buffer_data, COLOR_ORANGE, GL_FILL);
}

void FireBeam::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void FireBeam::tick() {
    this->position.x -= speed_x;
    this->box.x = this->position.x;

    if (this->direction == "up") {
        this->position.y += this->speed_y;
        if (this->position.y >= 3.5) {
            this->direction = "down";
        }
    }

    else if (this->direction == "down")
    {
        this->position.y -= this->speed_y;
        if (this->position.y <= -0.5) {
            this->direction = "up";
        }
        
    }



}