#ifndef SHAPE_H
#define SHAPE_H
#include <iostream>
#include <conio.h>
#include "screen.h"
#include <exception>

#define Test 5
#define OutOfScreen 1
#define WrongParam 2
#define NotEnoughSpace 3

using namespace std;

class ForbiddenCoord
{
//private:
//    char msg;

//public:
//    MyError()
//    {
//        msg=0;
//    }

//    MyError(char newMsg)
//    {
//        msg=newMsg;
//    }

//    char what()
//    {
//        return msg;
//    }

};

void screen_init()
{
    for (int y = 0; y < YMAX; ++y)
        for (int x = 0; x < XMAX; ++x)
            screen[x][y] = white;
}

int on_screen(int a, int b) //Попадание на экран
{
    return (0 <= a && a < XMAX && 0 <= b && b < YMAX);
}

void put_point(int a, int b)
{
    //MyError exp(OutOfScreen);
    if (on_screen(a, b)) screen[a][b] = black;
    else throw ForbiddenCoord();
}

void put_line(int x0, int y0, int x1, int y1)
{
    /*
    Рисование отрезка прямой (x0, y0) - (x1, y1)
    Уравнение прямой: b(x-x0) + a(y-y0) = 0
    Минимизируется величина abs(eps)
    где eps = 2*(b(x-x0))+a(y-y0)
//    */
//    int dx = 1;
//    int a = x1 - x0;
//    if (a < 0) dx = -1, a = -a;
//    int dy = 1;
//    int b = y1 - y0;
//    if (b < 0) dy = -1, b = -b;
//    int two_a = 2 * a;
//    int two_b = 2 * b;
//    int xcrit = -b + two_a;
//    int eps = 0;
//    put_point(x1, y1);
//    while(x0 == x1 && y0 == y1) {
//        put_point(x0, y0);
//        if (eps <= xcrit) x0 += dx, eps += two_b;
//        if (eps >= a || a < b)y0 += dy, eps -= two_a;
//    }


    const int deltaX = abs(x1 - x0);
    const int deltaY = abs(y1 - y0);
    const int signX = x0 < x1 ? 1 : -1;
    const int signY = y0 < y1 ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    put_point(x1, y1);
    while(x0 != x1 || y0 != y1)
   {
        put_point(x0, y0);
        const int error2 = error * 2;
        //
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x0 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y0 += signY;
        }
    }
}

extern void put_line(point a, point b)
{
    put_line(a.x, a.y, b.x, b.y);
}

void screen_clear() { screen_init(); }

void screen_refresh()
{
    for (int y = 0; y < YMAX; ++y)
    {
        for (int x = 0; x < XMAX; ++x)
            cout << screen[x][y];
        cout << '\n';
    }
}

struct shape {
    static shape* list;
    shape* next;
    shape() { next = list; list = this; }

    virtual point north() const = 0;
    virtual point south() const = 0;
    virtual point east() const = 0;
    virtual point west() const = 0;
    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;
    virtual point center() const = 0;

    virtual void draw() = 0;
    virtual void move(int, int) = 0;
};

shape* shape::list = nullptr; //Иницализация списка фигур

class rotatable : public virtual shape { //Фигуры, пригодные к повороту
public:
    virtual void rotate_left() = 0;
    virtual void rotate_right() = 0;
};

class reflectable : public virtual shape //Фигуры пригодные к зеркальному отражению
{
public:
    virtual void flip_horisontally() = 0;
    virtual void flip_vertically() = 0;
};

class line : public shape {
    /*
    Отрезок прямой ["w", "e"]
    north() определяет точку "выше центра отрезка и так далеко на север,
    как самая его северная точка", и т.п.
    */
    point w, e;
public:
    line(point a, point b) : w(a), e(b) {};
    line(point a, int L) : w(a), e(point(a.x + L - 1, a.y)) {};

