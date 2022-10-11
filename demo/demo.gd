extends Node2D

var c2_polygon:C2Shape=C2Shape.new()
var c2_circle:C2Shape=C2Shape.new()
var c2_cursor:C2Shape=C2Shape.new()


func _ready():
	c2_polygon.set_points($Polygon/CollisionPolygon2D.polygon)
	c2_polygon.transform=$Polygon.transform
	c2_circle.set_shape($Circle/CollisionShape2D.shape)
	c2_circle.transform=$Circle.transform
	c2_cursor.set_shape($Cursor/CollisionShape2D.shape)


func _process(_delta:float):
	c2_cursor.transform=$Cursor.transform
	$VBoxContainer/CollidePolygon.text="polygon:"+str(c2_polygon.is_collided(c2_cursor))
	$VBoxContainer/PolygonContains.text="polygon_contains_point:"+str(c2_polygon.contains_point(get_local_mouse_position(),true))
	$VBoxContainer/CollideCircle.text="circle:"+str(c2_circle.is_collided(c2_cursor))
	$VBoxContainer/CircleContains.text="circle_contains_point:"+str(c2_circle.contains_point(get_local_mouse_position(),true))
