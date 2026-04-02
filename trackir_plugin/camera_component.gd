extends Node3D

## Maximum offset applied to the camera's position in each direction (x, y, z).
## Default represents a maximum offset of 50cm.
@export_custom(PROPERTY_HINT_NONE, "suffix:m") var max_offset : float = 0.5
## The number of seconds without new data from TrackIR before tracking is
## considered lost and the camera recenters itself.
@export_custom(PROPERTY_HINT_NONE, "suffix:sec") var tracking_lost_timeout : float = 3.0
## The amount of time in seconds the camera takes to recenter itself.
@export_custom(PROPERTY_HINT_NONE, "suffix:sec") var recenter_time : float = 1.0

var trackIR_connected : bool
var stale_data_duration : float = 0
var data : Dictionary
var pose : Transform3D 


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	trackIR_connected = TrackIR.is_connected()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	# check that TrackIRClient has initialized correctly
	if trackIR_connected == false:
		return
	
	# get data from TrackIRClient
	data = TrackIR.get_trackIR_data()
	
	if data["status"] == "NEW_DATA":
		# reset time since new data
		stale_data_duration = 0
		
		# scale position to be between -max_offset and +max_offset
		# data from trackIR is between -50.0 and +50.0
		pose.origin = data["position"] * max_offset / 50.0
		# change coordinates to match Godot's conventions
		pose.origin.x *= -1
		
		# get rotation angles from data, convert to Godot's conventions
		var pitch = deg_to_rad(-1 * data["pitch"])
		var yaw = deg_to_rad(data["yaw"])
		var roll = deg_to_rad(data["roll"])
		var rotation_angles = Vector3(pitch, yaw, roll)
		
		# apply rotations in roll-pitch-yaw order (Z-Y-X)
		pose.basis = Basis.from_euler(rotation_angles, EULER_ORDER_ZYX)
		
		# apply transform
		transform = pose
	else:
		# no data - increment time since fresh data was received
		stale_data_duration += delta
		
		if stale_data_duration > tracking_lost_timeout:
			# interpolate between last transform and identity transform
			var recenter_fraction = min((stale_data_duration - tracking_lost_timeout) / recenter_time, 1.0)
			transform = pose.interpolate_with(Transform3D.IDENTITY, recenter_fraction)
