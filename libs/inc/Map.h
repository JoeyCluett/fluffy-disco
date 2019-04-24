#ifndef __JJC__MAP__H__
#define __JJC__MAP__H__

#include <cmath>

// default performs mapping function 
// on single-precision floats
template<typename T = float>
T map(T input, T input_start, T input_end, T output_start, T output_end) {
    T output = 
        output_start + 
            ((output_end - output_start) / 
            (input_end - input_start)) 
        * (input - input_start);
    
    return output;
}

template<typename T = float>
T dist(T x1, T y1, T x2, T y2) {
    T deltaX = x2-x1;
    T deltaY = y2-y1;
    return std::sqrt(deltaX*deltaX + deltaY*deltaY);
}

template<typename T = float>
T dist_2(T x1, T y1, T x2, T y2) {
    T deltax = x2-x1;
    T deltaY = y2-y1;
    return std::sqrt(deltax*deltax + deltaY*deltaY);
}

#endif // __JJC__MAP__H__