#ifndef SCREEN_H
#define SCREEN_H

const int scale = 2;
const int XMAX = 80*scale;
const int YMAX = 40*scale;
const int rad = 4 * scale;

char screen[XMAX][YMAX];
enum color { black = '*', white = ' ' };

class point //�����
{
public:
    int x, y;
    point() {};
    point(int a, int b) :x(a), y(b) {}
};

//������� ������ � �������
extern int on_screen(int a, int b);
extern int on_screen(point p) { return on_screen(p.x, p.y); }
extern void put_point(int a, int b); //����� �����
inline void put_point(point p) { put_point(p.x, p.y); }
extern void put_line(int, int, int, int); //����� �����
extern void put_line(point a, point b);
extern void screen_init();    //�������� ������
extern void screen_destroy(); //�������� ������
extern void screen_refresh(); //���������� ������
extern void screen_clear();   //������� ������



#endif // SCREEN_H
