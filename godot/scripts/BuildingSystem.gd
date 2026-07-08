extends Node3D

@export var build_range = 8.0
@export var grid_size = 1.0

var preview_mesh : MeshInstance3D
var needs_system : Node
var current_block_color = Color(0.8, 0.6, 0.4)  # Default wood-like color

func _ready():
	preview_mesh = get_node("../BuildPreview")
	needs_system = get_node("../NeedsSystem")

func _process(delta):
	update_build_preview()

	if Input.is_action_just_pressed("build"):
		try_place_block()
	if Input.is_action_just_pressed("remove"):
		try_remove_block()

	# Change block color with number keys
	if Input.is_action_just_pressed("ui_1"): current_block_color = Color(0.8, 0.6, 0.4)
	if Input.is_action_just_pressed("ui_2"): current_block_color = Color(0.6, 0.8, 0.6)
	if Input.is_action_just_pressed("ui_3"): current_block_color = Color(0.6, 0.7, 0.9)
	if Input.is_action_just_pressed("ui_4"): current_block_color = Color(0.9, 0.7, 0.5)

func update_build_preview():
	var player = get_parent().get_node("GribbitPlayer")
	if not player: return

	var camera = player.get_node("Camera3D")
	var space_state = get_world_3d().direct_space_state

	var from = camera.global_position
	var to = from + camera.global_transform.basis.z * -build_range

	var query = PhysicsRayQueryParameters3D.create(from, to)
	var result = space_state.intersect_ray(query)

	if result:
		var pos = result.position
		pos = Vector3(
			floor(pos.x / grid_size) * grid_size + grid_size * 0.5,
			floor(pos.y / grid_size) * grid_size + grid_size * 0.5,
			floor(pos.z / grid_size) * grid_size + grid_size * 0.5
		)
		preview_mesh.global_position = pos
		preview_mesh.visible = true
	else:
		preview_mesh.visible = false

func try_place_block():
	if preview_mesh.visible and needs_system:
		var new_block = MeshInstance3D.new()
		new_block.mesh = BoxMesh.new()
		new_block.scale = Vector3(0.95, 0.95, 0.95)
		new_block.global_position = preview_mesh.global_position
		
		# Apply color
		var material = StandardMaterial3D.new()
		material.albedo_color = current_block_color
		new_block.material_override = material
		
		add_child(new_block)

		needs_system.modify_need("energy", -5)

func try_remove_block():
	var player = get_parent().get_node("GribbitPlayer")
	if not player: return

	var camera = player.get_node("Camera3D")
	var space_state = get_world_3d().direct_space_state
	var from = camera.global_position
	var to = from + camera.global_transform.basis.z * -build_range

	var query = PhysicsRayQueryParameters3D.create(from, to)
	var result = space_state.intersect_ray(query)

	if result and result.collider is MeshInstance3D and result.collider != get_parent().get_node("Floor").get_node("MeshInstance3D"):
		result.collider.queue_free()