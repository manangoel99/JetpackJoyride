#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    this->life = 3;
    this->speed_x = 0;
    this->speed_y = 0;

    static const GLfloat vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    this->box.height = 0.5;
    this->box.width = 0.5;
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
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

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->rotation += 0;
    // this->rotation += speed;
    this->position.x += speed_x;
    this->position.y += speed_y;
    this->box.x = this->position.x;
    this->box.y = this->position.y;
}

JetPack::JetPack(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    
    static const GLfloat vertex_buffer_data[] = {
        //0.25f, 0.0f, 0.0f,
        //0.125f, -0.25f, 0.0f,
        //0.375f, -0.25f, 0.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_FIRE, GL_FILL);

}

void JetPack::draw(glm::mat4 VP) {
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

void JetPack::propel() {
    static const GLfloat vertex_buffer_data[] = {
        0.25f, 0.0f, 0.0f,
        0.125f, -0.25f, 0.0f,
        0.375f, -0.25f, 0.0f,
    };
    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_FIRE, GL_FILL);
}

void JetPack::stop_propel() {
    static const GLfloat vertex_buffer_data[] = {
        //0.25f, 0.0f, 0.0f,
        //0.125f, -0.25f, 0.0f,
        //0.375f, -0.25f, 0.0f,
    };
    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_FIRE, GL_FILL);
}

void JetPack::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}



