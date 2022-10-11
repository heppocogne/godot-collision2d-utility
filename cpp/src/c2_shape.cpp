#include "c2_shape.h"
#include <CircleShape2D.hpp>
#include <ConvexPolygonShape2D.hpp>
#include <RectangleShape2D.hpp>
#include <Plane.hpp>
#include <Vector3.hpp>

#include "c2_convert.h"

using namespace godot;

template<typename number_type>
int signof(const number_type value)
{
    if(value==0)
        return 0;
    else if(value<0)
        return -1;
    else
        return 1;
}

void C2Shape::_register_methods()
{
    register_property<C2Shape,Transform2D>("transform",&C2Shape::set_transform,&C2Shape::get_transform,Transform2D::IDENTITY);
    register_method("set_transform",&C2Shape::set_transform);
    register_method("get_transform",&C2Shape::get_transform);
    register_property<C2Shape,Ref<Shape2D>>("shape",&C2Shape::set_shape,&C2Shape::get_shape,nullptr);
    register_method("set_shape",&C2Shape::set_shape);
    register_method("get_shape",&C2Shape::get_shape);
    register_method("set_points",&C2Shape::set_points);
    register_method("is_collided",&C2Shape::is_collided);
    register_method("contains_point",&C2Shape::contains_point);
    register_method("is_collided_manifold",&C2Shape::is_collided_manifold);
}


C2Shape::C2Shape():
    _tf_dirty(true),
    _shape_dirty(true)
{

}


void C2Shape::_init()
{
    transform=Transform2D::IDENTITY;
    shape=nullptr;
}


void C2Shape::set_transform(Transform2D p_transform2d)
{
    const auto scale_old=transform.get_scale();
    transform=p_transform2d;
    _tf_dirty=true;
    _shape_dirty=true;
}


void C2Shape::set_shape(Ref<Shape2D> p_shape)
{
    shape=p_shape;
    _shape_dirty=true;
}


void C2Shape::set_points(PoolVector2Array points)
{
    Ref<ConvexPolygonShape2D> poly_shape=ConvexPolygonShape2D::_new();
    poly_shape->set_point_cloud(points);
    shape=poly_shape;
    
    _shape_dirty=true;
}


void C2Shape::_update_tf()
{
    if(_tf_dirty)
    {
        Transform2D no_scaled=Transform2D::IDENTITY;
        no_scaled.set_origin(transform.get_origin());
        no_scaled.set_rotation(transform.get_rotation());
        tf_c2x=Transform2D_to_c2x(no_scaled);

        _tf_dirty=false;
    }
}


const char* shape2d_classes[]=
{
    "CapsuleShape2D",
    "CircleShape2D",
    "ConcavePolygonShape2D",
    "ConvexPolygonShape2D",
    "LineShape2D",
    "RayShape2D",
    "RectangleShape2D",
    "SegmentShape2D"
};


void C2Shape::_update_shape()
{
    if(_shape_dirty)
    {
        const auto scale=transform.get_scale();
        String godot_shape_type="";
        for(int i=0;i<sizeof(shape2d_classes);i++)
        {
            if(shape->is_class(shape2d_classes[i]))
            {
                godot_shape_type=shape2d_classes[i];
                break;
            }
        }

        if(godot_shape_type=="CircleShape2D")
        {
            Ref<CircleShape2D> circle_shape=shape;
            c2_shape_type=C2_TYPE_CIRCLE;
            circle.p=Vector2_to_c2v(transform.get_origin());
            if(scale.x!=scale.y)
                Godot::print_warning("scale.x and scale.y should be same (scale.y is discarded)",__func__,__FILE__,__LINE__);
            circle.r=scale.x*circle_shape->get_radius();
        }else if(godot_shape_type=="ConvexPolygonShape2D")
        {
            Ref<ConvexPolygonShape2D> poly_shape=shape;
            PoolVector2Array& points=poly_shape->get_points();
            if(points.size()>C2_MAX_POLYGON_VERTS)
            {
                Godot::print_error(String("The number of vertices is limited to ")+Variant(C2_MAX_POLYGON_VERTS),__func__,__FILE__,__LINE__);
                c2_shape_type=C2_TYPE_NONE;
                return;
            }
            c2_shape_type=C2_TYPE_POLY;

            poly.count=points.size();
            for(int i=0;i<poly.count;i++)
            {
                poly.verts[i]=Vector2_to_c2v(scale*points[i]);
            }
            c2MakePoly(&poly);
        }else if(godot_shape_type=="RectangleShape2D")
        {
            Ref<RectangleShape2D> rect_shape=shape;
            c2_shape_type=C2_TYPE_POLY;
            poly.count=4;
            Vector2 ext_scaled=scale*rect_shape->get_extents();
            poly.verts[0]=Vector2_to_c2v(-ext_scaled);
            poly.verts[1]=c2v{ext_scaled.x,-ext_scaled.y};
            poly.verts[2]=Vector2_to_c2v(ext_scaled);
            poly.verts[3]=c2v{-ext_scaled.x,ext_scaled.y};
            c2MakePoly(&poly);
        }else
        {
            Godot::print_error(shape->get_class()+String(" is not supported"),__func__,__FILE__,__LINE__);
            c2_shape_type=C2_TYPE_NONE;
            return;
        }

        _shape_dirty=false;
    }
    return;
}


