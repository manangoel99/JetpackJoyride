#include "main.h"
#include "balloon.h"

Balloon::Balloon(float x, float y) {
    this->position.x = x;
    this->position.y = y;
    this->rotation = 0;

    this->speed_x = 0.15;
    this->speed_y = 0.05;
    this->acc_y = -0.025;
    this->box.height = 0.25;
    this->box.width = 0.25;

    static const GLfloat vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.125f, 0.0f,
        0.125f, 0.0f, 0.0f,
        0.125f, 0.125f, 0.0f,
        0.125f, 0.0f, 0.0f,
        0.0f, 0.125f, 0.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data, COLOR_BLUE, GL_FILL);
}

void Balloon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Balloon::tick() {
    this->position.x += speed_x;
    this->position.y += speed_y;
    this->speed_y += this->acc_y;
    this->box.x = this->position.x;
    this->box.y = this->position.y;
}