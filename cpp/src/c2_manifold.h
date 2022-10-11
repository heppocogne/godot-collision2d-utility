#ifndef C2_MANIFOLD_H
    #define C2_MANIFOLD_H

    #include <Godot.hpp>
    #include <Reference.hpp>
    #include <Ref.hpp>
    #include "cute_c2.h"

    namespace godot
    {
        class C2Manifold:public Reference
        {
            GODOT_CLASS(C2Manifold,Reference)
        public:
            c2Manifold manifold;

            C2Manifold();
            ~C2Manifold();
        };
    };

#endif
