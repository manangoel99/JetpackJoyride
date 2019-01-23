#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include <iostream>
#include "coins.h"
#include "enemy.h"
#include <bits/stdc++.h>
#define ll long long

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Ground ground;
vector <FireBeam> fire_list;
vector <FireLine> fire_lint_list;
float init_pos = 0;

ll num_ticks = 0;

vector <Coin> coin_arr;
bool jump_status = false;
float speed = 0.05;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 90;

Timer t60(1.0 / 60);

//bool jump_status = false;

float randomFloat(float a, float b) {
    float random = (float) rand() / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void move_horizontal(Ball *ball, char direction) {
    if (direction == 'l') {
        (*ball).position.x -= 0.05;   
        
    }

    else if (direction == 'r') {
        (*ball).position.x += 0.05;
    }

}


/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model
    // Scene render
    ball1.draw(VP);
    ground.draw(VP);

    for (int i = 0; i < coin_arr.size(); i++) {
        coin_arr[i].draw(VP);
    }

    for (vector<FireBeam>::iterator it = fire_list.begin(); it != fire_list.end(); it++) {
        (*it).draw(VP);
    }

    for(vector <FireLine>::iterator it = fire_lint_list.begin(); it != fire_lint_list.end(); it++) {
        (*it).draw(VP);
    }


}

void jump(Ball *ball) {
    if ((*ball).position.y <= 3.5)
        (*ball).position.y += 0.075;
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);

    if (!up && jump_status == true) {
        if (ball1.position.y - speed <= -1.025) {
            ball1.position.y = -1;
        }
        else {
            ball1.position.y -= speed;
        }
        if (ball1.position.y > -1.025 && ball1.position.y < -0.975) {
            jump_status = false;
            speed = 0;
        }
        speed += 0.0025;
    }

    if (left) {
        move_horizontal(&ball1, 'l');
    }

    if (right) {
        move_horizontal(&ball1, 'r');
    }

    if (up) {
        jump_status = true;
        jump(&ball1);
    }
}

bool detect_firebeam_collision(FireLine fire) {
    float interim_x = (fire).position.x;
    float interim_y = (fire).position.y + 0.1;

    float ball_interim_x = ball1.position.x;
    float ball_interim_y = ball1.position.y + 0.5;

    float tangent = tan((fire).angle);

    float val1 = (ball_interim_x * tangent) - ball_interim_y - (interim_x * tangent) + interim_y;

    float interim_x_1 = (fire).position.x;
    float interim_y_1 = (fire).position.y - 0.1;

    float ball_interim_x_1 = ball1.position.x;
    float ball_interim_y_1 = ball1.position.y + 0.5;

    float val2 = (ball_interim_x_1 * tangent) - ball_interim_y_1 - (interim_x_1 * tangent) + interim_y_1;

    ////////////////////////////////////////////////

    float interim_x_2 = (fire).position.x;
    float interim_y_2 = (fire).position.y + 0.1;

    float ball_interim_x_2 = ball1.position.x + 0.5;
    float ball_interim_y_2 = ball1.position.y;

    tangent = tan((fire).angle);

    float val3 = (ball_interim_x_2 * tangent) - ball_interim_y_2 - (interim_x_2 * tangent) + interim_y_2;

    float interim_x_3 = (fire).position.x;
    float interim_y_3 = (fire).position.y - 0.1;

    float ball_interim_x_3 = ball1.position.x + 0.5;
    float ball_interim_y_3 = ball1.position.y;

    float val4 = (ball_interim_x_3 * tangent) - ball_interim_y_3 - (interim_x_3 * tangent) + interim_y_3;

    if ((val1 * val2 < 0 || val3 * val4 < 0) && ball1.position.x > (fire).position.x && ball1.position.x < (fire).final_position.x)
    {
        return true;
    }

    return false;
}

void tick_elements() {
    num_ticks++;
    ball1.tick();
    init_pos += 0.075;
    camera_rotation_angle += 0;
    bool flag = false;
    for (int i = 0; i < coin_arr.size(); i++) {
        coin_arr[i].position.x -= 0.075;
        coin_arr[i].box.x = coin_arr[i].position.x;
        if (coin_arr[i].position.x <= -4) {
            coin_arr.erase(coin_arr.begin() + i - 1);
            flag = true;
            break;
        }

        if (detect_collision(ball1.box, coin_arr[i].box)) {
            coin_arr.erase(coin_arr.begin() + i - 1);
            flag = true;
            break;
        }

    }
    if (flag == true) {

        Coin coin = Coin(4 - randomFloat(0, 0.5), randomFloat(0, 4), COLOR_BLACK);

        coin_arr.push_back(coin);
    }

    
    for (vector<FireBeam>::iterator it = fire_list.begin(); it != fire_list.end(); it++) {
        (*it).tick();
        if ((*it).position.x + (*it).length <= -4) {
            fire_list.erase(it);
            it--;
        }

        if (detect_collision(ball1.box, (*it).box)) {
            quit(window);
        }

    }

    for(vector<FireLine>::iterator it = fire_lint_list.begin(); it != fire_lint_list.end(); it++) {
        (*it).tick();

        if ((*it).position.x + (*it).length * cos((*it).angle) <= -4) {
            fire_lint_list.erase(it);
            it--;
        }
        
        if (detect_firebeam_collision(*it)) {
            cout << "COLLISION" << endl;
        }
    }
    

    if (num_ticks % 293 == 0) {
        FireBeam fire = FireBeam(4, randomFloat(-1, 2), rand() % 4);
        fire_list.push_back(fire);
    }

    if (num_ticks % 149 == 0) {
        FireLine fire = FireLine(4, randomFloat(0, 2), M_PI / ((rand() % 8) + 1), rand() % 2 + 1);
        fire_lint_list.push_back(fire);

    }

}

int num_coins = 0;

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(0, -1, COLOR_RED);
    ground      = Ground(0, 0, COLOR_GREEN);

    while (num_coins != 5) {
        Coin coin = Coin(randomFloat(-4, 4), randomFloat(0, 4), COLOR_BLACK);
        coin_arr.push_back(coin);
        num_coins++;
    }

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();

            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    cout << num_ticks << endl;

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
