// ****************************************************************************************
//
// Fubi API
// ---------------------------------------------------------
// Copyright (C) 2010-2012 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************

#pragma once
#ifndef FUBI_H
#define FUBI_H

#if defined ( WIN32 ) || defined( _WINDOWS )
#	 ifdef FUBI_EXPORTS
#       define FUBI_API __declspec( dllexport )
#	 else
#       define FUBI_API __declspec( dllimport )
#    endif
#else
#	 define FUBI_API
#endif


#include <set>
#include <vector>

#include "FubiPostures.h"
#include "FubiUtils.h"
#include "FubiUser.h"

/**
 * \mainpage Fubi - Full Body Interaction Framework
 * 
 * \section intro_sec Introduction
 * 
 * Full Body Interaction Framework (FUBI) is a framework for recognizing full body gestures and postures in real time from the data of an OpenNI-applicable depth sensor, especially the Microsoft Kinect sensor.
  */


/**
 * \namespace Fubi
 *
 * \brief The Fubi namespace provides all methods to control Fubi.
 *
 */
namespace Fubi
{
	/** \addtogroup Fubi API
	 * All the C++ API functions
	 * 
	 * @{
	 */

	/**
	 * \brief Initializes OpenNI with the given xml file and sets the tracking calibration file
	 * 
	 * @param openniXmlconfig name of the xml file for openNI initialization inlcuding all needed productions nodes 
		(should be placed in the working directory, i.e. "bin" folder)
	 * @param calibrationFile name of the binary file, were the OpenNI tracking calibration is saved or loaded from
	 	(should also be placed in the working directory, i.e. "bin" folder)
	 *
	 * Default openni xml configuration file (note that only specific resolutions and FPS values are allowed):
	 	<OpenNI>
		  <Log writeToConsole="true" writeToFile="false">
			<!-- 0 - Verbose, 1 - Info, 2 - Warning, 3 - Error (default) -->
			<LogLevel value="2"/>
		  </Log>
		  <ProductionNodes>
			<Node type="Image">
			  <Configuration>
				<MapOutputMode xRes="1280" yRes="1024" FPS="15"/>
				<Mirror on="true"/>
			  </Configuration>
			</Node>
			<Node type="Depth">
			  <Configuration>
				<MapOutputMode xRes="640" yRes="480" FPS="30"/>
				<Mirror on="true"/>
			  </Configuration>
			</Node>
			<Node type="Scene" />
			<Node type="User" />
		  </ProductionNodes>
		</OpenNI>
	 */
	FUBI_API void init(const char* openniXmlconfig = 0x0, const char* calibrationFile = 0x0, XnSkeletonProfile profile = XN_SKEL_PROFILE_ALL);

	/**
	 * \brief Shuts down OpenNI and the tracker, releasing all allocated memory
	 * 
	 */
	FUBI_API void release();

	/**
	 * \brief Returns true if OpenNI has been already initialized
	 * 
	 */
	FUBI_API bool isInitialized();

	/**
	 * \brief Returns the OpenNI user generator for direct access
	 * 
	 */
	FUBI_API xn::UserGenerator* getUserGenerator();

	/**
	 * \brief Returns the OpenNI depth generator for direct access
	 * 
	 */
	FUBI_API xn::DepthGenerator* getDepthGenerator();

	/**
	 * \brief Returns the OpenNI image generator for direct access
	 * 
	 */
	FUBI_API xn::ImageGenerator* getImageGenerator();

	/**
	 * \brief Returns the OpenNI ir generator for direct access
	 * Note: You have to replace the image production node in the OpenNI xml config file
	 *       with an IR production node (There Can Be Only One)
	 * 
	 */
	FUBI_API xn::IRGenerator* getIRGenerator();

	/**
	 * \brief Updates all OpenNI production node getting the next frame of depth, rgb, and tracking data.
	 *        Also searches for users in the scene and loads the default tracking calibration for new users or request a calibration
	 * 
	 */
	FUBI_API void update();

