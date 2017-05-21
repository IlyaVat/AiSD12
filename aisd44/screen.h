#ifndef SCREEN_H
#define SCREEN_H

const int XMAX = 80*2;
const int YMAX = 40*2;
const int rad = 6;

class point { //�����
public:
    int x, y;
    point() {};
    point(int a, int b) :x(a), y(b) {}
};

//������� ������ � �������
extern void put_point(int a, int b); //����� �����
inline void put_point(point p) { put_point(p.x, p.y); }
extern void put_line(int, int, int, int); //����� �����
extern void put_line(point a, point b);
extern void screen_init();    //�������� ������
extern void screen_destroy(); //�������� ������
extern void screen_refresh(); //���������� ������
extern void screen_clear();   //������� ������



#endif // SCREEN_H
