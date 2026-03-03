#include <glut.h>     
#include <cmath>     
#include <vector>    
#include <cstdlib>  
#include <ctime>   
#include <algorithm>

// Параметры окна
const int WIDTH = 800;

// Число pi
constexpr double PI = 3.14159265358979323846;

// Параметры сцены
const float LAMP_RADIUS = 50.0f;   // радиус лампочки
const float INSECT_SPEED = 200.0f;  // скорость насекомых
const float ANGLE = PI / 3.0f; // угол между направлением и скоростью
const float GRAVITY = -400.0f; // ускорение свободного падения
const float DT = 0.016f;  // FPS примерно 60 

// Насекомое
struct Insect {
    float x, y;     // текущая позиция
    float vx, vy;   // текущая скорость
    int spin;       // направление вращения (+1 или -1)
    bool falling;   // падает или нет
};

// Список насекомых
std::vector<Insect> insects;

// Генерация случайного числа с плавающей точкой
float randf(float a, float b) {
    return a + (b - a) * (float(rand()) / RAND_MAX);
}

// Создание нового насекомого
void spawnInsect() {
    Insect i;

    // Случайная начальная позиция по окружности
    float r = randf(LAMP_RADIUS*2, WIDTH/2);
    float ang = randf(0.0f, 2.0f * PI);

    i.x = r * cos(ang);
    i.y = r * sin(ang);

    // Начальная скорость
    i.vx = 0.0f;
    i.vy = 0.0f;

    // Случайное направление вращения
    i.spin = (rand() % 2 == 0) ? 1 : -1;

    // Насекомое летит
    i.falling = false;

    insects.push_back(i);
}

// Обновление состояния всех насекомых
void updateInsects() {
    for (auto& i : insects) {

        // Полёт к лампочке
        if (!i.falling) {

            // Вектор от насекомого к лампочке
            float dx = -i.x;
            float dy = -i.y;

            // Расстояние до лампы
            float dist = sqrt(dx * dx + dy * dy);

            // Падение, если насекомое достигло лампочки
            if (dist < LAMP_RADIUS) {
                i.falling = true;
                i.vy = 0.0f;
                continue;
            }

            // Нормализация вектора направления
            if (dist > 0.001f) {
                dx /= dist;
                dy /= dist;
            }
            
            // Перпендикулярный вектор
            float tx = -dy;
            float ty = dx;

            // Учёт направления вращения
            float a = ANGLE * i.spin;

            // Скорость
            float vx_dir = cos(a) * dx - sin(a) * dy;
            float vy_dir = sin(a) * dx + cos(a) * dy;
            i.vx = INSECT_SPEED * vx_dir;
            i.vy = INSECT_SPEED * vy_dir;

            // Случайный шум при движении
            i.vx += randf(-200.0f, 200.0f);
            i.vy += randf(-200.0f, 200.0f);
        }

        // Свободное падение
        else {
            i.vy += GRAVITY * DT;
        }

        // Обновление позиции
        i.x += i.vx * DT;
        i.y += i.vy * DT;
    }
    // Удаление насекомых
    insects.erase(
        std::remove_if(insects.begin(), insects.end(),
            [](const Insect& i) {
                return (i.y < -(WIDTH/2) || i.x < -(WIDTH/2) || i.x >  WIDTH/2 || i.y >  WIDTH/2);
            }),
        insects.end()
    );
}

// Рисование лампочки
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 50; ++i) {
        float a = 2.0f * PI * i / 50.0f;
        glVertex2f(cx + cos(a) * r, cy + sin(a) * r);
    }
    glEnd();
}

// Отрисовка кадра
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Лампочка
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(0.0f, 0.0f, LAMP_RADIUS);

    // Насекомые
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (auto& i : insects)
        glVertex2f(i.x, i.y);
    glEnd();

    glutSwapBuffers();
}

// Таймер обновления кадра примерно 60 раз в сек
void timer(int) {
    updateInsects();

    // Периодическое добавление новых насекомых
    if (rand() % 20 == 0)
        spawnInsect();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}


void init() {
    // Белый фон
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
