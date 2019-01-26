#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include <iostream>
#include "coins.h"
#include "enemy.h"
#include <bits/stdc++.h>
#include "balloon.h"
#include "segdisp.h"
#include "projectiles.h"
#include "semicircle.h"
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
JetPack jet;

vector <Segment> segments;
vector <Segment> life_segments;
vector <Segment> stage_segments;
vector <FireBeam> fire_list;
vector <FireLine> fire_lint_list;
vector <Balloon> balloon_list;
vector <SpeedUp> speed_list;
vector <CoinBoost> coin_boost_list;
vector <Boomerang> boomerang_list;
vector <Magnet> magnet_list;
vector <Ring> ring_list;
vector <Inverter> inv_list;

ll score = 0;
int stage = 1;

float init_pos = 0;

ll num_ticks = 0;

vector <Coin> coin_arr;
bool jump_status = false;
float speed = 0.05;

bool ring_trap = false;

float speed_x = 0;

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
        (*ball).position.x -= (0.05 + speed_x);   
        
    }

    else if (direction == 'r') {
        (*ball).position.x += (0.05 + speed_x);
    }

}

void WriteLife(int lives) {

    int i = 0;
    if (lives != 0) {

            if (lives != 1 && lives != 4 && lives != 7) {//Bottom        
                life_segments[7 * i].TurnBlue();                 //Bottom
            }                                               //Bottom

            if (lives == 2 || lives == 6 || lives == 8) {        //Left Down
                life_segments[7 * i + 1].TurnBlue();     //Left Down
            }       //Left Down

            if (lives != 1 && lives != 2 && lives != 3 && lives != 7) {         //Left Up
                life_segments[7 * i + 2].TurnBlue();         //Left Up
            }           //Left Up

            if (lives != 7 && lives != 1) {           //Middle
                life_segments[7 * i + 3].TurnBlue();         //Middle
            }           //Middle

            if (lives != 1 && lives != 4) {           //Top
                life_segments[7 * i + 4].TurnBlue();         //Top
            }           //Top

            if (lives != 2) {              //Right Down
                life_segments[7 * i + 5].TurnBlue();             //Right Down
            }               //Right Down

            if (lives != 5 && lives != 6) {       //Right Up
                life_segments[7 * i + 6].TurnBlue();     //Right Up
            }       //Right Up

        }
        else {
            for (int j = 0; j < 7; j++) {
                life_segments[7 * i + j].TurnBlue();
            }

            life_segments[7 * i + 3].TurnWhite();

        }

}

