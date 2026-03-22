#ifndef PROG_COLOR_H
#define PROG_COLOR_H

struct color {
    int r, g, b, a;
    color() : r(255), g(255), b(255), a(255) {}
    color(int r_, int g_, int b_, int a_ = 255)
        : r(r_), g(g_), b(b_), a(a_) {}
};

#endif
