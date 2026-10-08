#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <string>

// Game Objects & State
float playerX = 0.0f, playerZ = 8.0f;
float ballX = 0.0f, ballZ = 4.0f;
float ballVx = 0.0f, ballVz = 0.0f;
int score = 0;

// Goal Dimensions
const float GOAL_X_MIN = -3.0f;
const float GOAL_X_MAX = 3.0f;
const float GOAL_Z = -14.0f;

// Basic 3D Sphere Renderer (for Ball and Player Head)
void drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i <= stacks; ++i) {
        float lat0 = M_PI * (-0.5f + (float)(i - 1) / stacks);
        float z0  = std::sin(lat0);
        float zr0 = std::cos(lat0);

        float lat1 = M_PI * (-0.5f + (float)i / stacks);
        float z1  = std::sin(lat1);
        float zr1 = std::cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = 2 * M_PI * (float)(j - 1) / slices;
            float x = std::cos(lng);
            float y = std::sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0 * radius, y * zr0 * radius, z0 * radius);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1 * radius, y * zr1 * radius, z1 * radius);
        }
        glEnd();
    }
}

// 3D Player Model
void drawPlayer(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // Body (Blue Box)
    glColor3f(0.1f, 0.3f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f);
    glScalef(0.6f, 1.0f, 0.3f);
    
    // Draw Cube
    glBegin(GL_QUADS);
    // Front / Back / Sides
    glVertex3f(-0.5f, -0.5f,  0.5f); glVertex3f( 0.5f, -0.5f,  0.5f); glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f,  0.5f, -0.5f); glVertex3f( 0.5f,  0.5f, -0.5f); glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f,  0.5f); glVertex3f(-0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f); glVertex3f( 0.5f,  0.5f, -0.5f); glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f( 0.5f, -0.5f,  0.5f);
    glEnd();
    glPopMatrix();

    // Head (Yellow Sphere)
    glColor3f(1.0f, 0.8f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 1.6f, 0.0f);
    drawSphere(0.25f, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

// 3D Goal Post
void drawGoal() {
    glColor3f(0.9f, 0.9f, 0.9f);
    glLineWidth(4.0f);

    glBegin(GL_LINES);
    // Left Post
    glVertex3f(GOAL_X_MIN, 0.0f, GOAL_Z);
    glVertex3f(GOAL_X_MIN, 2.0f, GOAL_Z);
    // Right Post
    glVertex3f(GOAL_X_MAX, 0.0f, GOAL_Z);
    glVertex3f(GOAL_X_MAX, 2.0f, GOAL_Z);
    // Crossbar
    glVertex3f(GOAL_X_MIN, 2.0f, GOAL_Z);
    glVertex3f(GOAL_X_MAX, 2.0f, GOAL_Z);
    glEnd();
}

// 3D Field Pitch
void drawField() {
    // Green Pitch Ground
    glColor3f(0.15f, 0.55f, 0.15f);
    glBegin(GL_QUADS);
    glVertex3f(-12.0f, 0.0f, -16.0f);
    glVertex3f( 12.0f, 0.0f, -16.0f);
    glVertex3f( 12.0f, 0.0f,  12.0f);
    glVertex3f(-12.0f, 0.0f,  12.0f);
    glEnd();

    // Boundary White Lines
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-10.0f, 0.01f, -15.0f);
    glVertex3f( 10.0f, 0.01f, -15.0f);
    glVertex3f( 10.0f, 0.01f,  10.0f);
    glVertex3f(-10.0f, 0.01f,  10.0f);
    glEnd();
}

void processInput(GLFWwindow* window) {
    float speed = 0.12f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    playerZ -= speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  playerZ += speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  playerX -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) playerX += speed;
// Pitch Limits for player
    if (playerX < -9.5f) playerX = -9.5f;
    if (playerX > 9.5f)  playerX = 9.5f;
    if (playerZ < -14.5f) playerZ = -14.5f;
    if (playerZ > 9.5f)   playerZ = 9.5f;
}

void updatePhysics() {
    // Player vs Ball Collision
    float dx = ballX - playerX;
    float dz = ballZ - playerZ;
    float dist = std::sqrt(dx * dx + dz * dz);

    if (dist < 0.7f) {
        float nx = dx / dist;
        float nz = dz / dist;
        ballVx = nx * 0.35f;
        ballVz = nz * 0.35f;
    }

    // Apply Ball Velocity & Friction
    ballX += ballVx;
    ballZ += ballVz;
    ballVx *= 0.94f;
    ballVz *= 0.94f;

    // Goal Collision Check
    if (ballZ <= GOAL_Z && ballX >= GOAL_X_MIN && ballX <= GOAL_X_MAX) {
        score++;
        std::cout << "GOAL!!! Total Goals: " << score << std::endl;

        // Reset Ball & Player
        ballX = 0.0f; ballZ = 2.0f;
        ballVx = 0.0f; ballVz = 0.0f;
        playerX = 0.0f; playerZ = 7.0f;
    }
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D C++ Football Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);

    // Setup Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat lightPos[] = {0.0f, 15.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    std::cout << "Controls: Use W, A, S, D or ARROW KEYS to move.\nScore by kicking the ball into the goal!" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        updatePhysics();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Projection Settings
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (double)width / (double)height, 0.1, 100.0);

        // 3D Camera - Follows the player
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(playerX, 6.0f, playerZ + 7.0f,  // Camera Position
                  playerX, 0.0f, playerZ - 2.0f,  // Target LookAt
                  0.0f, 1.0f, 0.0f);              // Up Vector

        // Draw Scene
        drawField();
        drawGoal();
        drawPlayer(playerX, playerZ);

        // Draw Ball (White Sphere)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(ballX, 0.3f, ballZ);
        drawSphere(0.3f, 12, 12);
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
