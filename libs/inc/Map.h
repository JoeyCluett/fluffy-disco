#ifndef __JJC__MAP__H__
#define __JJC__MAP__H__

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

#endif // __JJC__MAP__H__