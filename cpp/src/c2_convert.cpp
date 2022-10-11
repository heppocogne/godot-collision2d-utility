#include "c2_convert.h"

#include <Array.hpp>
#include <PoolArrays.hpp>

#include <cmath>

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


    c2v Vector2_to_c2v(const Vector2& v)
    {
        return c2v{v.x,v.y};
    }


    c2x Transform2D_to_c2x(const Transform2D& tf)
    {
        const auto r=tf.get_rotation();
        return {Vector2_to_c2v(tf.get_origin()),c2r{cos(r),sin(r)}};
    }


    Dictionary c2Manifold_to_Dictionary(const c2Manifold& m)
    {
        Dictionary result;
        result["count"]=m.count;
        PoolRealArray depths;
        depths.append(m.depths[0]);
        depths.append(m.depths[1]);
        result["depths"]=depths;
        PoolVector2Array contact_points;
        contact_points.append(c2v_to_Vector2(m.contact_points[0]));
        contact_points.append(c2v_to_Vector2(m.contact_points[1]));
        result["contact_points"]=contact_points;
        result["normal"]=c2v_to_Vector2(m.n);

        return result;
    }
};
