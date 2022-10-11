#ifndef C2_CONVERT_H
    #define C2_CONVERT_H

    #include <Vector2.hpp>
    #include <Transform2D.hpp>
    
    #include "cute_c2.h"

    namespace godot
    {
        Vector2 c2v_to_Vector2(const c2v& v);
        Transform2D c2x_to_Transform2D(const c2x& tf);
    };

#endif
