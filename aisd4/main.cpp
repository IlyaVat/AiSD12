#include "shape.h"

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

class circle : public virtual shape {
protected:
    point mid;
    int radius;
public:
    circle(point, int);
    void draw();
};

class cross : public rectangle
{
public:
    cross(point, int);
    void draw();
};

class target : public cross, public circle
{
public:
    target(point, int);
    void draw();
};

circle::circle(point centre, int rad)
{
    mid = centre;
    radius = rad;
}

void circle::draw()
{
    int x = 0,
        y = radius,
        gap = 0,
        delta = (2 - 2 * radius);

    while (y >= 0)
    {
        put_point(point(center().x + x, center().y + y));
        put_point(point(center().x + x, center().y - y));
        put_point(point(center().x - x, center().y - y));
        put_point(point(center().x - x, center().y + y));
        gap = 2 * (delta + y) - 1;
        if (delta < 0 && gap <= 0)
        {
            x++;
            delta += 2 * x + 1;
            continue;
        }
        if (delta > 0 && gap > 0)
        {
            y--;
            delta -= 2 * y + 1;
            continue;
        }
        x++;
        delta += 2 * (x - y);
        y--;
    }

//    int rad = (ne.x - sw.x) / 2;
//    int hrw = 0;
//    point nw(sw.x, ne.y);
//    for (int y = -rad + 1; y < rad; y++)
//    {
//        hrw = sqrt(rad*(rad - 3) - y*(y));
//        //for (int x = -hrw; x < hrw; x++)
//        put_point(point(center().x + hrw, center().y + y));
//        put_point(point(center().x - hrw, center().y + y));
//        put_point(point(center().x + y, center().y + y));
//        put_point(point(center().x - y, center().y + y));
//    }
}

cross::cross(point centre, int rad) : rectangle(point(centre.x - rad, centre.y - rad),
                                                point(centre.x + rad, centre.y + rad)){}

void cross::draw()
{
    put_line(nwest(), seast());
    put_line(swest(), neast());
}

target::target(point centre, int rad) : circle(centre, rad),
                                        cross(centre, rad){}

void target::draw()
{
    circle::draw();
    cross::draw();
}

int main()
{
    screen_init();
    rotatable* p1 = new rectangle(point(0, 0), point(14*3, 3*3));
    shape* p2 = new line(point(0 * XMAX, 0.375*YMAX), 0.425*YMAX);
    shape* p3 = new myshape(point(0.1875 * XMAX, 0.25 * YMAX),
                            point(0.3375 * XMAX * 1.5, 0.45 * YMAX * 1.5));

    shape* l_ear = new target(point(45, 56), rad);
    shape* nose = new target(point(12, 34), rad);
    shape* r_ear = new target(point(23, 28), rad);

    p1->rotate_right();
    shape_refresh();
    system("pause");
    up(p2, p3);
    up(p1, p3);
    shape_refresh();
    system("pause");

    left(l_ear, p3);
    mid(nose, p3);
    right(r_ear, p3);

    shape_refresh();
    system("pause");
    return 0;
}
