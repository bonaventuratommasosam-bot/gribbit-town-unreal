extends Node

@export var hunger = 80.0
@export var energy = 80.0
@export var fun = 70.0
@export var social = 60.0

@onready var hunger_label = $"../UI/NeedsPanel/HungerLabel"
@onready var energy_label = $"../UI/NeedsPanel/EnergyLabel"

func _process(delta):
	# Slow decay
	hunger = clamp(hunger - 0.5 * delta, 0, 100)
	energy = clamp(energy - 0.3 * delta, 0, 100)
	fun = clamp(fun - 0.4 * delta, 0, 100)
	social = clamp(social - 0.25 * delta, 0, 100)

	update_ui()

func update_ui():
	if hunger_label:
		hunger_label.text = "Fame: %d" % hunger
	if energy_label:
		energy_label.text = "Energia: %d" % energy

func modify_need(need_name: String, amount: float):
	match need_name:
		"hunger": hunger = clamp(hunger + amount, 0, 100)
		"energy": energy = clamp(energy + amount, 0, 100)
		"fun": fun = clamp(fun + amount, 0, 100)
		"social": social = clamp(social + amount, 0, 100)