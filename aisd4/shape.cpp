//#include "shape.h"

using namespace std;

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

circle::circle(point centre, int rad) : rectangle(point(centre.x - rad, centre.y - rad),
                                                  point(centre.x + rad, centre.y + rad))
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
                                                point(centre.x + rad, centre.y + rad))
{
    mid = centre;
    radius = rad;
}

void cross::draw()
{
    put_line(nwest(), seast());
    put_line(swest(), neast());
}

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