	/**
	 * \brief retrieve an image from one of the OpenNI production nodes with specific format and optionally enhanced by different
	 *        tracking information 
	 *		  Some render options require an OpenCV installation!
	 *
	 * @param outputImage pointer to a unsigned char array
	 *        Will be filled with wanted image
	 *		  Array has to be of correct size, e.g. depth image (640x480 std resolution) with tracking info
	 *		  requires 4 channels (RGBA) --> size = 640*480*4 = 1228800
	 * @param type can be color, depth, or ir image
	 * @param numChannels number channels in the image 1, 3 or 4
	 * @param depth the pixel depth of the image, 8 bit (standard) or 16 bit (mainly usefull for depth images
	 * @param renderOptions options for rendering additional informations into the image (e.g. tracking skeleton)
	 * @param depthModifications options for transforming the depht image to a more visible format
	*/
	FUBI_API bool getImage(unsigned char* outputImage, ImageType::List type, ImageNumChannels::List numChannels, ImageDepth::List depth, 
		unsigned int renderOptions = (RenderOptions::Shapes | RenderOptions::Skeletons | RenderOptions::UserCaptions),
		DepthImageModification::List depthModifications = DepthImageModification::UseHistogram);

	/**
	 * \brief save an image from one of the OpenNI production nodes with specific format and optionally enhanced by different
	 *        tracking information
	 *
	 * @param filename filename where the image should be saved to
	 *        can be relative to the working directory (bin folder) or absolute
	 *		  the file extension determins the file format (should be jpg)
	 * @param jpegQuality qualitiy (= 88) of the jpeg compression if a jpg file is requested, ranges from 0 to 100 (best quality)
	 * @param type can be color, depth, or ir image
	 * @param numChannels number channels in the image 1, 3 or 4
	 * @param depth the pixel depth of the image, 8 bit (standard) or 16 bit (mainly usefull for depth images
	 * @param userId If set to something else than 0 an image will be cut cropped around the joint of intereset of this user, if 0 the whole image is put out
	 * @param jointOfInterest the joint of the user the image is cropped around
	 * @param renderOptions options for rendering additional informations into the image (e.g. tracking skeleton)
	 * @param depthModifications options for transforming the depht image to a more visible format
	*/
	FUBI_API bool saveImage(const char* fileName, int jpegQuality, ImageType::List type, ImageNumChannels::List numChannels, ImageDepth::List depth,
		XnUserID userId = 0, XnSkeletonJoint jointOfInterest = XN_SKEL_HEAD,
		unsigned int renderOptions = (RenderOptions::Shapes | RenderOptions::Skeletons | RenderOptions::UserCaptions),
		DepthImageModification::List depthModifications = DepthImageModification::UseHistogram);

	/**
	 * \brief If set to true the calibration loaded from file or calibrated by the first user will be applied to all others
	 * 
	 * @param keepIt Apply the calibration to all users or calibrate everyone individually
	 */
	FUBI_API void keepFirstCalibration(bool keepIt);

	/**
	 * \brief Sets the OpenNI paramter for smoothing
	 * 
	 * @param smoothing paramter between 0 (no smoothing) and 1 (completely keep the old data)
	 *        The initial value is 0 (no smoothing) because you better do the smoothing yourself apapted to your needs!
	 */
	FUBI_API void setSmoothing(float smoothing);

	/**
	 * \brief Tries to recognize a posture in the current frame of tracking data of one user
	 * 
	 * @param postureID enum id of the posture to be found in FubiPostures.h
	 * @param userID the OpenNI user id of the user to be checked
	 * @return true in case of a succesful detection
	 */
	FUBI_API bool isPostureDetected(Postures postureID, XnUserID userID);

	/**
	 * \brief Checks a user defined gesture or posture recognizer for its success
	 * 
	 * @param recognizerID id of the recognizer return during its creation
	 * @param userID the OpenNI user id of the user to be checked
	 * @return true in case of a succesful detection
	 */
	FUBI_API bool isUserDefinedGestureDetected(unsigned int recognizerIndex, XnUserID userID);

	/**
	 * \brief Checks a user defined gesture or posture recognizer for its success
	 * 
	 * @param recognizerName name of the recognizer return during its creation
	 * @param userID the OpenNI user id of the user to be checked
	 * @return true in case of a succesful detection
	 */
	FUBI_API bool isUserDefinedGestureDetected(const char* recognizerName, XnUserID userID);

