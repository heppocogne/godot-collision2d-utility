tool
extends EditorPlugin


func _enter_tree():
	add_custom_type("C2Shape","Reference",preload("res://addons/collision2d_utility/c2_shape.gdns"),null)


func _exit_tree():
	remove_custom_type("C2Shape")