    point north() const { return point((w.x + e.x) / 2, e.y<w.y ? w.y : e.y); }
    point south() const { return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y); }
    point east() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point west() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point neast() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point seast() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point nwest() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point swest() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point center() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }

    void move(int a, int b) {
        w.x += a;
        w.y += b;
        e.x += a;
        e.y += b;
    }

    void draw()
    {
        try
        {
            put_line(w, e);
        }
        catch(ForbiddenCoord)
        {
            cout << "coord error\n";

            if (e.x >= XMAX - 1)
                if(w.x <= 0)
                    w.x = 5;
                else
                    move(XMAX - e.x -2, 0);
            else if (w.x <= 0)
                 move(-w.x + 1, 0);

            if ((e.y<w.y ? w.y : e.y) >= YMAX - 1)
                move(0, YMAX - (e.y<w.y ? w.y : e.y) -2);
            else if ((e.y>w.y ? w.y : e.y) <= 0)
                 move(0, -w.y + 1);

            //radius -= 4;
            draw();
        }
    }
};

//Прямоугольник
class rectangle : public rotatable
{
    void rotate_right()//Поворот относительно se
    {
        int w = se.x - nw.x,
            h = se.y - nw.y;
        nw.x = se.x - h * 2;
        se.y = nw.y + w / 2;
    }
    void rotate_left() //Поворот относительно sw
    {
        int w = se.x - nw.x,
            h = se.y - nw.y;
        se.x = nw.x + h * 2;
        se.y = nw.y + w / 2;
    }
protected:
    /*
    nw-----n-----ne
    |             |
    |             |
    w      c      e
    |             |
    |             |
    sw-----s-----se
    */
    point nw, se;


public:
    rectangle(point, point);
    point north() const { return point((se.x + nw.x) / 2, nw.y); }
    point south() const { return point((se.x + nw.x) / 2, se.y); }
    point east() const { return point(se.x, (se.y + nw.y) / 2); }
    point west() const { return point(nw.x, (se.y + nw.y) / 2); }
    point neast() const { return point(se.x, nw.y); }
    point seast() const { return se; }
    point nwest() const { return nw; }
    point swest() const { return point(nw.x, se.y); }
    point center() const { return point(west().x + ((east().x - west().x) / 2), east().y); }
    void move(int a, int b)
    {
        nw.x += a;
        nw.y += b;
        se.x += a;
        se.y += b;
    }
    void draw();
};

rectangle::rectangle(point a, point b)
{
    if (a.x <= b.x)
        if (a.y <= b.y) {
            nw = a;
            se = b;
        }
        else {
            nw = point(a.x, b.y);
            se = point(b.x, a.y);
        }
    else
        if (a.y <= b.y) {
            nw = point(b.x, a.y);
            se = point(a.x, b.y);
        }
        else {
            se = b;
            nw = a;
        }
}

void rectangle::draw()
{
    try
    {
        point sw(nw.x, se.y);
        point ne(se.x, nw.y);
        put_line(nw, ne);
        put_line(ne, se);
        put_line(se, sw);
        put_line(sw, nw);
    }
    catch(ForbiddenCoord)
    {
        cout << "coord error\n";

        point sw(nw.x, se.y);
        point ne(se.x, nw.y);
        if (se.x >= XMAX - 1)
            if(nw.x <= 0)
                nw.x = 5;
            else
                move(XMAX - se.x -2, 0);
        if (nw.x <= 0)
             move(-nw.x + 1, 0);

        if (se.y >= YMAX - 1)
            move(0, YMAX - se.y -2);
        if ((nw.y>se.y ? se.y : nw.y) <= 0)
             move(0, -nw.y + 1);

        draw();
    }
}

void shape_refresh()//Перерисовка всех фигур
{
    screen_clear();
    for (shape* p = shape::list; p; p = p->next)
        p->draw();
    screen_refresh();
}

void up(shape* p, const shape* q)//Поместить p над q
{
    point n = q->north();
    point s = p->south();
    p->move(n.x - s.x, n.y - s.y - 1);
}

void left(shape* p, const shape* q)//Поместить p слева от q
{
    point n = q->west();
    point s = p->east();
    p->move(n.x - s.x, n.y - s.y - 1);
}

void right(shape* p, const shape* q)//Поместить p справа от q
{
    point n = q->east();
    point s = p->west();
    p->move(n.x - s.x, n.y - s.y - 1);
}

void mid(shape* p, const shape* q)//Поместить p в центр q
{
    point n = q->center();
    point s = p->center();
    p->move(n.x - s.x, n.y - s.y - 1);
}

#endif // SHAPE_H
