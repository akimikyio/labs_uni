// ¬ заголовочном файле world.h определ€ютс€ типы будущих частиц, их физические характеристики,
// заголовки функций взаимодействи€ с физикой, общие константы

#ifndef WORLD_H
#define WORLD_H

#define MAX_PARTICLES 1000


#define SCREEN_WIDTH 1280                   // ширина экрана
#define SCREEN_HEIGHT 720                   // высота экрана

typedef enum {                              // определение типов частиц
    PARTICLE_FIREBALL,
    PARTICLE_SPARK
} ParticleType;

typedef struct {                            // определение параметров любой частицы
    ParticleType type;                      // тип частицы

    float r, g, b;                          // цвета
    float x, y;                             // координаты
    float vx, vy;                           // горизонтальные и вертикальные скорости
    float radius;                           // радиус
    float life;                             // врем€ жизни

    int alive;                              // состо€ние: жива/мертва
} Particle;


void initWorld(void);                       // заготовка функции инициализации симул€ции
void updateWorld(float dt);                 // заготовка функции обновлени€ симул€ции


extern Particle particles[MAX_PARTICLES];   // публичный массив с частицами (неопределЄнными)
extern int particleCount;                   // публичный счЄтчик переменных

#endif