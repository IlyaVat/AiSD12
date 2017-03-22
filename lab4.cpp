// lab4.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>

using namespace std;

const int XMAX = 80*2;
const int YMAX = 40*2;
const int rad = 10;
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
extern void put_line(point a, point b) { put_line(a.x, a.y, b.x, b.y); }
extern void screen_init();    //Создание экрана
extern void screen_destroy(); //Удаление экрана
extern void screen_refresh(); //Обновление экрана
extern void screen_clear();   //Очистка экрана

char screen[XMAX][YMAX];

enum color { black = '#', white = ' ' };

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
	if (on_screen(a, b)) screen[a][b] = black;
}

void put_line(int x0, int y0, int x1, int y1)
{
	/*
	Рисование отрезка прямой (x0, y0) - (x1, y1)
	Уравнение прямой: b(x-x0) + a(y-y0) = 0
	Минимизируется величина abs(eps)
	где eps = 2*(b(x-x0))+a(y-y0)
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

shape* shape::list = nullptr; //Иницализация списка фигур

class rotatable : public shape { //Фигуры, пригодные к повороту
public:
	virtual void rotate_left() = 0;
	virtual void rotate_right() = 0;
};

class reflectable : public virtual shape {//Фигуры пригодные к зеркальному отражению
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

//Прямоугольник
class rectangle :public rotatable {
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
	void rotate_right()//Поворот относительно se
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		sw.x = ne.x - h * 2;
		ne.y = sw.y + w / 2;
	}
	void rotate_left() //Поворот относительно sw
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		ne.x = sw.x + h * 2;
		ne.y = sw.y + w / 2;
	}
	void move(int a, int b)
	{
		sw.x += a;
		sw.y += b;
		ne.x += a;
		ne.y += b;
	}
	void draw();
};

void rectangle::draw()
{
	point nw(sw.x, ne.y);
	point se(ne.x, sw.y);
	put_line(nw, ne);
	put_line(ne, se);
	put_line(se, sw);
	put_line(sw, nw);
}


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

class circle : public rectangle {
	/*
	nw	-----n-----   ne
	*  \         /  *
	|     \     /     |
	|       \ /       |
	w        c        e
	|       / \       |
	|     /     \     |
	*  /         \  *
	sw  -----s-----   se

	*/
	//point sw, ne;
public:
	circle(point, point);
	/*point center() const { return point((sw.x + ne.x) / 2, (sw.y + ne.y) / 2); }
	point north() const { return point((sw.x + ne.x) / 2, ne.y); }
	point south() const { return point((sw.x + ne.x) / 2, sw.y); }
	point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
	point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
	point neast() const { return ne; }
	point seast() const { return point(ne.x, sw.y); }
	point nwest() const { return point(sw.x, ne.y); }
	point swest() const { return sw; }
	void move(int a, int b)
	{
	sw.x += a;
	sw.y += b;
	ne.x += a;
	ne.y += b;
	}*/
	void draw();
};

circle::circle(point a, point b) : rectangle(a, b)
{
	sw = a;
	ne = b;
}

void circle::draw()
{
	int rad = (ne.x - sw.x) / 2;
	int hrw = 0;
	point nw(sw.x, ne.y);
	for (int y = -rad + 1; y < rad; y++)
	{
		hrw = sqrt(rad*(rad - 3) - y*(y));
		//for (int x = -hrw; x < hrw; x++)
		put_point(point(center().x + hrw, center().y + y));
		put_point(point(center().x - hrw, center().y + y));
		put_point(point(center().x + y, center().y + y));
		put_point(point(center().x - y, center().y + y));
	}
}

void shape_refresh()//Перерисовка всех фигур
{
	screen_clear();
	for (shape* p = shape::list; p; p = p->next) p->draw();
	screen_refresh();
}

void up(shape* p, const shape* q)//Поместить p над q
{
	point n = q->north();
	point s = p->south();
	p->move(n.x - s.x, n.y - s.y + 1);
}



class myshape : public rectangle {
	line* l_eye; //левый глаз
	line* r_eye; //правый глаз
	line* mouth; //рот
public:
	myshape(point, point);
	void draw();
	void move(int, int);
};

myshape::myshape(point a, point b) : rectangle(a, b)
{
	int ll = neast().x - swest().x + 1;
	int hh = neast().y - swest().y + 1;
	l_eye = new line(point(swest().x + 4, swest().y + hh * 3 / 4), ll/5);
	r_eye = new line(point(swest().x + ll*4/5 - 4, swest().y + hh * 3 / 4), ll/5);
	mouth = new line(point(swest().x + 2, swest().y + hh / 4), ll - 4);
}

void myshape::draw()
{
	rectangle::draw();
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	put_point(point(a, b));
}

void myshape::move(int a, int b)
{
	rectangle::move(a, b);
	l_eye->move(a, b);
	r_eye->move(a, b);
	mouth->move(a, b);
}

int main()
{
	screen_init();
	rotatable* p1 = new rectangle(point(0, 0), point(14*3, 3*3));
	shape* p2 = new line(point(0 * XMAX, 0.375*YMAX), 0.425*YMAX);
	shape* p3 = new myshape(point(0.1875 * XMAX, 0.25 * YMAX), point(0.3375 * XMAX * 1.5, 0.45 * YMAX * 1.5));
	p1->rotate_right();
	shape_refresh();
	system("pause");
	up(p2, p3);
	up(p1, p3);
	shape_refresh();
	system("pause");
	shape* l_ear = new circle(point(p3->west().x-2*rad,p3->west().y-rad), 
							  point(p3->west().x, p3->west().y + rad));
	shape* nose = new circle(point(p3->center().x - rad, p3->center().y - rad),
							 point(p3->center().x + rad, p3->center().y + rad));
	shape* r_ear = new circle(point(p3->east().x, p3->east().y - rad),
							 point(p3->east().x + 2*rad, p3->east().y + rad));
	shape_refresh();
	system("pause");
    return 0;
}

