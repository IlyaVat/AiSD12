#include "shape.h"

#include <exception>

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
    l_eye = new line(point(swest().x + 4, swest().y + hh * 3/ 4), ll/5);
    r_eye = new line(point(swest().x + ll*4/5 - 4, swest().y + hh * 3/ 4), ll/5);
    mouth = new line(point(swest().x + 2, swest().y + hh / 4), ll - 4);
}

void myshape::draw()
{
    rectangle::draw();
    int a = (swest().x + neast().x) / 2;
    int b = (swest().y + neast().y) / 2;
//    put_point(point(a, b));
}

void myshape::move(int a, int b)
{
    rectangle::move(a, b);
    l_eye->move(a, b);
    r_eye->move(a, b);
    mouth->move(a, b);
}

class circle : public /*virtual*/ shape {
protected:
    point mid;
    int radius;
public:
    circle(point, int);
    point north() const { return point(mid.x, mid.y - radius); }
    point south() const { return point(mid.x, mid.y + radius); }
    point east() const  { return point(mid.x + radius, mid.y); }
    point west() const  { return point(mid.x - radius, mid.y); }
    point neast() const { return point(mid.x - radius, mid.y + radius); }
    point seast() const { return point(mid.x + radius, mid.y + radius); }
    point nwest() const { return point(mid.x - radius, mid.y - radius); }
    point swest() const { return point(mid.x + radius, mid.y - radius); }
    point center() const { return mid; }
    void move(int a, int b)
    {
        mid.x += a;
        mid.y += b;
    }
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
    void move(int a, int b)
    {
        circle :: move(a, b);
        cross :: move(a, b);
    }
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

    try
    {
        while (y >= 0)
        {
            put_point(point(mid.x + x, mid.y + y));
            put_point(point(mid.x + x, mid.y - y));
            put_point(point(mid.x - x, mid.y - y));
            put_point(point(mid.x - x, mid.y + y));

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
    }
    catch(ForbiddenCoord)
    {
        cout << "coord error\n";

        if ((center().x + radius) >= XMAX - 1)
            if((center().x - radius) <= 0)
                radius = 5;
            else
                move(XMAX - radius - center().x -2, 0);
        else if ((center().x - radius) <= 0)
             move(-(center().x - radius)+1, 0);

        if ((center().y + radius) >= YMAX - 1)
            move(0, YMAX - radius - center().y -2);
        else if ((center().y - radius) <= 0)
             move(0, -(center().y - radius)+1);

        //radius -= 4;
        draw();
    }


}

cross::cross(point centre, int rad) : rectangle(point(centre.x - rad, centre.y - rad),
                                                point(centre.x + rad, centre.y + rad)){}

void cross::draw()
{
    try
    {
        put_line(nwest(), seast());
        put_line(swest(), neast());
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
    rotatable* p1 = new rectangle(point(10*scale, 30*scale),
                                  point(20*scale, 40*scale));
    shape* p2 = new line(point(50, 50), 17*scale);
    shape* p3 = new myshape(point(17, 25),
                            point(27*scale, 30*scale));

    shape* l_ear = new circle(point(45, 56), rad);
    shape* nose = new circle(point(30, 20), rad);
    shape* r_ear = new circle(point(23, 28), rad);
    shape* l_earX = new cross(point(45, 56), rad);
    shape* noseX = new cross(point(30, 20), rad);
    shape* r_earX = new cross(point(23, 28), rad);

    //p1->rotate_right();
    shape_refresh();
     getch();
    //system("cls");
    up(p2, p3);
    up(p1, p3);
    //nose->move(20, 10);
    shape_refresh();
     getch();

    left(l_ear, p3);
    mid(nose, p3);
    right(r_ear, p3);
    left(l_earX, p3);
    mid(noseX, p3);
    right(r_earX, p3);

    shape_refresh();

    //getch();
    return 0;
}


            //move(, );
//            switch(error.what())
//            {
//                case OutOfScreen:
//                    cout << "ERROR ";
//                    //throw OutOfScreen;
//                    break;
////                case WrongParam:

////                    break;
////                case NotEnoughSpace:

////                    break;
////                case Test:

////                    break;
//                default:

//                    break;
//            }

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
