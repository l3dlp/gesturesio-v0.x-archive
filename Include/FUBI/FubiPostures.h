// ****************************************************************************************
//
// Fubi Tracker - Predefined posture and gesture ids and names
// -------------------------------------------------------------
// Copyright (C) 2010-2012 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
//
// 
// ****************************************************************************************

#pragma once

#include "FubiUtils.h"

namespace Fubi
{
	// All predefined postures that can be recognized
	enum Postures
	{
		RIGHT_HAND_OVER_SHOULDER = 0,
		LEFT_HAND_OVER_SHOULDER,
		ARMS_CROSSED,
		ARMS_NEAR_POCKETS,
		ARMS_DOWN_TOGETHER,
		RIGHT_HAND_OUT,
		LEFT_HAND_OUT,
		HANDS_FRONT_TOGETHER,
		LEFT_KNEE_UP,
		RIGHT_KNEE_UP,
		RIGHT_HAND_OVER_HEAD,
		LEFT_HAND_OVER_HEAD,
		RIGHT_HAND_LEFT_OF_SHOULDER,
		RIGHT_HAND_RIGHT_OF_SHOULDER,
		POINTING_RIGHT,
		RIGHT_HAND_CLOSE_TO_ARM,
		LEFT_HAND_CLOSE_TO_ARM,
		NUM_POSTURES
	};

	// All predefined posture combinations that can be recognized
	enum PostureCombinations
	{
		WAVE_RIGHT_HAND_OVER_SHOULDER = 0,
		WAVE_RIGHT_HAND,
		CLIMBING_HANDS,
		THROWING_RIGHT,
		BALANCING,
		NUM_POSTURE_COMBINATIONS,
	};

	// String names for each posture
	static const char* getPostureName(Postures postureID)
	{
		switch (postureID)
		{
		case RIGHT_HAND_OVER_SHOULDER:
			return "Right hand over shoulder";
		case LEFT_HAND_OVER_SHOULDER:
			return "Left hand over shoulder";
		case ARMS_CROSSED:
			return "Arms crossed";
		case ARMS_NEAR_POCKETS:
			return "Arms near pockets";
		case ARMS_DOWN_TOGETHER:
			return "Arms down together";
		case RIGHT_HAND_OUT:
			return "Right hand out";
		case LEFT_HAND_OUT:
			return "Left hand out";
		case HANDS_FRONT_TOGETHER:
			return "Hands front together";
		case LEFT_KNEE_UP:
			return "Left knee up";
		case RIGHT_KNEE_UP:
			return "Right knee up";
		case RIGHT_HAND_OVER_HEAD:
			return "Right hand over head";
		case LEFT_HAND_OVER_HEAD:
			return "Left hand over head";
		case RIGHT_HAND_LEFT_OF_SHOULDER:
			return "Right hand left of shoulder";
		case RIGHT_HAND_RIGHT_OF_SHOULDER:
			return "Right hand right of shoulder";
		case POINTING_RIGHT:
			return "Pointing right";
		case RIGHT_HAND_CLOSE_TO_ARM:
			return "Right hand close to arm";
		case LEFT_HAND_CLOSE_TO_ARM:
			return "Left hand close to arm";
		case NUM_POSTURES:
			return "Invalid posture id";
		}

		return "Unknown posture id";
	};

	static Postures getPostureID(const std::string& name)
	{
		std::string normName = removeWhiteSpacesAndToLower(name);
		for (unsigned int p = 0; p < NUM_POSTURES; p++)
		{
			if (normName.compare(removeWhiteSpacesAndToLower(getPostureName((Postures)p))) == 0)
			{
				return (Postures)p;
			}
		}
		return NUM_POSTURES;
	}

	// String names for each posture combination
	static const char* getPostureCombinationName(PostureCombinations postureID)
	{
		switch (postureID)
		{
			case WAVE_RIGHT_HAND_OVER_SHOULDER:
				return "Waving right hand over shoulder";
			case WAVE_RIGHT_HAND:
				return "Waving right hand";
			case CLIMBING_HANDS:
				return "Climbing hands movement";
			case THROWING_RIGHT:
				return "Throwing with right hand";
			case BALANCING:
				return "Balancing";
			case NUM_POSTURE_COMBINATIONS:
				return "Posture combination id not set (User defined recognizer?)";
		}

		return "Unknown posture combination id";
	};
}