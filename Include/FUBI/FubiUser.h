// ****************************************************************************************
//
// Fubi Tracker - User class
// ---------------------------------------------------------
// Copyright (C) 2010-2012 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************
#pragma once

#include "FubiPostures.h"

#include "FubiUtils.h"

#include <map>
#include <deque>

class PostureCombinationRecognizer;

// The FubiUser class hold all relevant informations for each tracked user
class FubiUser
{
public:
	FubiUser(unsigned int userIndex);
	~FubiUser();

	// Enables/disables a posture combination recognizer of this user
	void enablePostureCombinationDetection(Fubi::PostureCombinations postureID, bool enable);
	void enableUserDefinedPostureCombinationDetection(const std::string& postureName, bool enable);

	// Enable/disables the tracking of the shown number of fingers for each hand
	void enableFingerTracking(bool leftHand, bool rightHand, bool useConvexityDefectMethod = false);

	// Gets the finger count calculated by the median of the last 10 calculations
	int getFingerCount(bool leftHand = false);

	// Stops and removes all user defined 
	void clearUserDefinedPostureCombinationRecognizers();

	// Swaps the data of two users
	void swap(FubiUser* other);

	// Update the tracking info from the openNI user generator
	void updateTrackingInfo();

	// Manually update the tracking info
	void addNewTrackingInfo(XnSkeletonJointPosition* positions, XnVector3D com,
		double timeStamp = -1, XnSkeletonJointOrientation* orientations = 0, XnSkeletonJointOrientation* localOrientations = 0);

	// Whether the user is currently seen in the depth image
	bool m_inScene;

	// Names of the users, their calibration files are named accordingly "%name%Calibration.bin"
	std::string m_name;

	// OpenNI id of this user
	XnUserID m_id;

	// Whether the user is currently tracked
	bool m_isTracked;
	
	// Current skeleton joints (position/orientation) and the com per user with a timestamp
	struct UserTrackingInfo
	{
		std::map<XnSkeletonJoint, XnSkeletonJointPosition> skelMap;
		std::map<XnSkeletonJoint, XnSkeletonJointPosition> localSkelMap;
		std::map<XnSkeletonJoint, XnSkeletonJointOrientation> orientMap;
		std::map<XnSkeletonJoint, XnSkeletonJointOrientation> localOrientMap;
		XnVector3D com;
		double timeStamp;
	};
	UserTrackingInfo m_trackingInfo, m_lastTrackingInfo;

	// One posture combination recognizer per posture combination
	PostureCombinationRecognizer* m_postureCombinationRecognizers[Fubi::NUM_POSTURE_COMBINATIONS];
	// And all user defined ones that are currently enabled
	std::map<std::string, PostureCombinationRecognizer*> m_userDefinedPostureCombinationRecognizers;

	// Time between the finger count detection of one hand
	double m_fingerTrackIntervall;

	// When the last detection of the finger count of each hand happened, -1 if disabled
	double m_lastRightFingerDetection, m_lastLeftFingerDetection;
	bool m_useConvexityDefectMethod;
	unsigned int m_maxFingerCountForMedian;

private:
	// Adds a finger count detection to the deque for later median calculation
	void addFingerCount(int count, bool leftHand = false);


	void calculateGlobalOrientations();

	void calculateLocalTransformations();

	void updateCombinationRecognizers();

	void updateFingerCount();


	std::deque<int> m_rightFingerCount, m_leftFingerCount;

		
};