void C2Shape::prepare()
{
    _update_tf();
    _update_shape();
}


bool C2Shape::is_collided(Ref<C2Shape> other)
{
    prepare();
    other->prepare();

    switch(c2_shape_type)
    {
        case C2_TYPE_POLY:
            switch (other->c2_shape_type)
            {
            case C2_TYPE_POLY:
                return c2PolytoPoly(&poly,&tf_c2x,&other->poly,&other->tf_c2x);
            case C2_TYPE_CIRCLE:
                return c2CircletoPoly(other->circle,&poly,&tf_c2x);
            default:
                return false;
            }
            break;
        case C2_TYPE_CIRCLE:
            switch (other->c2_shape_type)
            {
            case C2_TYPE_POLY:
                return c2CircletoPoly(circle,&other->poly,&other->tf_c2x);
            case C2_TYPE_CIRCLE:
                return c2CircletoCircle(circle,other->circle);
            default:
                return false;
            }
        default:
            return false;
    }
}


bool C2Shape::contains_point(Vector2 point,bool include_border)
{
    prepare();
    switch(c2_shape_type)
    {
        case C2_TYPE_POLY:
        {
            const Vector3 point_v3(point.x,point.y,0.0);
            //[count(-1),count(0),count(1)]
            int signs_count[3]={0,0,0};
            for(int i=0;i<poly.count;i++)
            {
                const Vector2 v=transform.xform(c2v_to_Vector2(poly.verts[i]));
                const Vector2 n=Vector2(poly.norms[i].x,poly.norms[i].y).rotated(transform.get_rotation());
                Plane p(Vector3(v.x,v.y,0.0),Vector3(n.x,n.y,0.0));
                signs_count[signof(p.distance_to(point_v3))+1]+=1;
            }
            if(include_border)
                //count(-1)+count(0)==poly.count or count(0)+count(1)==poly.count
                return (signs_count[0]+signs_count[1]==poly.count) || (signs_count[1]+signs_count[2]==poly.count);
            else
                //count(0)==0 and (count(-1)==poly.count or count(1)==poly.count)
                return signs_count[1]==0 && (signs_count[0]==poly.count || signs_count[2]==poly.count);
        }
        case C2_TYPE_CIRCLE:
            if(include_border)
                return point.distance_to(c2v_to_Vector2(circle.p))<=circle.r;
            else
                return point.distance_to(c2v_to_Vector2(circle.p))<circle.r;
        default:
            return false;
    }
}


Ref<C2Manifold> C2Shape::is_collided_manifold(Ref<C2Shape> other)
{
    prepare();
    other->prepare();
    Ref<C2Manifold> m=C2Manifold::_new();

    switch(c2_shape_type)
    {
        case C2_TYPE_POLY:
            switch (other->c2_shape_type)
            {
            case C2_TYPE_POLY:
                c2PolytoPolyManifold(&poly,&tf_c2x,&other->poly,&other->tf_c2x,&m->manifold);
                return m;
            case C2_TYPE_CIRCLE:
                c2CircletoPolyManifold(other->circle,&poly,&tf_c2x,&m->manifold);
                return m;
            default:
                return nullptr;
            }
            break;
        case C2_TYPE_CIRCLE:
            switch (other->c2_shape_type)
            {
            case C2_TYPE_POLY:
                c2CircletoPolyManifold(circle,&other->poly,&other->tf_c2x,&m->manifold);
                return m;
            case C2_TYPE_CIRCLE:
                c2CircletoCircleManifold(circle,other->circle,&m->manifold);
                return m;
            default:
                return nullptr;
            }
        default:
            return nullptr;
    }
}
