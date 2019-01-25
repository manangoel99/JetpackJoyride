#include "semicircle.h"
#include "main.h"
#include <bits/stdc++.h>

Ring::Ring(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    GLfloat vertex_buffer_data[10000];
    std::vector <GLfloat> vertcies;
    this->radius = 1.0f;
    this->rotation = 0;
    this->speed_x = 0.075;

    for (float theta = -M_PI / 2; theta < M_PI / 2; theta += M_PI/180) 
    {
        GLfloat x1, y1, z1, 
                x2, y2, z2, 
                x3, y3, z3, 
                x4, y4, z4,
                x5, y5, z5,
                x6, y6, z6;

        
        x1 = x + this->radius * sin(theta);
        y1 = y + this->radius * cos(theta);
        z1 = 0;

        x2 = x + this->radius * sin(theta + M_PI / 180);
        y2 = y + this->radius * cos(theta + M_PI/ 180);
        z2 = 0;

        x3 = x1;
        y3 = y1 + 0.05;
        z3 = 0;

        x4 = x1;
        y4 = y1 + 0.05;
        z4 = 0;

        x5 = x2;
        y5 = y2;
        z5 = 0;

        x6 = x2;
        y6 = y2 + 0.05;
        z6 = 0;


        vertcies.push_back(x1);
        vertcies.push_back(y1);
        vertcies.push_back(z1);

        vertcies.push_back(x2);
        vertcies.push_back(y2);
        vertcies.push_back(z2);

        vertcies.push_back(x3);
        vertcies.push_back(y3);
        vertcies.push_back(z3);

        vertcies.push_back(x4);
        vertcies.push_back(y4);
        vertcies.push_back(z4);

        vertcies.push_back(x5);
        vertcies.push_back(y5);
        vertcies.push_back(z5);

        vertcies.push_back(x6);
        vertcies.push_back(y6);
        vertcies.push_back(z6);
    }

    int i = 0;

    for (std::vector <GLfloat>::iterator it = vertcies.begin(); it != vertcies.end(); it++) {
        //std::cout << *it << std::endl;
        vertex_buffer_data[i] = (*it);
        //std::cout << vertex_buffer_data[i] << std::endl;

        i++;
    }

    //std::cout << vertcies.size() << std::endl;

    this->object = create3DObject(GL_TRIANGLES, 1080, vertex_buffer_data, COLOR_BLACK, GL_FILL);

}

void Ring::draw(glm::mat4 VP) {
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

void Ring::tick() {
    this->position.x -= this->speed_x;
}