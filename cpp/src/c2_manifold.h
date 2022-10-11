#ifndef C2_MANIFOLD_H
    #define C2_MANIFOLD_H

    #include <Godot.hpp>
    #include <Reference.hpp>
    #include <Ref.hpp>
    #include <PoolArrays.hpp>
    #include <Vector2.hpp>
    #include <Array.hpp>

    #include "cute_c2.h"

    #include "c2_convert.h"

    namespace godot
    {
        class C2Manifold:public Reference
        {
            GODOT_CLASS(C2Manifold,Reference)

            c2Manifold _manifold;
        public:
            static void _register_methods();

            int get_count()const
            {
                return _manifold.count;
            }

            PoolRealArray get_depthes()const
            {
                return Array::make(_manifold.depths[0],_manifold.depths[1]);
            }

            PoolVector2Array get_contact_points()const
            {
                return Array::make(c2v_to_Vector2(_manifold.contact_points[0]),c2v_to_Vector2(_manifold.contact_points[1]));
            }

            Vector2 get_normal()const
            {
                return c2v_to_Vector2(_manifold.n);
            }
        };
    };

#endif
