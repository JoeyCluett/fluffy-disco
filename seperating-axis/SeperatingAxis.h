#pragma once

#define real_t float

/*
    Seperating Axis Theorem
    - used for collision detection
*/

namespace SAT {

    // if the two lines intersect, the point of 
    // intersection will be stored in x and y
    bool linesIntersect(
            real_t slope_a, real_t intercept_a, 
            real_t slope_b, real_t intercept_b,
            float& x, float& y) {

        // if the slopes are equal, the lines are parallel
        // thus, they will never intersect
        if(slope_a == slope_b)
            return false;
    
        x = (intercept_b - intercept_a) / (slope_a - slope_b);
        y = slope_a * x + intercept_a;
        return true;
    }

} // end of namespace SAT