	/**
	 * \brief Checks a posture/movement combination recognizer for its success
	 * 
	 * @param postureID  enum id of the posture combination to be found in FubiPostures.h
	 * @param userID the OpenNI user id of the user to be checked
	 * @param userStates (= 0x0) pointer to a vector of tracking data that represents the tracking information of the user
	 *		  during the recognition of each state
	 * @param restart (=true) if set to true, the recognizer automatically restarts, so the posture combination can be recognized again.
	 * @return true in case of a succesful detection
	 */
	FUBI_API bool isPostureCombinationDetected(PostureCombinations postureID, XnUserID userID, std::vector<FubiUser::UserTrackingInfo>* userStates = 0x0, bool restart = true);

	/**
	 * \brief Checks a user defined posture/movement combination recognizer for its success
	 * 
	 * @param postureName name of the posture combination
	 * @param userID the OpenNI user id of the user to be checked
	 * @param userStates (= 0x0) pointer to a vector of tracking data that represents the tracking information of the user
	 *		  during the recognition of each state
	 * @param restart (=true) if set to true, the recognizer automatically restarts, so the posture combination can be recognized again.
	 * @return true in case of a succesful detection
	 */
	FUBI_API bool isUserDefinedPostureCombinationDetected(const char* recognizerName, XnUserID userID, std::vector<FubiUser::UserTrackingInfo>* userStates = 0x0, bool restart = true);

	/**
	 * \brief Starts or stops the recognition process of a posture combination for one user
	 * 
	 * @param postureID enum id of the posture combination to be found in FubiPostures.h
	 * @param userID the OpenNI user id of the user for whom the recognizers should be modified
	 * @param enable if set to true, the recognizer will be started (if not already stared), else it stops
	 */
	FUBI_API void enablePostureCombinationDetection(PostureCombinations postureID, XnUserID userID, bool enable);

	/**
	 * \brief Starts or stops the recognition process of a user defined posture combination for one user
	 * 
	 * @param postureName name defined for this recognizer
	 * @param userID the OpenNI user id of the user for whom the recognizers should be modified
	 * @param enable if set to true, the recognizer will be started (if not already stared), else it stops
	 */
	FUBI_API void enableUserDefinedPostureCombinationDetection(const char* postureName, XnUserID userID, bool enable);

	/**
	 * \brief Automatically starts all posture combinationDetections for new users
	 * 
	 * @param enable if set to true, the recognizer will automatically start, else this must be done manually (by using enablePostureCombinationDetection(..))
	 * @param postureID enum id of the posture combination to be found in FubiPostures.h
	 */
	FUBI_API void autoStartPostureCombinationDetection(bool enable, PostureCombinations postureID = NUM_POSTURE_COMBINATIONS);

	/**
	 * \brief Returns the color for a user in the background image
	 * 
	 * @param id OpennNI user id of the user of interest
	 * @param r, g, b returns the red, green, and blue components of the color in which the users shape is displayed in the tracking image
	 */
	FUBI_API void getColorForUserID(XnUserID id, float& r, float& g, float& b);


	/**
	 * \brief Returns the name of a user
	 * 
	 * @param  userId OpennNI user id of the user of interest
	 * @return returns the user name or an empty string if the user is not found or the name not set
	 */
	FUBI_API const char* getUserName(XnUserID userId);

	/**
	 * \brief Gives a name to a user and loads a specific calibration for him (request recalibration if file not found)
	 * 
	 * @param  userId OpennNI user id of the user of interest
	 * @param name to be given to the user, also defines the calibration file name (name + "Calibration.bin")
	 *       Function has no effect if name already exists!
	 */
	FUBI_API void setUserName(XnUserID userId, const char* name);

	/**
	 * \brief Returns the OpenNI user id from the user index
	 * 
	 * @param index index of the user in the user array
	 * @return OpenNI user id of that user or 0 if not found
	 */
	FUBI_API XnUserID getUserID(unsigned int index);

	/**
	 * \brief Returns all users that do not have a name yet
	 * 
	 * @param userContainer pointer to an array were the ids of all unknown users will be copied to
	 * @return number of unknown users
	 */
	FUBI_API unsigned int getUnkownUsers(XnUserID* userContainer);


