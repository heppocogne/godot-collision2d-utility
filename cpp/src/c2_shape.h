#ifndef C2_SHAPE_H
    #define C2_SHAPE_H

    #include <Godot.hpp>
    #include <Reference.hpp>
    #include <Ref.hpp>

    namespace godot
    {
        class C2Shape:public Reference
        {
            GODOT_CLASS(C2Shape,Reference)
        public:
            C2Shape();
            ~C2Shape();

            bool colide(Ref<C2Shape> other)const;
        };
    };

#endif
