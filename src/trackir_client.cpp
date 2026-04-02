//================================================
// Copyright 2026, NaturalPoint Inc. DBA OptiTrack
//================================================

#include <Windows.h>
#include <strsafe.h>
#include "TrackIR/NPClientWraps.h"
#include "trackir_client.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

// The developer ID provided to you by NaturalPoint.
// The default is the SDK ID, which is available to everyone.
#define NP_DEVELOPER_ID 1000

/// Registry location for locating NPClient.dll
std::wstring dLLRegKeyLoc = L"Software\\NaturalPoint\\NATURALPOINT\\NPClient Location\\";

// Helper function declaration
std::wstring FindDllLocation(std::wstring regKeyLoc);


// This function registers the class's methods with the Godot engine so that 
// they can be accessed in GDScript
void TrackIRClient::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_trackIR_data"), &TrackIRClient::get_trackIR_data);
	ClassDB::bind_method(D_METHOD("is_connected"), &TrackIRClient::is_connected);
}


// Constructor
TrackIRClient::TrackIRClient() {
	// Set bits for Pitch, Yaw, Roll, X, Y, Z data fields
	data_fields = 0;
	data_fields |= NPPitch;
	data_fields |= NPYaw;
	data_fields |= NPRoll;
	data_fields |= NPX;
	data_fields |= NPY;
	data_fields |= NPZ;
}


// Destructor
TrackIRClient::~TrackIRClient() {
	// Unregister window handle
	NPRESULT result = NPClient::NP_UnregisterWindowHandle();
	if (result == NP_OK) {
		print_line("unregistered window handle successfully");
	}
	else {
		print_line("Error: NP_UnregisterWindowHandle");
	}
}


// Initialize a connection to TrackIR software
void TrackIRClient::_enter_tree() {
	print_line("TrackIR plugin initializing...");
	connected = false;

	// Search registry for DLL location
	std::wstring pathToDll = FindDllLocation( dLLRegKeyLoc ) + L"\\";
	if (pathToDll.find( L"Error" ) != -1) {
		print_line("Error: Cannot find DLL location");
		return;
	}
	
	// Initialize functions from DLL
	NPRESULT result = NPClient::NPClient_Init(pathToDll);
	if (result != NP_OK) {
		print_line("Error: Could not initialize NPClient interface. Make sure TrackIR is running!");
		return;
	}

	// Get the application's window handle
	HWND handle = GetActiveWindow();
	if (handle == nullptr) {
		print_line("Error: Could not retrieve window handle.");
		return;
	}

	// Register window handle to communicate between TrackIR and application
	result = NPClient::NP_RegisterWindowHandle(handle);
	if (result != NP_OK) {
		print_line("Error: Registering window handle failed.");
		return;
	}

	// Query the NaturalPoint software version
	unsigned short wNPClientVer;
	result = NPClient::NP_QueryVersion( &wNPClientVer );
	if (result == NP_OK) {
		char csMajorVer[250], csMinorVer[250], csVerMsg[250];
		// right shift by 1 byte to get high byte (major version)
		sprintf_s(csMajorVer, "%d", ( wNPClientVer >> 8 ) ); 
		// mask remaining to get the lower byte (minor version)
		sprintf_s(csMinorVer, "%02d", ( wNPClientVer & 0x00FF ) ); 
		sprintf_s(csVerMsg, "TrackIR software version is %s.%s",
			csMajorVer, csMinorVer );
		print_line(String(csVerMsg));
	}
	else {
		print_line("Error: querying TrackIR software version failed");
		return;
	}

	// Register program developer ID provided by NaturalPoint
	result = NPClient::NP_RegisterProgramProfileID( NP_DEVELOPER_ID );
	if (result != NP_OK) {
		print_line("Error: Could not register Developer ID.");
		return;
	}

	// Tell TrackIR what data fields we want
	NPClient::NP_RequestData(data_fields);

	// Tell TrackIR we are ready to receive data
	result = NPClient::NP_StartDataTransmission();
	if (result == NP_OK) {
		print_line("Initialization complete. Data transmission started.");
	}
	else {
		print_line("Error: starting data transmission failed");
		return;
	}

	// initialization succeeded
	connected = true;
}


