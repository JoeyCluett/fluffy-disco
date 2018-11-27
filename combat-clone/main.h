#pragma once

#include <inc/TwoDimensionalPoint.h>
#include <inc/FloatRect.h>
#include <vector>

std::vector<_2DPt> M_sniper_round_targets;
_2DPt M_round_hit_loc;

_2DPt sniper_target(float dir, float x, float y, FloatRect* stuff_to_collide_with) {
    M_sniper_round_targets.clear();

    //float slope = (y - sin(dir)) / (x - cos(dir));
    float slope = sin(dir) / cos(dir);
    float far_x = x + (5.0f * cos(dir));
    float far_y = y + (5.0f * sin(dir));

    auto between = [](float in, float a, float b) -> bool {
        float max = a > b ? a : b;
        float min = a < b ? a : b;

        if(in >= min && in <= max)
            return true;
        return false;
    };

    // add potential hits to the sniper round queue
    auto hit_segment = [&](float x1, float x2, float y1, float y2) -> void {
        float m1 = (y2-y1) / (x2-x1);
        if(slope == m1)
            return; // the lines are parallel, do nothing
        float b1 = y1 - (m1 * x1);
    
        float& m2 = slope;
        float b2 = y - (m2 * x);

        float x_int = (b2 - b1) / (m1 - m2);
        float y_int = (b1*m2 - b2*m1) / (m2 - m1);

        if(between(x_int, x1, x2) 
                && between(y_int, y1, y2)
                && between(x_int, x, far_x)
                && between(y_int, y, far_y))
            M_sniper_round_targets.push_back({x_int, y_int});
    };

//    auto hit_vert = [&](float xi, float y1, float y2) -> void {
//        if(slope == )
//    }

    for(int i: {0, 1, 2, 3, 8, 9, 10, 11}) {
        FloatRect& fr = stuff_to_collide_with[i];
        // vertical collisions
        hit_segment(fr.x+0.0001f,      fr.x-0.0001f,      fr.y, fr.y + fr.h);
        hit_segment(fr.x+fr.w+0.0001f, fr.x+fr.w-0.0001f, fr.y, fr.y + fr.h);

        // horizontal collisions
        hit_segment(fr.x, fr.x+fr.w, fr.y+0.0001f, fr.y-0.0001f);
        hit_segment(fr.x, fr.x+fr.w, fr.y+fr.h+0.0001f, fr.y+fr.h-0.0001f);
    }

    if(M_sniper_round_targets.size() == 0)
        M_sniper_round_targets.push_back({far_x, far_y});

    M_round_hit_loc = M_sniper_round_targets[0];
    float current_smallest_distance = dist(x, y, M_round_hit_loc.x, M_round_hit_loc.y);

    for(auto& pt : M_sniper_round_targets) {
        float this_distance = dist(x, y, pt.x, pt.y);
        if(this_distance < current_smallest_distance) {
            M_round_hit_loc = pt;
            current_smallest_distance = this_distance;
        }
    }

    return M_round_hit_loc;
}
