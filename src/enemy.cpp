#include "main.h"
#include "enemy.h"
#define ll long long

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
    this->length = length;

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
    this->box.y = this->position.y;



}

FireLine::FireLine(float x, float y, float angle, float length) {
    this->position.x = x;
    this->position.y = y;
    this->angle = angle;
    this->length = length;
    this->speed_x = 0.075;

    this->final_position.x = x + length * cos(angle);
    this->final_position.y = y + length * sin(angle);

    int n = 1000;
    float r = 0.1;

    GLfloat coord[18 * n + 18];

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

    for (ll i = 9 * n; i < 18 * n; i += 9) {
        coord[i] = length * cos(angle) + r * cos(theta);
        coord[i + 1] = length * sin(angle) + r * sin(theta);
        coord[i + 2] = 0;
        coord[i + 3] = length * cos(angle);
        coord[i + 4] = length * sin(angle);
        coord[i + 5] = 0;
        coord[i + 6] = length * cos(angle) + r * cos(theta + (2 * M_PI) / n);
        coord[i + 7] = length * sin(angle) + r * sin(theta + (2 * M_PI) / n);
        coord[i + 8] = 0;

        theta += ((2 * M_PI) / n);
    }

    coord[18 * n] = 0.0f;
    coord[18 * n + 1] = 0.05f;
    coord[18 * n + 2] = 0.0f;
    coord[18 * n + 3] = 0.0f;
    coord[18 * n + 4] = -0.05f;
    coord[18 * n + 5] = 0.0f;
    coord[18 * n + 6] = length * cos(angle);
    coord[18 * n + 7] = 0.05f + length * sin(angle);
    coord[18 * n + 8] = 0.0f;

    coord[18 * n + 9] = 0.0f;
    coord[18 * n + 10] = -0.05f;
    coord[18 * n + 11] = 0.0f;
    coord[18 * n + 12] = length * cos(angle);
    coord[18 * n + 13] = 0.05f + length * sin(angle);
    coord[18 * n + 14] = 0.0f;
    coord[18 * n + 15] = length * cos(angle);
    coord[18 * n + 16] = -0.05f + length * sin(angle);
    coord[18 * n + 17] = 0.0f;

    this->object = create3DObject(GL_TRIANGLES, n * 6 + 6, coord, COLOR_FIRE, GL_FILL);
}

void FireLine::draw(glm::mat4 VP) {
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

void FireLine::tick() {
    this->position.x -= speed_x;
    this->final_position.x -= speed_x;
}

Boomerang::Boomerang(float x, float y) {
    this->position = glm::vec3(x, y, 0);

    this->speed_x = -0.125;
    this->speed_y = -0.02;
    this->acc_x = 0.00125;
    this->rotation = 0;

    static const GLfloat vertex_buffer_data[] = {
        -0.125f, 0.0625f, 0.0f,
        0.125f, 0.0625f, 0.0f,
        -0.125f, 0.0f, 0.0f,

        0.125f, 0.0f, 0.0f,
        0.125f, 0.0625f, 0.0f,
        -0.125f, 0.0f, 0.0f,

        -0.125f, 0.0625f, 0.0f,
        -0.025f, 0.0625f, 0.0f,
        -0.025f, 0.1875f, 0.0f,

        -0.125f, 0.0625f, 0.0f,
        -0.125f, 0.1875f, 0.0f,
        -0.025f, 0.1875f, 0.0f,

        0.125f, 0.0625f, 0.0f,
        0.025f, 0.0625f, 0.0f,
        0.025f, 0.1875f, 0.0f,

        0.125f, 0.0625f, 0.0f,
        0.125f, 0.1875f, 0.0f,
        0.025f, 0.1875f, 0.0f,
    };

    color_t COLOR_PINK = {255,20,147};

    this->object = create3DObject(GL_TRIANGLES, 6 * 3, vertex_buffer_data, COLOR_PINK, GL_FILL);

}

void Boomerang::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Boomerang::tick() {
    this->position.x += this->speed_x;
    this->position.y += this->speed_y;
    this->speed_x += this->acc_x;

    this->rotation += 5;
}

Magnet::Magnet(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 90;
    this->num_ticks = 0;
     
    static const GLfloat vertex_buffer_data[] = {
       -0.125f, 0.0625f, 0.0f,
       0.125f, 0.0625f, 0.0f,
       -0.125f, 0.0f, 0.0f,
       0.125f, 0.0f, 0.0f,
       0.125f, 0.0625f, 0.0f,
       -0.125f, 0.0f, 0.0f,
       -0.125f, 0.0625f, 0.0f,
       -0.025f, 0.0625f, 0.0f,
       -0.025f, 0.1875f, 0.0f,
       -0.125f, 0.0625f, 0.0f,
       -0.125f, 0.1875f, 0.0f,
       -0.025f, 0.1875f, 0.0f,
       0.125f, 0.0625f, 0.0f,
       0.025f, 0.0625f, 0.0f,
       0.025f, 0.1875f, 0.0f,
       0.125f, 0.0625f, 0.0f,
       0.125f, 0.1875f, 0.0f,
       0.025f, 0.1875f, 0.0f,
    };

    color_t COLOR_GREY = {169, 169, 169}; 

    this->UObject = create3DObject(GL_TRIANGLES, 6 * 3, vertex_buffer_data, COLOR_GREY, GL_FILL);

    static const GLfloat vertex_buffer_data_1[] = {
        -0.125f, 0.25f, 0.0f,
        -0.125f, 0.1875f, 0.0f,
        -0.025f, 0.25f, 0.0f,

        -0.125f, 0.1875f, 0.0f,
        -0.025f, 0.25f, 0.0f,
        -0.025f, 0.1875f, 0.0f,
    };

    this->TipLObject = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data_1, COLOR_RED, GL_FILL);

    static const GLfloat vertex_buffer_data_2[] = {
        0.125f, 0.25f, 0.0f,
        0.125f, 0.1875f, 0.0f,
        0.025f, 0.25f, 0.0f,

        0.125f, 0.1875f, 0.0f,
        0.025f, 0.25f, 0.0f,
        0.025f, 0.1875f, 0.0f,
    };

    this->TipRObject = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data_2, COLOR_RED, GL_FILL);

}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->UObject);
    draw3DObject(this->TipLObject);
    draw3DObject(this->TipRObject);
}

void Magnet::tick() {
    num_ticks++;
}