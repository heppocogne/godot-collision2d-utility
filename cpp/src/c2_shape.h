#ifndef C2_SHAPE_H
    #define C2_SHAPE_H

    #include <Godot.hpp>
    #include <Reference.hpp>
    #include <Ref.hpp>
    #include <Transform2D.hpp>
    #include <Shape2D.hpp>
    #include <String.hpp>

    #include "c2_manifold.h"

    namespace godot
    {
        class C2Shape:public Reference
        {
            GODOT_CLASS(C2Shape,Reference)


            bool _tf_dirty;
            bool _shape_dirty;

            void _update_tf();
            void _update_shape();
        public:
            Transform2D transform;
            Ref<Shape2D> shape;

            c2x tf_c2x;
            C2_TYPE c2_shape_type;
            union
            {
                c2Circle circle;
                c2AABB aabb;
                c2Capsule capsule;
                c2Poly poly;
                c2Ray ray;
            };

            static void _register_methods();

            C2Shape();

            void _init();

            bool is_collided(Ref<C2Shape> other);
            bool contains_point(Vector2 point,bool include_border=false);
            Ref<C2Manifold> is_collided_manifold(Ref<C2Shape> other);
            void set_transform(Transform2D p_transform2d);
            Transform2D get_transform()const{return transform;}
            void set_shape(Ref<Shape2D> p_shape);
            Ref<Shape2D> get_shape()const{return shape;}
            void set_points(PoolVector2Array points);

            void prepare();
        };
    };

#endif
