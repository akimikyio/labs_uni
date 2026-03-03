#include <GL/freeglut.h>     
#include <cmath>     
#include <vector>    
#include <cstdlib>  
#include <ctime>   
#include <algorithm>

// ��������� ����
const int WIDTH = 700;

// ����� pi
constexpr double PI = 3.14159265358979323846;

// ��������� �����
const float LAMP_RADIUS = 80.0f;   // ������ ��������
const float INSECT_SPEED = 200.0f;  // �������� ���������
const float ANGLE = PI / 3.0f; // ���� ����� ������������ � ���������
const float GRAVITY = -400.0f; // ��������� ���������� �������
const float DT = 0.016f;  // FPS �������� 60 
int dead = 0; //смерти мух

// ���������
struct Insect {
    float x, y;     // ������� �������
    float vx, vy;   // ������� ��������
    int spin;       // ����������� �������� (+1 ��� -1)
    bool falling;   // ������ ��� ���
};

// ������ ���������
std::vector<Insect> insects;

// ��������� ���������� ����� � ��������� ������
float randf(float a, float b) {
    return a + (b - a) * (float(rand()) / RAND_MAX);
}

// �������� ������ ����������
void spawnInsect() {
    Insect i;

    // ��������� ��������� ������� �� ����������
    float r = randf(LAMP_RADIUS*2, WIDTH/2);
    float ang = randf(0.0f, 2.0f * PI);

    i.x = r * cos(ang);
    i.y = r * sin(ang);

    // ��������� ��������
    i.vx = 0.0f;
    i.vy = 0.0f;

    // ��������� ����������� ��������
    i.spin = (rand() % 2 == 0) ? 1 : -1;

    // ��������� �����
    i.falling = false;

    insects.push_back(i);
}

// ���������� ��������� ���� ���������
void updateInsects() {
    for (auto& i : insects) {

        // ���� � ��������
        if (!i.falling) {

            // ������ �� ���������� � ��������
            float dx = -i.x;
            float dy = -i.y;

            // ���������� �� �����
            float dist = sqrt(dx * dx + dy * dy);

            // �������, ���� ��������� �������� ��������
            if (dist < LAMP_RADIUS) {
                i.falling = true;
                i.vy = 0.0f;
                continue;
            }

            // ������������ ������� �����������
            if (dist > 0.001f) {
                dx /= dist;
                dy /= dist;
            }
            
            // ���������������� ������
            float tx = -dy;
            float ty = dx;

            // ���� ����������� ��������
            float a = ANGLE * i.spin;

            // ��������
            float vx_dir = cos(a) * dx - sin(a) * dy;
            float vy_dir = sin(a) * dx + cos(a) * dy;
            i.vx = INSECT_SPEED * vx_dir;
            i.vy = INSECT_SPEED * vy_dir;

            // ��������� ��� ��� ��������
            i.vx += randf(-200.0f, 200.0f);
            i.vy += randf(-200.0f, 200.0f);
        }

        // ��������� �������
        else {
            i.vy += GRAVITY * DT;
        }

        // ���������� �������
        i.x += i.vx * DT;
        i.y += i.vy * DT;
    }
    // �������� ���������
    insects.erase(
        std::remove_if(insects.begin(), insects.end(),
            [](const Insect& i) {
                return (i.y < -(WIDTH/2) || i.x < -(WIDTH/2) || i.x >  WIDTH/2 || i.y >  WIDTH/2);
            }),
        insects.end()
    );
    dead++;
}

// ��������� ��������
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 50; ++i) {
        float a = 2.0f * PI * i / 50.0f;
        glVertex2f(cx + cos(a) * r, cy + sin(a) * r);
    }
    glEnd();
}

// ��������� �����
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ��������
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(0.0f, 0.0f, LAMP_RADIUS);

    // ���������
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (auto& i : insects)
        glVertex2f(i.x, i.y);
    glEnd();

    glutSwapBuffers();
}

// ������ ���������� ����� �������� 60 ��� � ���
void timer(int) {
    updateInsects();

    // ������������� ���������� ����� ���������
    if (rand() % 10 == 0)
        spawnInsect();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}


void init() {
    // ����� ���
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-(WIDTH/2), WIDTH / 2, -(WIDTH / 2), WIDTH / 2);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(nullptr)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, WIDTH);
    glutCreateWindow("Insects and Light");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