	/**
	 * \brief Creates a user defined posture recognizer
	 * 
	 * @param joint the joint of interest
	 * @param relJoint the joint in which it has to be in a specifc relation
	 * @param useCOMInsteadOfRelJoint (= false) use the center of mass as relation instead of relJoint
	 * @param minValues (=-inf, -inf, -inf) the minimal values allowed for the vectore relJoint -> joint
	 * @param maxValues (=inf, inf, inf) the maximal values allowed for the vectore relJoint -> joint
	 * @param minDistance (= 0) the minimal distance between joint and relJoint
	 * @param minDistance (= inf) the maximal distance between joint and relJoint
	 * @param atIndex (= -1) if an index is given, the corresponding recognizer will be replaced instead of creating a new one
	 *
	 * @return index of the recognizer needed to call it later
	 */
	FUBI_API unsigned int addJointRelationRecognizer(XnSkeletonJoint joint, XnSkeletonJoint relJoint,
		float minX = -Fubi::Math::MaxFloat, float minY = -Fubi::Math::MaxFloat, float minZ = -Fubi::Math::MaxFloat, 
		float maxX = Fubi::Math::MaxFloat, float maxY = Fubi::Math::MaxFloat, float maxZ = Fubi::Math::MaxFloat, 
		float minDistance = 0, 
		float maxDistance = Fubi::Math::MaxFloat,
		bool useCOMInsteadOfRelJoint = false,
		bool uselocalPositions = false,
		int atIndex = -1,
		const char* name = 0);

	/**
	 * \brief Creates a user defined linear movement recognizer
	 * 
	 * @param joint the joint of interest
	 * @param relJoint the joint in which it has to be in a specifc relation
	 * @param direction the direction in which the movement should happen
	 * @param minVel the minimal velocity that has to be reached in this direction
	 * @param maxVel (= inf) the maximal velocity that is allowed in this direction
	 * @param useCOMInsteadOfRelJoint (= false) use the center of mass as relation
	 * @param atIndex (= -1) if an index is given, the corresponding recognizer will be replaced instead of creating a new one
	 *
	 * @return index of the recognizer needed to call it later
	 */
	// A linear gesture has a vector calculated as joint - relative joint, 
	// the direction (each component -1 to +1) that will be applied per component on the vector, an a min and max vel in milimeter per second
	FUBI_API unsigned int addLinearMovementRecognizer(XnSkeletonJoint joint, XnSkeletonJoint relJoint, 
		float dirX, float dirY, float dirZ, float minVel, float maxVel = Fubi::Math::MaxFloat,
		bool uselocalPositions = false,
		int atIndex = -1,
		const char* name = 0);
	FUBI_API unsigned int addLinearMovementRecognizer(XnSkeletonJoint joint, 
		float dirX, float dirY, float dirZ, float minVel, float maxVel = Fubi::Math::MaxFloat,
		bool relativeToCOM = false,
		bool uselocalPositions = false,
		int atIndex = -1,
		const char* name = 0);

	 /**
		 * \brief Loads a recognizer config xml file and adds the configured recognizers
		 * 
		 * @para fileName name of the xml config file
		 * @return true if at least one recognizers was loaded from the given xml
		 */ 
	 FUBI_API bool loadRecognizersFromXML(const char* fileName);

	/**
	 * \brief Returns current number of user defined recognizers
	 * 
	 * @return number of recognizers, the recognizers also have the indices 0 to numberOfRecs-1
	 */
	FUBI_API unsigned int getNumUserDefinedRecognizers();

	/**
	 * \brief Returns the name of a user defined recognizer
	 * 
	 * @param  recognizerIndex index of the recognizer
	 * @return returns the recognizer name or an empty string if the user is not found or the name not set
	 */
	FUBI_API const char* getUserDefinedRecognizerName(unsigned int recognizerIndex);

	/**
	 * \brief Returns the index of a user defined recognizer
	 * 
	 * @param recognizerName name of the recognizer
	 * @return returns the recognizer name or -1 if not found
	 */
	FUBI_API int getUserDefinedRecognizerIndex(const char* recognizerName);

	/**
	 * \brief Returns the index of a user defined posture combination recognizer
	 * 
	 * @param recognizerName name of the recognizer
	 * @return returns the recognizer name or -1 if not found
	 */
	FUBI_API int getUserDefinedPostureCombinationRecognizerIndex(const char* recognizerName);