// Queries TrackIR for the lastest position and rotation data
// If there is a new frame, returns a Godot::Dictionary with the key "status"
// set to "NEW_DATA" and the keys "position", "pitch", "yaw", and "roll" set
// to the corresponding data
// If no new data was available, returns a dictionary with the key "status" set
// to "NO_DATA"
Dictionary TrackIRClient::get_trackIR_data() {
	Dictionary data;

	TRACKIRDATA tid;
	memset( &tid, 0, sizeof( TRACKIRDATA ) );

	// Go get the latest data
	NPRESULT result = NPClient::NP_GetData( &tid );

	if (NP_OK == result)
	{
		// Got data to process ...

		// compare the last frame signature to the current one
		// if they are not the same then new data has arrived since then
		char buf[250];
		static int prevLength = 0;

		// check if we are not using mouse emulation mode.
		// TrackIR ships with mouse emulation software to 
		// control the mouse from head position. If not checked for
		// it is very likely this will cause weird things to happen in game.
		if (tid.Status == NPSTATUS_REMOTEACTIVE) {
			if (frame_signature != tid.FrameSignature) {
				data.set("status", "NEW_DATA");

				// convert translation values from TIR units to cm
				// max value constants are defined in NPClient.h
				double x = ( tid.X / NP_MAX_VALUE ) * NP_MAX_TRANSLATION;
				double y = ( tid.Y / NP_MAX_VALUE ) * NP_MAX_TRANSLATION;
				double z = ( tid.Z / NP_MAX_VALUE ) * NP_MAX_TRANSLATION;

				// convert rotation values from TIR units to degrees
				double yaw = ( tid.Yaw / NP_MAX_VALUE ) * NP_MAX_ROTATION;
				double pitch = ( tid.Pitch / NP_MAX_VALUE ) * NP_MAX_ROTATION;
				double roll = ( tid.Roll / NP_MAX_VALUE ) * NP_MAX_ROTATION;
			
				// set values in return dictionary
				Vector3 trackIR_position = Vector3(x, y, z);
				data.set("position", trackIR_position);
				data.set("pitch", pitch);
				data.set("yaw", yaw);
				data.set("roll", roll);
				
				// keep track of latest frame signature
				frame_signature = tid.FrameSignature;
			}
			else {
				data.set("status", "NO_DATA");
			}
		}
		else {
			data.set("status", "NO_DATA");
		}
	}
	return data;
}


// Returns whether the initialization process succeeded
bool TrackIRClient::is_connected(){
	return connected;
}


// Looks up the location of the NPClient DLL in the windows registry
std::wstring FindDllLocation(std::wstring regKeyLoc)
{
	HKEY pKey = NULL;

	//open the registry key 
	LONG lresult = RegOpenKeyEx( HKEY_CURRENT_USER, (LPCTSTR) regKeyLoc.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &pKey);
	if (lresult != ERROR_SUCCESS) {
		//error condition
		return L"Error: Unable to open DLL location registry key";
	}

	//get the value from the key
	wchar_t szValue[250];
	DWORD dwSize;

	//first discover the size of the value
	if (RegQueryValueEx( pKey, (LPCTSTR) L"Path", NULL, NULL, NULL, &dwSize ) == ERROR_SUCCESS) {
		//now get the value
		if (RegQueryValueEx( pKey, (LPCTSTR) L"Path", NULL, NULL, reinterpret_cast<LPBYTE>( szValue ), &dwSize ) == ERROR_SUCCESS) {
			//everything worked
			RegCloseKey( pKey );
			std::wstring lValue(szValue);
			lValue.pop_back();
			return lValue;
		}
		else {
			return L"Error reading location key!";
		}
	}
	RegCloseKey( pKey );
	return L"Error";
}
