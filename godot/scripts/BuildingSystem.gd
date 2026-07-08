extends Node3D

@export var build_range = 8.0
@export var grid_size = 1.0

var preview_mesh : MeshInstance3D

var is_building = false

func _ready():
	preview_mesh = get_node("../BuildPreview")

func _process(delta):
	update_build_preview()

func update_build_preview():
	var player = get_parent().get_node("GribbitPlayer")
	if not player:
		return
	
	var camera = player.get_node("Camera3D")
	var space_state = get_world_3d().direct_space_state
	
	var from = camera.global_position
	var to = from + camera.global_transform.basis.z * -build_range
	
	var query = PhysicsRayQueryParameters3D.create(from, to)
	var result = space_state.intersect_ray(query)
	
	if result:
		var pos = result.position
		# Snap to grid
		pos = Vector3(
			floor(pos.x / grid_size) * grid_size + grid_size/2,
			floor(pos.y / grid_size) * grid_size + grid_size/2,
			floor(pos.z / grid_size) * grid_size + grid_size/2
		)
		preview_mesh.global_position = pos
		preview_mesh.visible = true
	else:
		preview_mesh.visible = false

func try_place_block():
	if preview_mesh.visible:
		var new_block = MeshInstance3D.new()
		new_block.mesh = BoxMesh.new()
		new_block.scale = Vector3(0.95, 0.95, 0.95)
		new_block.global_position = preview_mesh.global_position
		add_child(new_block)

func try_remove_block():
	var player = get_parent().get_node("GribbitPlayer")
	if not player: return
	
	var camera = player.get_node("Camera3D")
	var space_state = get_world_3d().direct_space_state
	var from = camera.global_position
	var to = from + camera.global_transform.basis.z * -build_range
	
	var query = PhysicsRayQueryParameters3D.create(from, to)
	var result = space_state.intersect_ray(query)
	
	if result and result.collider is MeshInstance3D:
		result.collider.queue_free()