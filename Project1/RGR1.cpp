#include <GL/freeglut.h>
#include <math.h>

#define MAX_BUBBLES 100  // Максимальное количество пузырьков

// Массивы для пузырьков
float bubble_x[MAX_BUBBLES];    // Позиция X
float bubble_y[MAX_BUBBLES];    // Позиция Y
float bubble_vx[MAX_BUBBLES];   // Скорость по X
float bubble_vy[MAX_BUBBLES];   // Скорость по Y  
float bubble_size[MAX_BUBBLES]; // Размер пузырька
float bubble_growth[MAX_BUBBLES]; // Скорость роста
int bubble_active[MAX_BUBBLES]; // 1 - активен, 0 - не активен
int bubble_count = 0;           // Текущее количество пузырьков

// Счетчик для создания пузырьков
int bubble_timer = 0;

//ribka
float fish1_x = 0.0f;      // Позиция X
float fish1_y = 0.0f;      // Позиция Y
float fish1_speed = 0.015f; // Скорость движения
int fish1_direction = 1;   // Направление: 1 - вправо, -1 - влево
float fish1_body_color[3] = { 1.0f, 0.2f, 0.8f }; // Розовый цвет
float fish1_tail_color[3] = { 1.0f, 0.3f, 0.9f }; // Светло-розовый

// 2 ribka
float fish2_x = -0.3f;  // Начальная позиция другая
float fish2_y = -0.1f;
float fish2_speed = 0.005f; // Другая скорость
int fish2_direction = -1;   // Начинает плыть влево
float fish2_body_color[3] = { 0.0f, 0.8f, 1.0f }; // Голубой
float fish2_tail_color[3] = { 0.2f, 0.9f, 1.0f };

//движение рыбёхи
void UpdateSingleFish(float* fish_x, float* fish_y, float speed, int* direction) {
    // Двигаем рыбку
    *fish_x += speed * (*direction);

    // Проверяем границы аквариума
    if (*fish_x > 0.6f) {  // Правая граница
        *direction = -1;
        *fish_x = 0.6f;
    }
    if (*fish_x < -0.6f) { // Левая граница
        *direction = 1;
        *fish_x = -0.6f;
    }

    // Немного изменяем Y-координату для эффекта плавания
    *fish_y = 0.1f * sin(*fish_x * 10.0f);
}

//отрисовка рыбки
void DrawSingleFish(float body_color[3], float tail_color[3], int direction) {
    glPushMatrix();

    // Применяем направление
    if (direction == -1) {
        glScalef(-1.0f, 1.0f, 1.0f);
    }

    // Тело рыбки
    glColor3fv(body_color);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(cos(angle) * 0.08f, sin(angle) * 0.04f);
    }
    glEnd();

    // Хвост рыбки
    glColor3fv(tail_color);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.08f, 0.0f);
    glVertex2f(-0.15f, 0.05f);
    glVertex2f(-0.15f, -0.05f);
    glEnd();

    // Глаз рыбки
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.03f, 0.01f);
    glEnd();

    glPopMatrix();
}

//рыбки меняют положение
void UpdateFish() {
    UpdateSingleFish(&fish1_x, &fish1_y, fish1_speed, &fish1_direction);
    UpdateSingleFish(&fish2_x, &fish2_y, fish2_speed, &fish2_direction);
}

//объявление пузырьков
void InitBubbles() {
    for (int i = 0; i < MAX_BUBBLES; i++) {
        bubble_active[i] = 0;  // Все пузырьки неактивны
    }
    bubble_count = 0;
    bubble_timer = 0;
}

void CreateBubbleFromFish(float fish_x, float fish_y, int fish_direction) {
    // Ищем свободный слот для пузырька
    for (int i = 0; i < MAX_BUBBLES; i++) {
        if (!bubble_active[i]) {
            // Начальная позиция - около рта рыбки
            bubble_x[i] = fish_x + 0.08f * fish_direction;
            bubble_y[i] = fish_y;

            // Начальная скорость - вверх и немного в сторону
            bubble_vx[i] = 0.001f * fish_direction;
            bubble_vy[i] = 0.003f;  // Основная скорость вверх

            // Начальный размер
            bubble_size[i] = 0.01f;

            // Скорость роста пузырька
            bubble_growth[i] = 0.0003f;

            bubble_active[i] = 1;
            bubble_count++;
            break;
        }
    }
}

