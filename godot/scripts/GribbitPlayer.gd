extends CharacterBody3D

@export var speed = 6.0
@export var jump_velocity = 5.0
@export var mouse_sensitivity = 0.002

var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")
var camera : Camera3D
var yaw = 0.0
var pitch = 0.0

var needs_system : Node

func _ready():
	camera = $Camera3D
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	needs_system = get_node("../NeedsSystem")

func _input(event):
	if event is InputEventMouseMotion:
		yaw -= event.relative.x * mouse_sensitivity
		pitch -= event.relative.y * mouse_sensitivity
		pitch = clamp(pitch, -1.5, 1.5)
		camera.rotation.y = yaw
		camera.rotation.x = pitch

	# Interact with E
	if event is InputEventKey and event.pressed and event.keycode == KEY_E:
		try_interact()

func try_interact():
	var space_state = get_world_3d().direct_space_state
	var from = camera.global_position
	var to = from + camera.global_transform.basis.z * -6.0

	var query = PhysicsRayQueryParameters3D.create(from, to)
	var result = space_state.intersect_ray(query)

	if result and needs_system:
		needs_system.modify_need("hunger", 12)
		print("Interacted! Hunger restored.")

func _physics_process(delta):
	if not is_on_floor():
		velocity.y -= gravity * delta

	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity.y = jump_velocity

	var input_dir = Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	var direction = (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()

	var forward = -camera.global_transform.basis.z
	forward.y = 0
	forward = forward.normalized()
	
	var right = camera.global_transform.basis.x
	right.y = 0
	right = right.normalized()
	
	var move_dir = forward * input_dir.y + right * input_dir.x
	
	if move_dir.length() > 0:
		velocity.x = move_dir.x * speed
		velocity.z = move_dir.z * speed
	else:
		velocity.x = move_toward(velocity.x, 0, speed)
		velocity.z = move_toward(velocity.z, 0, speed)

	move_and_slide()