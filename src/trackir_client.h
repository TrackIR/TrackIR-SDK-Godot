//================================================
// Copyright 2026, NaturalPoint Inc. DBA OptiTrack
//================================================

#pragma once

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;


class TrackIRClient : public Node {
	GDCLASS(TrackIRClient, Node)

protected:
	bool connected;
	unsigned int data_fields;
	unsigned short frame_signature;

	static void _bind_methods();

public:
	TrackIRClient();
	~TrackIRClient();
	void _enter_tree() override;
	Dictionary get_trackIR_data();
	bool is_connected();
};
