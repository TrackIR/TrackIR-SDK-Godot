//================================================
// Copyright 2026, NaturalPoint Inc. DBA OptiTrack
//================================================

#pragma once

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
//#include "TrackIR/NPClientWraps.h"


using namespace godot;

class TrackIRClient : public Node {
	GDCLASS(TrackIRClient, Node)

protected:
	static void _bind_methods();
	

public:
	TrackIRClient();
	~TrackIRClient();
};