void WriteStage(int stage) {
    int i = 0;
    if (stage != 0) {

            if (stage != 1 && stage != 4 && stage != 7) {//Bottom        
                stage_segments[7 * i].TurnBlue();                 //Bottom
            }                                               //Bottom

            if (stage == 2 || stage == 6 || stage == 8) {        //Left Down
                stage_segments[7 * i + 1].TurnBlue();     //Left Down
            }       //Left Down

            if (stage != 1 && stage != 2 && stage != 3 && stage != 7) {         //Left Up
                stage_segments[7 * i + 2].TurnBlue();         //Left Up
            }           //Left Up

            if (stage != 7 && stage != 1) {           //Middle
                stage_segments[7 * i + 3].TurnBlue();         //Middle
            }           //Middle

            if (stage != 1 && stage != 4) {           //Top
                stage_segments[7 * i + 4].TurnBlue();         //Top
            }           //Top

            if (stage != 2) {              //Right Down
                stage_segments[7 * i + 5].TurnBlue();             //Right Down
            }               //Right Down

            if (stage != 5 && stage != 6) {       //Right Up
                stage_segments[7 * i + 6].TurnBlue();     //Right Up
            }       //Right Up

        }
        else {
            for (int j = 0; j < 7; j++) {
                stage_segments[7 * i + j].TurnBlue();
            }

            stage_segments[7 * i + 3].TurnWhite();

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
    jet.draw(VP);

    for (int i = 0; i < coin_arr.size(); i++) {
        coin_arr[i].draw(VP);
    }

    for (vector<FireBeam>::iterator it = fire_list.begin(); it != fire_list.end(); it++) {
        (*it).draw(VP);
    }

    for(vector <FireLine>::iterator it = fire_lint_list.begin(); it != fire_lint_list.end(); it++) {
        (*it).draw(VP);
    }

    for (vector <Balloon>::iterator it = balloon_list.begin(); it != balloon_list.end(); it++) {
        (*it).draw(VP);
    }

    for (vector <SpeedUp>::iterator it = speed_list.begin(); it != speed_list.end(); it++) {
        (*it).draw(VP);
    }

    for (vector <CoinBoost>::iterator it = coin_boost_list.begin(); it != coin_boost_list.end(); it++) {
        (*it).draw(VP);
    }

    for (vector <Boomerang>::iterator it = boomerang_list.begin(); it != boomerang_list.end(); it++) {
        it->draw(VP);
    }


    for (vector <Magnet>::iterator it = magnet_list.begin(); it != magnet_list.end(); it++) {
        it->draw(VP);
    }

    for (vector <Segment>::iterator it = segments.begin(); it != segments.end(); it++) {
        it->draw(VP);
    }

    for (vector <Segment>::iterator it = life_segments.begin(); it != life_segments.end(); it++) {
        it->draw(VP);
    }

    for (vector <Segment>::iterator it = stage_segments.begin(); it != stage_segments.end(); it++) {
        it->draw(VP);
    }

    for (vector <Ring>::iterator it = ring_list.begin(); it != ring_list.end(); it++) {
        it->draw(VP);
    }

    for (vector <Inverter>::iterator it = inv_list.begin(); it != inv_list.end(); it++) {
        it->draw(VP);
    }
}

void jump(Ball *ball) {
    if ((*ball).position.y <= 3.5)
        (*ball).position.y += 0.075;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (yoffset == 1) {
        screen_zoom += 0.1;
    }
    else if (yoffset == -1) {
        if (screen_zoom != 0.1)
            screen_zoom -= 0.1;
    }
    reset_screen();
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int f = glfwGetKey(window, GLFW_KEY_F);

    glfwSetScrollCallback(window, scroll_callback);

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
        jet.set_position(ball1.position.x, ball1.position.y);
        jet.propel();
    }

    if(!up) {
        jet.stop_propel();
    }

    if (f) {
        Balloon balloon = Balloon(ball1.position.x, ball1.position.y);
        balloon_list.push_back(balloon);
    }
}

void WriteScore(int hund, int ten, int one) {
    int num[] = {
        hund, ten, one,
    };

    for (int i = 0; i < 3; i++) {

        if (num[i] != 0) {

            if (num[i] != 1 && num[i] != 4 && num[i] != 7) {//Bottom        
                segments[7 * i].TurnBlue();                 //Bottom
            }                                               //Bottom

            if (num[i] == 2 || num[i] == 6 || num[i] == 8) {        //Left Down
                segments[7 * i + 1].TurnBlue();     //Left Down
            }       //Left Down

            if (num[i] != 1 && num[i] != 2 && num[i] != 3 && num[i] != 7) {         //Left Up
                segments[7 * i + 2].TurnBlue();         //Left Up
            }           //Left Up

            if (num[i] != 7 && num[i] != 1) {           //Middle
                segments[7 * i + 3].TurnBlue();         //Middle
            }           //Middle

            if (num[i] != 1 && num[i] != 4) {           //Top
                segments[7 * i + 4].TurnBlue();         //Top
            }           //Top

            if (num[i] != 2) {              //Right Down
                segments[7 * i + 5].TurnBlue();             //Right Down
            }               //Right Down

            if (num[i] != 5 && num[i] != 6) {       //Right Up
                segments[7 * i + 6].TurnBlue();     //Right Up
            }       //Right Up

        }
        else {
            for (int j = 0; j < 7; j++) {
                segments[7 * i + j].TurnBlue();
            }

            segments[7 * i + 3].TurnWhite();

        }



    }

}


