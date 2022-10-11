extends Node2D

var c2_polygon:=C2Shape.new()
var c2_circle:=C2Shape.new()
var c2_cursor:=C2Shape.new()


func _ready():
	c2_polygon.set_points($Polygon/CollisionPolygon2D.polygon)
	c2_polygon.transform=$Polygon.transform
	c2_circle.set_shape($Circle/CollisionShape2D.shape)
	c2_circle.transform=$Circle.transform
	c2_cursor.set_shape($Cursor/CollisionShape2D.shape)

var test_manifold:=C2Manifold.new()


func _process(_delta:float):
	c2_cursor.transform=$Cursor.transform
	var polygon_collided:bool=c2_polygon.is_collided(c2_cursor)
	$VBoxContainer/CollidePolygon.text="polygon:"+str(polygon_collided)
	var polygon_manifold:Dictionary=c2_polygon.is_collided_manifold(c2_cursor)
	if polygon_manifold:
		$VBoxContainer/PolygonManifold.text="   count:%d\n   depthes:[%f, %f]\n   contact_points:(%f, %f), (%f,%f)\nnormal:(%f, %f)"%[
				polygon_manifold["count"],
				polygon_manifold["depths"][0],
				polygon_manifold["depths"][1],
				polygon_manifold["contact_points"][0].x,
				polygon_manifold["contact_points"][0].y,
				polygon_manifold["contact_points"][1].x,
				polygon_manifold["contact_points"][1].y,
				polygon_manifold["normal"].x,
				polygon_manifold["normal"].y,
			]
	else:
		$VBoxContainer/PolygonManifold.text=""
	$VBoxContainer/PolygonContains.text="polygon_contains_point:"+str(c2_polygon.contains_point(get_local_mouse_position(),true))
	
	var circle_collided:bool=c2_circle.is_collided(c2_cursor)
	$VBoxContainer/CollideCircle.text="circle:"+str(circle_collided)
	var circle_manifold:Dictionary=c2_circle.is_collided_manifold(c2_cursor)
	if circle_manifold:
		$VBoxContainer/CircleManifold.text="   count:%d\n   depthes:[%f, %f]\n   contact_points:(%f, %f), (%f,%f)\nnormal:(%f, %f)"%[
				circle_manifold["count"],
				circle_manifold["depths"][0],
				circle_manifold["depths"][1],
				circle_manifold["contact_points"][0].x,
				circle_manifold["contact_points"][0].y,
				circle_manifold["contact_points"][1].x,
				circle_manifold["contact_points"][1].y,
				circle_manifold["normal"].x,
				circle_manifold["normal"].y,
			]
	else:
		$VBoxContainer/CircleManifold.text=""
	$VBoxContainer/CircleContains.text="circle_contains_point:"+str(c2_circle.contains_point(get_local_mouse_position(),true))
