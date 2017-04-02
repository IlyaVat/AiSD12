#ifndef SCREEN_H
#define SCREEN_H

const int XMAX = 80*2;
const int YMAX = 40*2;
const int rad = 6;

class point { //Точка
public:
    int x, y;
    point() {};
    point(int a, int b) :x(a), y(b) {}
};

//Функции работы с экраном
extern void put_point(int a, int b); //Вывод точки
inline void put_point(point p) { put_point(p.x, p.y); }
extern void put_line(int, int, int, int); //Вывод линии
extern void put_line(point a, point b);
extern void screen_init();    //Создание экрана
extern void screen_destroy(); //Удаление экрана
extern void screen_refresh(); //Обновление экрана
extern void screen_clear();   //Очистка экрана



#endif // SCREEN_H
