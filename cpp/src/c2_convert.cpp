#include "c2_convert.h"

namespace godot
{
    Vector2 c2v_to_Vector2(const c2v& v)
    {
        return Vector2(v.x,v.y);
    }


    Transform2D c2x_to_Transform2D(const c2x& tf)
    {
        Transform2D reuslt=Transform2D::IDENTITY;
        reuslt.set_origin(c2v_to_Vector2(tf.p));
        reuslt.set_rotation(Vector2(tf.r.c,tf.r.s).angle());

        return reuslt;
    }
};