void UpdateBubbles() {
    bubble_timer++;

    // Каждые 30 кадров пускаем пузырьки от рыбок
    if (bubble_timer % 30 == 0) {
        // Пузырьки от первой рыбки
        CreateBubbleFromFish(fish1_x, fish1_y, fish1_direction);

        // Пузырьки от второй рыбки (реже)
        if (bubble_timer % 60 == 0) {
            CreateBubbleFromFish(fish2_x, fish2_y, fish2_direction);
        }
    }

    // Обновляем все активные пузырьки
    for (int i = 0; i < MAX_BUBBLES; i++) {
        if (bubble_active[i]) {
            // ФИЗИКА ПУЗЫРЬКОВ:

            // 1. Движение (скорость + позиция)
            bubble_x[i] += bubble_vx[i];
            bubble_y[i] += bubble_vy[i];

            // 2. Ускорение (пузырьки ускоряются вверх)
            bubble_vy[i] += 0.00005f;

            // 3. Сопротивление воды для горизонтального движения
            bubble_vx[i] *= 0.995f;

            // 4. Рост пузырька
            bubble_size[i] += bubble_growth[i];

            // 5. Уменьшение скорости роста со временем
            bubble_growth[i] *= 0.999f;

            // Условия удаления пузырька:
            // - Вышел за верхнюю границу
            // - Слишком большой
            // - Вышел за боковые границы
            if (bubble_y[i] > 0.4f ||
                bubble_size[i] > 0.05f ||
                bubble_x[i] < -0.78f ||
                bubble_x[i] > 0.78f) {
                bubble_active[i] = 0;
                bubble_count--;
            }
        }
    }
}

void DrawBubble(float x, float y, float size) {
    // Включаем прозрачность
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Основной круг пузырька (голубой с прозрачностью)
    float alpha = 0.7f - (size / 0.05f) * 0.3f; // Чем больше, тем прозрачнее
    glColor4f(0.7f, 0.9f, 1.0f, alpha);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i += 15) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(x + cos(angle) * size,
            y + sin(angle) * size);
    }
    glEnd();

    // Блик на пузырьке (белый)
    glColor4f(1.0f, 1.0f, 1.0f, alpha + 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + size * 0.3f, y + size * 0.3f);
    glVertex2f(x + size * 0.6f, y + size * 0.3f);
    glVertex2f(x + size * 0.3f, y + size * 0.6f);
    glEnd();

    glDisable(GL_BLEND);
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void Draw(void)
{
    // Обновляем положение рыбки
    UpdateFish();

    // Обновляем физику пузырьков
    UpdateBubbles();

    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(1);

    //квадрат 1 белый
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glVertex2f(0.8, 0.5f);
    glVertex2f(0.8, -0.5f);
    glVertex2f(-0.8, -0.5f);
    glVertex2f(-0.8, 0.5f);
    glEnd();

    //квадрат 2 чёрный
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);

    glVertex2f(0.78, 0.5f);
    glVertex2f(0.78, -0.48f);
    glVertex2f(-0.78, -0.48f);
    glVertex2f(-0.78, 0.5f);

    glEnd();

    //квадрат 3 вода
    glColor3f(0.0f, 0.1f, 0.7f);

    glBegin(GL_QUADS);

    glVertex2f(0.78, 0.42f);
    glVertex2f(0.78, -0.48f);
    glVertex2f(-0.78, -0.48f);
    glVertex2f(-0.78, 0.42f);

    glEnd();

    //квадрат 4 песок
    glColor3f(0.9f, 0.7f, 0.3f);

    glBegin(GL_QUADS);

    glVertex2f(0.78, -0.32f);
    glVertex2f(0.78, -0.48f);
    glVertex2f(-0.78, -0.48f);
    glVertex2f(-0.78, -0.32f);

    glEnd();

    // Рисуем первую рыбку
    glPushMatrix();
    glTranslatef(fish1_x, fish1_y, 0.0f);
    DrawSingleFish(fish1_body_color, fish1_tail_color, fish1_direction);
    glPopMatrix();

    // Рисуем вторую рыбку
    glPushMatrix();
    glTranslatef(fish2_x, fish2_y, 0.0f);
    DrawSingleFish(fish2_body_color, fish2_tail_color, fish2_direction);
    glPopMatrix();

    // Рисуем все активные пузырьки
    for (int i = 0; i < MAX_BUBBLES; i++) {
        if (bubble_active[i]) {
            DrawBubble(bubble_x[i], bubble_y[i], bubble_size[i]);
        }
    }

    glutSwapBuffers();
}

// Таймер для обновления анимации
void Timer(int value) {
    glutPostRedisplay(); // Перерисовываем сцену
    glutTimerFunc(16, Timer, 0); // 60 FPS (1000ms / 60 ≈ 16ms)
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Аквариум с рыбкой");

    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);

    // Запускаем таймер для анимации
    glutTimerFunc(0, Timer, 0);

    glClearColor(0, 0, 0, 0);

    // Инициализируем пузырьки
    InitBubbles();

    glutMainLoop();
    return 0;
}