	/**
	 * \brief Returns current number of user defined posture combination recognizers
	 * 
	 * @return number of recognizers, the recognizers also have the indices 0 to numberOfRecs-1
	 */
	FUBI_API unsigned int getNumUserDefinedPostureCombinationRecognizers();

	/**
	 * \brief Returns the name of a user defined posture combination recognizer
	 * 
	 * @param  recognizerIndex index of the recognizer
	 * @return returns the recognizer name or an empty string if the user is not found or the name not set
	 */
	FUBI_API const char* getUserDefinedPostureCombinationRecognizerName(unsigned int recognizerIndex);

	/**
	 * \brief Returns all current users with their tracking information
	 * 
	 * @param pUserContainer (=0) pointer where a pointer to the current users will be stored at
	 *        The maximal size is Fubi::MaxUsers, but the current size can be requested by leaving the Pointer at 0
	 * @return the current number of users (= valid users in the container)
	 */
	FUBI_API XnUInt16 getCurrentUsers(FubiUser*** pUserContainer = 0);

	/**
	 * \brief Returns one user with his tracking information
	 * 
	 * @param id OpenNI id of the user
	 * @return a pointer to the user data
	 */
	FUBI_API FubiUser* getUser(XnUserID id);


	/**
	 * \brief Returns the current depth resolution or -1, -1 if failed
	 * 
	 * @param width, height the resolution
	 */
	FUBI_API void getDepthResolution(int& width, int& height);

	/**
	 * \brief Returns the current rgb resolution or -1, -1 if failed
	 * 
	 * @param width, height the resolution
	 */
	FUBI_API void getRgbResolution(int& width, int& height);

	/**
	 * \brief Returns the current ir resolution or -1, -1 if failed
	 * 
	 * @param width, height the resolution
	 */
	FUBI_API void getIRResolution(int& width, int& height);

	/**
	 * \brief Returns the number of shown fingers detected at the hand of one user (REQUIRES OPENCV!)
	 * 
	 * @param userID OpenNI id of the user
	 * @param leftHand looks at the left instead of the right hand
	 * @param useConvexityDefectMethod if true using old method that calculates the convexity defects
	 * @return the number of shown fingers detected, 0 if there are none or there is an error
	 */
	FUBI_API int getFingerCount(XnUserID userID, bool leftHand = false, bool useConvexityDefectMethod = false);

	/**
	 * \brief Enables/Disables finger tracking for the hands of one user
	 *        If enabled the finger count will be tracked over time and the 
	 *		  median of these value will be returned in case of a query
	 *		  (REQUIRES OPENCV!)
	 * 
	 * @param userID OpenNI id of the user
	 * @param leftHand enable/disable finger tracking for the left hand
	 * @param rightHand enable/disable finger tracking for the right hand
	 */
	FUBI_API void enableFingerTracking(XnUserID userID, bool leftHand, bool rightHand, bool useConvexityDefectMethod = false);


	/**
	 * \brief  Whether the user is currently seen in the depth image
	 *
	 * @param userID OpenNI id of the user
	 */
	FUBI_API bool isUserInScene(XnUserID userID);

	/**
	 * \brief Whether the user is currently tracked
	 *
	 * @param userID OpenNI id of the user
	 */
	FUBI_API bool isUserTracked(XnUserID userID);
	

	/**
	 * \brief Get the most current tracking info of the user
	 * (including all joint positions and orientations, the center of mass and a timestamp)
	 *
	 * @param userID OpenNI id of the user
	 * @return the user tracking info struct
	 */
	FUBI_API FubiUser::UserTrackingInfo* getCurrentUserTrackingInfo(XnUserID userId);

