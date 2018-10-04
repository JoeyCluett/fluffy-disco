#ifndef __JJC__TWO__DIMENSIONAL__POINT__H__
#define __JJC__TWO__DIMENSIONAL__POINT__H__

struct TwoDimensionalPoint {
    float x, y;

    friend TwoDimensionalPoint operator+(TwoDimensionalPoint& tdp, TwoDimensionalPoint _tdp) {
        TwoDimensionalPoint tmp = tdp;
        tmp.x += _tdp.x;
        tmp.y += _tdp.y;
        return tmp;
    }

    friend TwoDimensionalPoint& operator+=(TwoDimensionalPoint& tdp, TwoDimensionalPoint _tdp) {
        tdp = tdp + _tdp;
        return tdp;
    }

    friend TwoDimensionalPoint operator-(TwoDimensionalPoint& tdp, TwoDimensionalPoint _tdp) {
        TwoDimensionalPoint tmp = tdp;
        tmp.x -= _tdp.x;
        tmp.y -= _tdp.y;
        return tmp;
    }

    friend TwoDimensionalPoint& operator-=(TwoDimensionalPoint& tdp, TwoDimensionalPoint _tdp) {
        tdp = tdp - _tdp;
        return tdp;
    }

};

typedef TwoDimensionalPoint _2DPt;

#endif // __JJC__TWO__DIMENSIONAL__POINT__H__