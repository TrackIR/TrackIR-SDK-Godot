@tool
extends EditorPlugin

const AUTOLOAD_NAME = "TrackIR"
const PLUGIN_FOLDER = "trackir_plugin"


func _enable_plugin() -> void:
	# Add autoloads here.
	add_autoload_singleton(AUTOLOAD_NAME, "trackir.gd")
	
	# Enable sub-plugins


func _disable_plugin() -> void:
	# Disable sub-plugins
	
	# Remove autoloads here.
	remove_autoload_singleton(AUTOLOAD_NAME)


func _enter_tree() -> void:
	# Initialization of the plugin goes here.
	pass


func _exit_tree() -> void:
	# Clean-up of the plugin goes here.
	pass