	/**
	 * \brief Get the last tracking info of the user (one frame before the current one)
	 * (including all joint positions and orientations, the center of mass and a timestamp)
	 *
	 * @param userID OpenNI id of the user
	 * @return the user tracking info struct
	 */
	FUBI_API FubiUser::UserTrackingInfo* getLastUserTrackingInfo(XnUserID userId);
		
		
	/**
	 * \brief  Get the skeleton joint position out of the tracking info
	 *
	 * @param trackingInfo the trackinginfo struct to extract the info from
	 * @param jointId
	 * @param x, y, z [out] where the position of the joint will be copied to
	 * @param confidence [out] where the confidence for this position will be copied to
	 * @param timestamp [out] where the timestamp of this tracking info will be copied to (seconds since program start)
	 * @param localPosition if set to true, the function will return local position (vector from parent joint)
	 */
	FUBI_API void getSkeletonJointPosition(FubiUser::UserTrackingInfo* trackingInfo, XnSkeletonJoint joint, float& x, float& y, float& z, float& confidence, double& timeStamp, bool localPosition = false);

	/**
	 * \brief  Get the skeleton joint orientation out of the tracking info
	 *
	 * @param trackingInfo the trackinginfo struct to extract the info from
	 * @param jointId
	 * @param mat [out] rotation 3x3 matrix (9 floats)
	 * @param confidence [out] the confidence for this position
	 * @param timestamp [out] (seconds since program start)
	 * @param localOrientation if set to true, the function will local orientations (cleared of parent orientation) instead of globals
	 */
	FUBI_API void getSkeletonJointOrientation(FubiUser::UserTrackingInfo* trackingInfo, XnSkeletonJoint joint, float* mat, float confidence, double& timeStamp, bool localOrientation = true);

	/**
	 * \brief  Get the skeleton joint position out of the tracking info
	 *
	 * @param trackingInfo the trackinginfo struct to extract the info from
	 * @param x, y, z the position of the joint
	 * @param confidence the confidence for this position
	 * @param timestamp (seconds since program start)
	 */
	FUBI_API void getCenterOfMass(FubiUser::UserTrackingInfo* trackingInfo, float& x, float& y, float& z, double& timeStamp);

	/**
	 * \brief  Creates an empty vector of UserTrackinginfo structs
	 *
	 */
	FUBI_API std::vector<FubiUser::UserTrackingInfo>* createTrackingInfoVector();
	
	/**
	 * \brief  Releases the formerly created vector
	 *
	 * @param vec the vector that will be released
	 */
	FUBI_API void releaseTrackingInfoVector(std::vector<FubiUser::UserTrackingInfo>* vec);

	/**
	 * \brief  Returns the size of the vector
	 *
	 * @param vec the vector that we get the size of
	 */
	FUBI_API unsigned int getTrackingInfoVectorSize(std::vector<FubiUser::UserTrackingInfo>* vec);
	/**
	 * \brief  Returns one element of the tracking info vector
	 *
	 * @param vec the vector that we get the element of
	 */
	FUBI_API FubiUser::UserTrackingInfo* getTrackingInfo(std::vector<FubiUser::UserTrackingInfo>* vec, unsigned int index);

	/**
	 * \brief Returns the OpenNI id of the users standing closest to the sensor
	 */
	FUBI_API XnUserID getClosestUserID();

	/**
	 * \brief Stops and removes all user defined recognizers
	 */
	FUBI_API void clearUserDefinedRecognizers();

	/**
	 * \brief Set the current tracking info of one user
	 * (including all joint positions and the center of mass. Optionally the orientations and a timestamp)
	 *
	 * @param userID OpenNI id of the user
	 * @param positions an array of the joint positions
	 * @param com the center of mass
	 * @param timestamp the timestamp of the tracking value (if -1 an own timestamp will be created)
	 * @param orientations an array of the joint positions (if 0, the orientations will be approximated from the given positions)
	 * @param localOrientations same as orientations, but in the local coordinate system of the joint
	 */
	FUBI_API void updateTrackingInfo(XnUserID userId, XnSkeletonJointPosition* positions, XnVector3D com,
		double timeStamp = -1, XnSkeletonJointOrientation* orientations = 0, XnSkeletonJointOrientation* localOrientations = 0);
	/* same function as before, but without OpenNI types,
	   i.e. 1 skeleton = per joint position, orientation, local orientation all three with 4 floats (x,y,z,conf) in milimeters or degrees,
	   com = 3 floats (x,y,z) in milimeters, timeStamp in seconds or -1 for self calculation*/
	FUBI_API void updateTrackingInfo(XnUserID userId, float* skeleton, float* com = 0,
		double timeStamp = -1);

	/*! @}*/
}

#endif