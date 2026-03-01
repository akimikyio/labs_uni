#include <GL/freeglut.h>

#include "world.h"
#include "render.h"

// callback для отрисовки
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    drawWorld();                        // вызываем отрисовку
    glutSwapBuffers();
}

// callback для кадров
static void timer(int value) {          
    const float dt = 0.016f;            // время кадра

    updateWorld(dt);                    // обновляем физику каждый кадр

    glutPostRedisplay();                // обновляем отрисовку каджый кадр
    glutTimerFunc(16, timer, 0);        // вызываем отрисовку в следующем кадре
}

// callback для изменения размеров
static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Fireworks");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initWorld();                        // создаём симуляцию с начальными параметрами

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);        // запускаем первый таймер

    glutMainLoop();
    return 0;
}