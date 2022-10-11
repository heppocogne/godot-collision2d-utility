#include "c2_manifold.h"

using namespace godot;

void C2Manifold::_register_methods()
{
    register_method("get_count",&C2Manifold::get_count);
    register_method("get_depthes",&get_depthes);
    register_method("get_contact_points",&C2Manifold::get_contact_points);
    register_method("get_normal",&C2Manifold::get_normal);
}
