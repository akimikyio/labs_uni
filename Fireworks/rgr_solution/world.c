// непосредственно обработка происходит в файле world.c

#include "world.h"
#include "utils.h"

#include <math.h>


Particle particles[MAX_PARTICLES];                      // массив частиц
int particleCount = 0;                                  // счётчик частиц


static float spawnTimer = 0.0f;                         // время с появления последней большой частицы
static const float SPAWN_INTERVAL = 1.2f;               // интервал появления частиц


static void addParticle(Particle p) {                   // функция добавления любой частицы
    if (particleCount < MAX_PARTICLES) {                // добавим только, если есть место
        particles[particleCount++] = p;                 // обновляем счётчик и добавляем частицу
    }
}

static void spawnFireball(void) {                       // функция добавления большой частицы
    Particle p;

    p.type = PARTICLE_FIREBALL;                         // задание типа

    p.x = randFloat(50.0f, SCREEN_WIDTH - 50.0f);       // задание случайной позиции по x
    p.y = randFloat(0.0f, 30.0f);                       // внизу экрана

    p.vx = 0.0f;                                       
    p.vy = 180.0f;                                      // частица летит вверх

    p.radius = 5.0f;                                    // начальный радиус
    p.life = 10.0f;                                     // время жизни
    p.alive = 1;                                        // частица ещё жива

    addParticle(p);                                     // добавляем частицу в массив
}

static void explode(float x, float y) {                     // функция взрыва большой частицы
    const int count = 40;                                   // количество вылетающих частиц

    for (int i = 0; i < count; ++i) {                       // рассчитываем параметры для каждой из частиц
        float angle = randFloat(0.0f, 2.0f * 3.1415926f);   // угол
        float speed = randFloat(60.0f, 300.0f);             // скорость

        Particle s;                                         // присваиваем параметры частице
        s.type = PARTICLE_SPARK;                            // тип
        s.x = x;                                            // координата x
        s.y = y;                                            // координата y
        s.vx = cosf(angle) * speed;                         // скорость по x
        s.vy = sinf(angle) * speed;                         // скорость по y
        s.radius = 2.0f;                                    // радиус
        s.life = 7.5f;                                      // время жизни
        s.alive = 1;                                        // частица пока жива

        addParticle(s);                                     // добавляем частицу в массив
    }
}

static void removeDead(void) {                              // убираем неживые частицы
    int i = 0;                                              // счётчик пройденных частиц
    while (i < particleCount) {                             // пока не обработали все частицы
        if (!particles[i].alive) {                          // проверяем их на то, живы ли они
            particles[i] = particles[particleCount - 1];    // если нет, то заменяем частицу первой с конца
            particleCount--;                                // и уменьшаем количество живых частиц
        }
        else {
            i++;                                            // если частица жива, то засчитываем проход
        }
    }
}


void initWorld(void) {                                      // инициализируем симуляцию с начальными параметрами
    particleCount = 0;                                      // частиц ещё нет
    spawnTimer = 0.0f;                                      // ни одной большой ещё не выпущено, так что таймер = 0
}

void updateWorld(float dt) {                                // функция обновления мира

    spawnTimer += dt;                                       // каждый кадр добавляем к таймеру время кадра
    if (spawnTimer >= SPAWN_INTERVAL) {                     // как только время превысило интервал
        spawnFireball();                                    // создаём большую частицу
        spawnTimer = 0.0f;                                  // и сбрасываем таймер
    }

    for (int i = 0; i < particleCount; ++i) {               // обновляем состояния всех активных частиц
        Particle* p = &particles[i];                        // обращаемся к существующей частице, а не к копии (по указателю)
        if (!p->alive) continue;                            // если частица мертва, то обрабатывать не будем

        p->x += p->vx * dt;                                 // интегрирум координаты
        p->y += p->vy * dt;                                 // по x и y

        if (p->type == PARTICLE_FIREBALL) {                 // если частица большая
            p->radius += 20.0f * dt;                        // то каждый кадр увеличиваем её в размерах

            if (p->radius >= 30.0f) {                       // как только частица становится больше заданного радиуса
                explode(p->x, p->y);                        // взрываем её
                p->alive = 0;                               // и переводим в ранг мёртвых
            }
        }

        if (p->type == PARTICLE_SPARK) {                    // если частица мелкая
            p->vy -= 60.0f * dt;                            // то воздействуем на неё гравитацией каждый кадр
        }

        p->life -= dt;                                      // и уменьшаем время жизни каждый кадр
        if (p->life <= 0.0f)                                // если оно закончилось
            p->alive = 0;                                   // переводим в ранг мёртвых
    }

    removeDead();                                           // удаляем все мёртвые частицы в конце кадра
}