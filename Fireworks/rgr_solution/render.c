#include "render.h"

#include <GL/freeglut.h>
#include <math.h>

#define PI 3.1415926

static void drawCircle(int x, int y, int r) {		// функция отрисовки круга
	glBegin(GL_POLYGON);							// рисуем круг из полигона с 50 вершинами
	for (int i = 0; i < 50; i++) {
		float angle = 2.0 * (float)i / 50.0 * PI;	// для каждой вершины вычисляем угол
		float dx = r * cosf(angle);					// координату x относительно 0
		float dy = r * sinf(angle);					// координату y относительно 0
		glVertex2f(x + dx, y + dy);					// отрисовываем вершину относительно центра окружности
	}
	glEnd();
}

void drawWorld(void) {								// функция отрисовки мира
    for (int i = 0; i < particleCount; ++i) {		// отрисовываем все частицы
        if (!particles[i].alive) continue;			// которые являются живыми

        if (particles[i].type == PARTICLE_FIREBALL)	// выбираем цвет для отрисовки разных типов частиц
            glColor3f(1.0f, 0.8f, 0.2f);
        else
            glColor3f(particles[i].r, particles[i].g, particles[i].b);

        drawCircle(particles[i].x, particles[i].y, particles[i].radius); // непосредственно рисуем частицу
    }
}