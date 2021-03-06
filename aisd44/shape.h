#ifndef SHAPE_H
#define SHAPE_H
#include <iostream>
#include "screen.h"

using namespace std;

char screen[XMAX][YMAX];
enum color { black = '#', white = ' ' };

void screen_init()
{
    for (int y = 0; y < YMAX; ++y)
        for (int x = 0; x < XMAX; ++x)
            screen[x][y] = white;
}

int on_screen(int a, int b) //��������� �� �����
{
    return (0 <= a && a < XMAX && 0 <= b && b < YMAX);
}

void put_point(int a, int b)
{
    if (on_screen(a, b)) screen[a][b] = black;
}

void put_line(int x0, int y0, int x1, int y1)
{
    /*
    ��������� ������� ������ (x0, y0) - (x1, y1)
    ��������� ������: b(x-x0) + a(y-y0) = 0
    �������������� �������� abs(eps)
    ��� eps = 2*(b(x-x0))+a(y-y0)
    */
    int dx = 1;
    int a = x1 - x0;
    if (a < 0) dx = -1, a = -a;
    int dy = 1;
    int b = y1 - y0;
    if (b < 0) dy = -1, b = -b;
    int two_a = 2 * a;
    int two_b = 2 * b;
    int xcrit = -b + two_a;
    int eps = 0;

    for (;;) {
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        if (eps <= xcrit) x0 += dx, eps += two_b;
        if (eps >= a || a < b)y0 += dy, eps -= two_a;
    }
}

extern void put_line(point a, point b)
{
    put_line(a.x, a.y, b.x, b.y);
}

void screen_clear() { screen_init(); }

void screen_refresh()
{
    for (int y = YMAX - 1; 0 <= y; --y) {
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

shape* shape::list = nullptr; //������������ ������ �����

class rotatable : public virtual shape { //������, ��������� � ��������
public:
    virtual void rotate_left() = 0;
    virtual void rotate_right() = 0;
};

class reflectable : public virtual shape //������ ��������� � ����������� ���������
{
public:
    virtual void flip_horisontally() = 0;
    virtual void flip_vertically() = 0;
};

class line : public shape {
    /*
    ������� ������ ["w", "e"]
    north() ���������� ����� "���� ������ ������� � ��� ������ �� �����,
    ��� ����� ��� �������� �����", � �.�.
    */
    point w, e;
public:
    line(point a, point b) : w(a), e(b) {};
    line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {};

    point north() const { return point((w.x + e.x) / 2, e.y<w.y ? w.y : e.y); }
    point south() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point east() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point west() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point neast() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point seast() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point nwest() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point swest() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
    point center() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }

    void move(int a, int b) { w.x += a; w.y += b;e.x += a; e.y += b; }

    void draw() {
        put_line(w, e);
    }
};

//�������������
class rectangle : public rotatable
{
    void rotate_right()//������� ������������ se
    {
        int w = ne.x - sw.x, h = ne.y - sw.y;
        sw.x = ne.x - h * 2;
        ne.y = sw.y + w / 2;
    }
    void rotate_left() //������� ������������ sw
    {
        int w = ne.x - sw.x, h = ne.y - sw.y;
        ne.x = sw.x + h * 2;
        ne.y = sw.y + w / 2;
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
    point sw, ne;


public:
    rectangle(point, point);
    point north() const { return point((sw.x + ne.x) / 2, ne.y); }
    point south() const { return point((sw.x + ne.x) / 2, sw.y); }
    point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast() const { return ne; }
    point seast() const { return point(ne.x, sw.y); }
    point nwest() const { return point(sw.x, ne.y); }
    point swest() const { return sw; }
    point center() const { return point(west().x + ((east().x - west().x) / 2), east().y); }
    void move(int a, int b)
    {
        sw.x += a;
        sw.y += b;
        ne.x += a;
        ne.y += b;
    }
    void draw();
};

rectangle::rectangle(point a, point b)
{
    if (a.x <= b.x)
        if (a.y <= b.y) {
            sw = a;
            ne = b;
        }
        else {
            sw = point(a.x, b.y);
            ne = point(b.x, a.y);
        }
    else
        if (a.y <= b.y) {
            sw = point(b.x, a.y);
            ne = point(a.x, b.y);
        }
        else {
            sw = b;
            ne = a;
        }
}

void rectangle::draw()
{
    point nw(sw.x, ne.y);
    point se(ne.x, sw.y);
    put_line(nw, ne);
    put_line(ne, se);
    put_line(se, sw);
    put_line(sw, nw);
}

void shape_refresh()//����������� ���� �����
{
    screen_clear();
    for (shape* p = shape::list; p; p = p->next) p->draw();
    screen_refresh();
}

void up(shape* p, const shape* q)//��������� p ��� q
{
    point n = q->north();
    point s = p->south();
    p->move(n.x - s.x, n.y - s.y + 1);
}

void left(shape* p, const shape* q)//��������� p ����� �� q
{
    point n = q->west();
    point s = p->east();
    p->move(n.x - s.x, n.y - s.y + 1);
}

void right(shape* p, const shape* q)//��������� p ������ �� q
{
    point n = q->east();
    point s = p->west();
    p->move(n.x - s.x, n.y - s.y + 1);
}

void mid(shape* p, const shape* q)//��������� p � ����� q
{
    point n = q->center();
    point s = p->center();
    p->move(n.x - s.x, n.y - s.y + 1);
}

#endif // SHAPE_H