bool detect_fireline_collision(Ball ball1, FireLine fire) {
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

bool detect_fireline_balloon_collision(Balloon balloon, FireLine fire) {
    float interim_x = (fire).position.x;
    float interim_y = (fire).position.y + 0.1;

    float ball_interim_x = balloon.position.x;
    float ball_interim_y = balloon.position.y + 0.25;

    float tangent = tan((fire).angle);

    float val1 = (ball_interim_x * tangent) - ball_interim_y - (interim_x * tangent) + interim_y;

    float interim_x_1 = (fire).position.x;
    float interim_y_1 = (fire).position.y - 0.1;

    float ball_interim_x_1 = balloon.position.x;
    float ball_interim_y_1 = balloon.position.y + 0.25;

    float val2 = (ball_interim_x_1 * tangent) - ball_interim_y_1 - (interim_x_1 * tangent) + interim_y_1;

    ////////////////////////////////////////////////

    float interim_x_2 = (fire).position.x;
    float interim_y_2 = (fire).position.y + 0.1;

    float ball_interim_x_2 = balloon.position.x + 0.25;
    float ball_interim_y_2 = balloon.position.y;

    tangent = tan((fire).angle);

    float val3 = (ball_interim_x_2 * tangent) - ball_interim_y_2 - (interim_x_2 * tangent) + interim_y_2;

    float interim_x_3 = (fire).position.x;
    float interim_y_3 = (fire).position.y - 0.1;

    float ball_interim_x_3 = balloon.position.x + 0.25;
    float ball_interim_y_3 = balloon.position.y;

    float val4 = (ball_interim_x_3 * tangent) - ball_interim_y_3 - (interim_x_3 * tangent) + interim_y_3;

    if ((val1 * val2 < 0 || val3 * val4 < 0) && balloon.position.x > (fire).position.x && balloon.position.x < (fire).final_position.x)
    {
        return true;
    }

    return false;
}

bool detect_firebeam_collision(FireBeam fire) {
    if (ball1.position.x + 0.5 > fire.position.x && ball1.position.x < fire.position.x + fire.length) {
        if (ball1.position.y < fire.position.y && ball1.position.y + 0.5 > fire.position.y + 0.3) {
            return true;
        }
    }
    return false;
}

bool detect_firebeam_balloon_collision(Balloon balloon, FireBeam fire) {
    if (balloon.position.x + 0.125 > fire.position.x && ball1.position.x < fire.position.x + fire.length) {
        if (balloon.position.y < fire.position.y + 0.3 && balloon.position.y > fire.position.y) {
            return true;
        }
    }
    return false;
}

bool detect_coin_collision(Coin coin) {
    if (coin.position.x > ball1.position.x && coin.position.x < ball1.position.x + 0.5) {
        if (coin.position.y > ball1.position.y && coin.position.y < ball1.position.y + 0.5) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool in_ball(Ball ball, pair <float, float> point) {
    if (point.first > ball.position.x && point.first < ball.position.x + 0.5) {
        if (point.second > ball.position.y && point.second < ball.position.y + 0.5) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool detect_boomerang_collision(Boomerang boomerang) {
    pair <float, float> p1, p2, p3, p4;
    
    p1.first = boomerang.position.x - 0.125;
    p1.second = boomerang.position.y;

    p2.first = boomerang.position.x - 0.125;
    p2.second = boomerang.position.y + 0.1875;

    p3.first = boomerang.position.x + 0.125;
    p3.second = boomerang.position.y;

    p4.first = boomerang.position.x + 0.125;
    p4.second = boomerang.position.y + 0.1875;


    if (in_ball(ball1, p1) || in_ball(ball1, p2) || in_ball(ball1, p3) || in_ball(ball1, p4)) {
        return true;
    }
    else {
        return false;
    }

}

float compute_distance(pair <float, float> p1, pair <float, float> p2) {
    return sqrt(((p1.first - p2.first) * (p1.first - p2.first)) + ((p1.second - p2.second) * (p1.second - p2.second)));
}

bool detect_ring_collision(Ring ring) {
    
    if ((ball1.position.y <= 2 * ring.position.y + ring.radius && ball1.position.y >= ring.position.y && ball1.position.y + 0.5 >= ring.position.y + ring.radius))
    {
        if (compute_distance(make_pair(ball1.position.x, ball1.position.y), make_pair(ring.position.x, ring.position.y)) <= ring.radius + 0.5) {

            return true;
        }
        else {
            return false;
        }
    }

    else {
        return false;
    }
}

void tick_elements() {
    num_ticks++;


    if (num_ticks % 2400 == 0) {
        stage += 1;
    }


    for (vector <Segment>::iterator it = segments.begin(); it != segments.end(); it++) {
        it->TurnWhite();
    }
    for (vector <Segment>::iterator it = life_segments.begin(); it != life_segments.end(); it++) {
        it->TurnWhite();
    }
    for (vector <Segment>::iterator it = stage_segments.begin(); it != stage_segments.end(); it++) {
        it->TurnWhite();
    }

    WriteLife(ball1.life);
    WriteScore(score/100, (score%100)/10, score % 10);
    WriteStage(stage);

    jet.set_position(ball1.position.x, ball1.position.y);
    
    init_pos += 0.075;
    
    camera_rotation_angle += 0;
    
    bool flag = false;

    ring_trap = false;

    Ring r;

    for (vector <Ring>::iterator it = ring_list.begin(); it != ring_list.end(); it++) {
        it->tick();
        if (detect_ring_collision(*it)) {
            ring_trap = true;
            r = *it;
        }

        if (it->position.x <= -20) {
            ring_list.erase(it);
            it--;
            break;
        }
    }

    if (ring_trap == false) {
        ball1.tick();
    }
    else {
        if (!isnan(sqrt((r.radius * r.radius) - (ball1.position.x - r.position.x) * (ball1.position.x - r.position.x)))) {
            ball1.position.y = r.position.y + sqrt((r.radius * r.radius) - (ball1.position.x - r.position.x) * (ball1.position.x - r.position.x));
        }
    }

    for (vector <Coin>::iterator it = coin_arr.begin(); it != coin_arr.end(); it++) {
        it->tick();
        it->speed_x = 0.075 + ((stage - 1) * 0.05);

        if (it->position.x <= -30) {
            coin_arr.erase(it);
            it--;
            flag = true;
            break;
        }

        else if (detect_coin_collision(*it)) {
            coin_arr.erase(it);
            it--;
            flag = true;
            score += 1;
            break;
        }

    }

    if (flag == true) {

        Coin coin = Coin(20 - randomFloat(0, 0.5), randomFloat(0, 3.5), COLOR_BLACK);

        coin_arr.push_back(coin);
    }

    
    for (vector<FireBeam>::iterator it = fire_list.begin(); it != fire_list.end(); it++) {
        (*it).tick();
        it->speed_x = 0.075 + ((stage - 1) * 0.05);
        if ((*it).position.x + (*it).length <= -40) {
            fire_list.erase(it);
            it--;
        }

        if (detect_firebeam_collision((*it)) && ring_trap == false) {
            fire_list.erase(it);
            it--;
            ball1.life --;
            if (ball1.life == 0) {
                quit(window);
            }
        }

    }

    for(vector<FireLine>::iterator it = fire_lint_list.begin(); it != fire_lint_list.end(); it++) {
        (*it).tick();
        it->speed_x = 0.075 + ((stage - 1) * 0.05);
        if ((*it).position.x + (*it).length * cos((*it).angle) <= -40) {
            fire_lint_list.erase(it);
            it--;
        }

        
        if (detect_fireline_collision(ball1, *it) && ring_trap == false) {
            fire_lint_list.erase(it);
            it--;
            ball1.life--;
            if (ball1.life == 0) {
                quit(window);
            }
        }
    }

    for (vector<Balloon>::iterator it = balloon_list.begin(); it != balloon_list.end(); it++) {
        (*it).tick();

        if ((*it).position.y <= -1) {
            balloon_list.erase(it);
            it--;
            break;
        }

        for (vector<FireBeam>::iterator itr = fire_list.begin(); itr != fire_list.end(); itr++) {
            if (detect_firebeam_balloon_collision((*it), (*itr))) {
                fire_list.erase(itr);
                itr--;
                balloon_list.erase(it);
                it--;
                score += 5;
                break;
            }
        }

        for (vector<FireLine>::iterator itr = fire_lint_list.begin(); itr != fire_lint_list.end(); itr++) {
            if (detect_fireline_balloon_collision(*it, *itr)) {
                fire_lint_list.erase(itr);
                itr--;
                balloon_list.erase(it);
                it--;
                score += 5;
                break;
            }
        }

    }

    for (vector<SpeedUp>::iterator it = speed_list.begin(); it != speed_list.end(); it++) {
        (*it).tick();

        if ((*it).position.y <= -1) {
            speed_list.erase(it);
            it--;
            break;
        }

        if (detect_collision((*it).box, ball1.box)) {
            speed_list.erase(it);
            it--;
            speed_x += 0.025;
        }
    }

    for (vector<Inverter>::iterator it = inv_list.begin(); it != inv_list.end(); it++) {
        (*it).tick();

        if ((*it).position.y <= -1) {
            inv_list.erase(it);
            it--;
            break;
        }

        if (detect_collision((*it).box, ball1.box)) {
            inv_list.erase(it);
            it--;
            camera_rotation_angle += 180;
        }
    }

    for (vector<CoinBoost>::iterator it = coin_boost_list.begin(); it != coin_boost_list.end(); it++) {
        (*it).tick();

        if ((*it).position.y <= -1) {
            coin_boost_list.erase(it);
            it--;
            break;
        }

        if (detect_collision((*it).box, ball1.box)) {
            coin_boost_list.erase(it);
            it--;
            score += 10;
        }
    }
    
    for (vector <Boomerang>::iterator it = boomerang_list.begin(); it != boomerang_list.end(); it++) {
        it->tick();
        if (detect_boomerang_collision(*it) && ring_trap == false) {
            ball1.life--;
            if (ball1.life == 0) {
                quit(window);
            }

            boomerang_list.erase(it);
            it--;
            break;

        }

        if (it->position.x >= 20) {
            boomerang_list.erase(it);
            it--;
            break;
        }
    }

    for (vector <Magnet>::iterator it = magnet_list.begin(); it != magnet_list.end(); it++) {
        it->tick();
        if(it->num_ticks >= 269) {
            magnet_list.erase(it);
            it--;
            ball1.speed_x = 0;
            ball1.speed_y = 0;
            break;
            
            
        }

        jump_status = true;

        float x_vec = it->position.x - ball1.position.x;
        float y_vec = it->position.y - ball1.position.y;

        float dist = sqrt(x_vec * x_vec + y_vec * y_vec);

        ball1.speed_x += 0.0001 * (x_vec / dist);
        ball1.speed_y += 0.0001 * (y_vec / dist);
    }



    if (num_ticks % 193 == 0) {
        FireBeam fire = FireBeam(4, randomFloat(-1, 2), rand() % 4 + 1);
        fire_list.push_back(fire);
    }

    if (num_ticks % 149 == 0) {
        FireLine fire = FireLine(4, randomFloat(0, 2), M_PI / ((rand() % 8) + 3), rand() % 2 + 1);

        if (stage > 1) {
            int zz = rand() % 2;
            if (zz == 1) {
                fire.rotate = true;
            }
        }

        fire_lint_list.push_back(fire);

    }

    if (num_ticks % 257 == 0) {
        SpeedUp speed = SpeedUp(4, rand() % 3 + 1);
        speed_list.push_back(speed);
    }

    if (num_ticks % 359 == 0) {
        CoinBoost booster = CoinBoost(4, rand() % 3 + 1);
        coin_boost_list.push_back(booster);
    }

    if (num_ticks % 299 == 0) {
        Boomerang boom = Boomerang(6, randomFloat(2.5, 3.8));
        boomerang_list.push_back(boom);
    }

    if (num_ticks % 717 == 0) {
        Magnet mag = Magnet(4, randomFloat(0, 3));
        magnet_list.push_back(mag);
    }

    if (num_ticks % 822 == 0) {
        Ring ring = Ring(4, randomFloat(0, 2));
        ring_list.push_back(ring);

    }

    if (num_ticks % 259 == 0) {
        Inverter inv = Inverter(4, randomFloat(0, 3));
        inv_list.push_back(inv);
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
    jet         = JetPack(0, -1);

    Segment hundred_seg1 = Segment(-3.9, 3.6, 0);          //Bottom
    Segment hundred_seg2 = Segment(-3.9, 3.65, 90);    //Left Down
    Segment hundred_seg3 = Segment(-3.9, 3.8, 90);    //Left Up
    Segment hundred_seg4 = Segment(-3.9, 3.75, 0);        //Middle
    Segment hundred_seg5 = Segment(-3.9, 3.9, 0);        //Top
    Segment hundred_seg6 = Segment(-3.75, 3.65, 90);   //Right Down
    Segment hundred_seg7 = Segment(-3.75, 3.8, 90);   //Right Up

    segments.push_back(hundred_seg1);
    segments.push_back(hundred_seg2);
    segments.push_back(hundred_seg3);
    segments.push_back(hundred_seg4);
    segments.push_back(hundred_seg5);
    segments.push_back(hundred_seg6);
    segments.push_back(hundred_seg7);

    Segment tens_seg1 = Segment(-3.6, 3.6, 0);    //Bottom
    Segment tens_seg2 = Segment(-3.6, 3.65, 90);  //Left Down
    Segment tens_seg3 = Segment(-3.6, 3.8, 90);   //Left Up
    Segment tens_seg4 = Segment(-3.6, 3.75, 0);   //Middle
    Segment tens_seg5 = Segment(-3.6, 3.9, 0);    //Top
    Segment tens_seg6 = Segment(-3.45, 3.65, 90); //Right Down
    Segment tens_seg7 = Segment(-3.45, 3.8, 90);  //Right Up

    segments.push_back(tens_seg1);
    segments.push_back(tens_seg2);
    segments.push_back(tens_seg3);
    segments.push_back(tens_seg4);
    segments.push_back(tens_seg5);
    segments.push_back(tens_seg6);
    segments.push_back(tens_seg7);

    Segment oness_seg1 = Segment(-3.3, 3.6, 0);    //Bottom
    Segment oness_seg2 = Segment(-3.3, 3.65, 90);  //Left Down
    Segment oness_seg3 = Segment(-3.3, 3.8, 90);   //Left Up
    Segment oness_seg4 = Segment(-3.3, 3.75, 0);   //Middle
    Segment oness_seg5 = Segment(-3.3, 3.9, 0);    //Top
    Segment oness_seg6 = Segment(-3.15, 3.65, 90); //Right Down
    Segment oness_seg7 = Segment(-3.15, 3.8, 90);  //Right Up

    segments.push_back(oness_seg1);
    segments.push_back(oness_seg2);
    segments.push_back(oness_seg3);
    segments.push_back(oness_seg4);
    segments.push_back(oness_seg5);
    segments.push_back(oness_seg6);
    segments.push_back(oness_seg7);

    Segment life_seg1 = Segment(-2.9, 3.6, 0);    //Bottom
    Segment life_seg2 = Segment(-2.9, 3.65, 90);  //Left Down
    Segment life_seg3 = Segment(-2.9, 3.8, 90);   //Left Up
    Segment life_seg4 = Segment(-2.9, 3.75, 0);   //Middle
    Segment life_seg5 = Segment(-2.9, 3.9, 0);    //Top
    Segment life_seg6 = Segment(-2.75, 3.65, 90); //Right Down
    Segment life_seg7 = Segment(-2.75, 3.8, 90);  //Right Up

    life_segments.push_back(life_seg1);
    life_segments.push_back(life_seg2);
    life_segments.push_back(life_seg3);
    life_segments.push_back(life_seg4);
    life_segments.push_back(life_seg5);
    life_segments.push_back(life_seg6);
    life_segments.push_back(life_seg7);

    Segment stage_seg1 = Segment(-2.5, 3.6, 0);    //Bottom
    Segment stage_seg2 = Segment(-2.5, 3.65, 90);  //Left Down
    Segment stage_seg3 = Segment(-2.5, 3.8, 90);   //Left Up
    Segment stage_seg4 = Segment(-2.5, 3.75, 0);   //Middle
    Segment stage_seg5 = Segment(-2.5, 3.9, 0);    //Top
    Segment stage_seg6 = Segment(-2.35, 3.65, 90); //Right Down
    Segment stage_seg7 = Segment(-2.35, 3.8, 90);  //Right Up

    stage_segments.push_back(stage_seg1);
    stage_segments.push_back(stage_seg2);
    stage_segments.push_back(stage_seg3);
    stage_segments.push_back(stage_seg4);
    stage_segments.push_back(stage_seg5);
    stage_segments.push_back(stage_seg6);
    stage_segments.push_back(stage_seg7);

    Ring ring = Ring(2  , 1);
    ring_list.push_back(ring);

    while (num_coins != 15) {
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

            if (ring_trap == false)
                tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

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
