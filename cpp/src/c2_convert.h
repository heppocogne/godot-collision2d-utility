#ifndef C2_CONVERT_H
    #define C2_CONVERT_H

    #include <Vector2.hpp>
    #include <Dictionary.hpp>
    #include <Transform2D.hpp>

    #include "cute_c2.h"

    namespace godot
    {
        Vector2 c2v_to_Vector2(const c2v& v);
        Transform2D c2x_to_Transform2D(const c2x& tf);
        c2v Vector2_to_c2v(const Vector2& v);
        c2x Transform2D_to_c2x(const Transform2D& tf);
        Dictionary c2Manifold_to_Dictionary(const c2Manifold& m);
    };

#endif
