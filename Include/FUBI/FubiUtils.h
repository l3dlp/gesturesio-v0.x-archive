// ****************************************************************************************
//
// Fubi Tracker - Utility  and math functions
// ---------------------------------------------------------
// Copyright (C) 2010-2012 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************
#pragma once

#include <XnCppWrapper.h>

#include <cmath>
#include <time.h>
#include <string>

namespace Fubi
{
	// Options for image rendering
	struct ImageType
	{
		/*	The possible image types
		*/
		enum List
		{
			Color,
			Depth,
			IR,
			Blank
		};
	};
	struct ImageNumChannels
	{
		/*	The number of channels in the image
		*/
		enum List
		{
			C1 = 1,
			C3 = 3,
			C4 = 4
		};
	};
	struct ImageDepth
	{
		/*	The depth of each channel
		*/
		enum List
		{
			D8 = 8,
			D16 = 16
		};
	};
	struct DepthImageModification
	{
		/*	How the depth image should be modified for depth differences
			being easier to distinguish by the human eye
		*/
		enum List
		{
			Raw,
			UseHistogram,
			StretchValueRange,
			ConvertToRGB
		};
	};
	struct RenderOptions
	{
		/*	The possible formats for the tracking info rendering
		*/
		enum Options
		{
			None							= 0,
			Shapes							= 0x000001,
			Skeletons						= 0x000002,
			UserCaptions					= 0x000004,
			LocalOrientCaptions				= 0x000008,
			GlobalOrientCaptions			= 0x000010,
			LocalPosCaptions				= 0x000020,
			GlobalPosCaptions				= 0x000040,
			Background						= 0x000080,
		};
	};

	// Maximum depth value that can occure in the depth image
	static const int MaxDepth = 10000;
	// And maximum value in the IR image
	static const unsigned short MaxIR = 1024;
	// Maximum number of tracked users
	static const int MaxUsers = 15;

	static std::string removeWhiteSpacesAndToLower(const std::string& str)
	{
		std::string ret;
		for (unsigned int i = 0; i < str.length(); i++)
		{
			const char& c = str[i];
			if (c != ' ' && c != '_' && c != '-' && c != '|')
			{
				if (c >= 'A' && c <= 'Z')
					ret += (c + ('a'-'A'));
				else
					ret += c;
			}
		}
		return ret;
	}

	/**
	 * \brief Ensures that a value is between certain bounds
	 * 
	 * @param  value the value to be clamped
	 * @param  min the minimum allowed for the value
	 * @param  max the maximum allowed for the value
	 * @return returns the clamped value (value if between min and max, else min or max)
	 */
	template<class T> static inline T clamp( T value, T min, T max )
	{
		if( value < min ) value = min;
		else if( value > max ) value = max;

		return value;
	}

	/**
	 * \brief Ensures that a value is between certain bounds
	 * 
	 * @param  value the value to be clamped
	 * @param  min the minimum allowed for the value
	 * @param  max the maximum allowed for the value
	 * @param valueToSet this will be returned if a value exceeds the given borders
	 * @return returns the clamped value (value if between min and max, else min or max)
	 */
	template<class T> static inline T clamp( T value, T min, T max, T valueToSet)
	{
		if( value < min ) value = valueToSet;
		else if( value > max ) value = valueToSet;

		return value;
	}

	/**
	 * \brief Checks whether two values have the same sign
	 * 
	 * @param  a first value
	 * @param  b second value
	 * @return true if they have the same sign
	 */
	template<class T> static inline bool sameSign( T a, T b )
	{
		return (a <= 0 && b <= 0) || (a >= 0 && b >= 0);
	}

	/**
	 * \brief Number of seconds since the program start
	 */
	static double currentTime()
	{
	  return double(clock()) / CLOCKS_PER_SEC;
	}

	static const char* getJointName(XnSkeletonJoint id)
	{
		switch(id)
		{
		case XN_SKEL_HEAD:
			return "head";
		case XN_SKEL_NECK:
			return "neck";
		case XN_SKEL_TORSO:
			return "torso";
		case XN_SKEL_WAIST:
			return "waist";

		case XN_SKEL_LEFT_COLLAR:
			return "leftCollar";
		case XN_SKEL_LEFT_SHOULDER:
			return "leftShoulder";
		case XN_SKEL_LEFT_ELBOW:
			return "leftElbow";
		case XN_SKEL_LEFT_WRIST:
			return "leftWrist";
		case XN_SKEL_LEFT_HAND:
			return "leftHand";
		case XN_SKEL_LEFT_FINGERTIP:
			return "leftFingertip";

		case XN_SKEL_RIGHT_COLLAR:
			return "rightCollar";
		case XN_SKEL_RIGHT_SHOULDER:
			return "rightShoulder";
		case XN_SKEL_RIGHT_ELBOW:
			return "rightElbow";
		case XN_SKEL_RIGHT_WRIST:
			return "rightWrist";
		case XN_SKEL_RIGHT_HAND:
			return "rightHand";
		case XN_SKEL_RIGHT_FINGERTIP:
			return "rightFingertip";

		case XN_SKEL_LEFT_HIP:
			return "leftHip";
		case XN_SKEL_LEFT_KNEE:
			return "leftKnee";
		case XN_SKEL_LEFT_ANKLE:
			return "leftAnkle";
		case XN_SKEL_LEFT_FOOT:
			return "leftFoot";

		case XN_SKEL_RIGHT_HIP:
			return "rightHip";
		case XN_SKEL_RIGHT_KNEE:
			return "rightKnee";
		case XN_SKEL_RIGHT_ANKLE:
			return "rightAnkle";
		case XN_SKEL_RIGHT_FOOT:
			return "rightFoot";
		}

		return "";
	}

	static XnSkeletonJoint getJointID(const char* name)
	{
		if (_stricmp(name, "head") == 0)
			return XN_SKEL_HEAD;
		if (_stricmp(name, "neck") == 0)
			return XN_SKEL_NECK;
		if (_stricmp(name, "torso") == 0)			
			return XN_SKEL_TORSO;
		if (_stricmp(name, "waist") == 0)	
			return XN_SKEL_WAIST;

		if (_stricmp(name, "leftCollar") == 0)
			return XN_SKEL_LEFT_COLLAR;
		if (_stricmp(name, "leftShoulder") == 0)
			return XN_SKEL_LEFT_SHOULDER;
		if (_stricmp(name, "leftElbow") == 0)
			return XN_SKEL_LEFT_ELBOW;
		if (_stricmp(name, "leftWrist") == 0)
			return XN_SKEL_LEFT_WRIST;
		if (_stricmp(name, "leftHand") == 0)
			return XN_SKEL_LEFT_HAND;
		if (_stricmp(name, "leftFingertip") == 0)
			return XN_SKEL_LEFT_FINGERTIP;

		if (_stricmp(name, "rightCollar") == 0)
			return XN_SKEL_RIGHT_COLLAR;
		if (_stricmp(name, "rightShoulder") == 0)			
			return XN_SKEL_RIGHT_SHOULDER;
		if (_stricmp(name, "rightElbow") == 0)
			return XN_SKEL_RIGHT_ELBOW;
		if (_stricmp(name, "rightWrist") == 0)
			return XN_SKEL_RIGHT_WRIST;
		if (_stricmp(name, "rightHand") == 0)
			return XN_SKEL_RIGHT_HAND;
		if (_stricmp(name, "rightFingertip") == 0)
			return XN_SKEL_RIGHT_FINGERTIP;

		if (_stricmp(name, "leftHip") == 0)
			return XN_SKEL_LEFT_HIP;
		if (_stricmp(name, "leftKnee") == 0)
			return XN_SKEL_LEFT_KNEE;
		if (_stricmp(name, "leftAnkle") == 0)
			return XN_SKEL_LEFT_ANKLE;
		if (_stricmp(name, "leftFoot") == 0)
			return XN_SKEL_LEFT_FOOT;

		if (_stricmp(name, "rightHip") == 0)
			return XN_SKEL_RIGHT_HIP;
		if (_stricmp(name, "rightKnee") == 0)
			return XN_SKEL_RIGHT_KNEE;
		if (_stricmp(name, "rightAnkle") == 0)
			return XN_SKEL_RIGHT_ANKLE;
		if (_stricmp(name, "rightFoot") == 0)
			return XN_SKEL_RIGHT_FOOT;
			
		return XN_SKEL_HEAD;
	}

// The following is copied form the utMath.h of the Horde3D Engine by Nicolas Schulz. 
// Horde3D namespace and some unnescessary functions are removed.
// Some special vector functions are added

// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2011 Nicolas Schulz
//
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/legal/epl-v10.html
//
// *************************************************************************************************

// -------------------------------------------------------------------------------------------------
//
// Math library
//
// Conventions:
//
// - Coordinate system is right-handed with positive y as up axis
// - All rotation angles are counter-clockwise when looking from the positive end of the rotation
//	 axis towards the origin
// - An unrotated view vector points along the negative z-axis
//
// -------------------------------------------------------------------------------------------------


// Constants
namespace Math
{
	const unsigned int MaxUInt32 = 0xFFFFFFFF;
	const unsigned short MaxUShort16 = 0xFFFF;
	const int MinInt32 = 0x80000000;
	const int MaxInt32 = 0x7FFFFFFF;
	const float MaxFloat = 3.402823466e+38F;
	const float MinPosFloat = 1.175494351e-38F;
	
	const float Pi = 3.141592654f;
	const float TwoPi = 6.283185307f;
	const float PiHalf = 1.570796327f;

	const float Epsilon = 0.000001f;
	const float ZeroEpsilon = 32.0f * MinPosFloat;  // Very small epsilon for checking against 0.0f
#ifdef __GNUC__
	const float NaN = __builtin_nanf("");
#else
	const float NaN = *(float *)&MaxUInt32;
#endif

	enum NoInitHint
	{
		NO_INIT
	};
};


// -------------------------------------------------------------------------------------------------
// General
// -------------------------------------------------------------------------------------------------

static inline float degToRad( float f ) 
{
	return f * 0.017453293f;
}

static inline float radToDeg( float f ) 
{
	return f * 57.29577951f;
}

static inline float clampf( float f, float min, float max )
{
	if( f < min ) f = min;
	else if( f > max ) f = max;

	return f;
}

static inline float minf( float a, float b )
{
	return a < b ? a : b;
}

static inline float maxf( float a, float b )
{
	return a > b ? a : b;
}

static inline float fsel( float test, float a, float b )
{
	// Branchless selection
	return test >= 0 ? a : b;
}


// -------------------------------------------------------------------------------------------------
// Conversion
// -------------------------------------------------------------------------------------------------

static inline int ftoi_t( double val )
{
	// Float to int conversion using truncation
	
	return (int)val;
}

static inline int ftoi_r( double val )
{
	// Fast round (banker's round) using Sree Kotay's method
	// This function is much faster than a naive cast from float to int

	union
	{
		double dval;
		int ival[2];
	} u;

	u.dval = val + 6755399441055744.0;  // Magic number: 2^52 * 1.5;
	return u.ival[0];         // Needs to be [1] for big-endian
}


// -------------------------------------------------------------------------------------------------
// Vector
// -------------------------------------------------------------------------------------------------

class Vec3f
{
public:
	float x, y, z;
	
	
	// ------------
	// Constructors
	// ------------
	Vec3f() : x( 0.0f ), y( 0.0f ), z( 0.0f )
	{ 
	}

	explicit Vec3f( Math::NoInitHint )
	{
		// Constructor without default initialization
	}
	
	Vec3f( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) 
	{
	}

	Vec3f( const Vec3f &v ) : x( v.x ), y( v.y ), z( v.z )
	{
	}

	Vec3f( const XnVector3D &v ) : x( v.X ), y( v.Y ), z( v.Z )
	{
	}

	// ------
	// Access
	// ------
	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}
	
	// -----------
	// Comparisons
	// -----------
	bool operator==( const Vec3f &v ) const
	{
		return (x > v.x - Math::Epsilon && x < v.x + Math::Epsilon && 
		        y > v.y - Math::Epsilon && y < v.y + Math::Epsilon &&
		        z > v.z - Math::Epsilon && z < v.z + Math::Epsilon);
	}

	bool operator!=( const Vec3f &v ) const
	{
		return (x < v.x - Math::Epsilon || x > v.x + Math::Epsilon || 
		        y < v.y - Math::Epsilon || y > v.y + Math::Epsilon ||
		        z < v.z - Math::Epsilon || z > v.z + Math::Epsilon);
	}
	
	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec3f operator-() const
	{
		return Vec3f( -x, -y, -z );
	}

	Vec3f operator+( const Vec3f &v ) const
	{
		return Vec3f( x + v.x, y + v.y, z + v.z );
	}

	Vec3f &operator+=( const Vec3f &v )
	{
		return *this = *this + v;
	}

	Vec3f operator-( const Vec3f &v ) const 
	{
		return Vec3f( x - v.x, y - v.y, z - v.z );
	}

	Vec3f &operator-=( const Vec3f &v )
	{
		return *this = *this - v;
	}

	Vec3f operator*( const float f ) const
	{
		return Vec3f( x * f, y * f, z * f );
	}

	Vec3f &operator*=( const float f )
	{
		return *this = *this * f;
	}

	Vec3f operator/( const float f ) const
	{
		return Vec3f( x / f, y / f, z / f );
	}

	Vec3f &operator/=( const float f )
	{
		return *this = *this / f;
	}

	Vec3f operator*(const Vec3f &v) const
	{
		return Vec3f( x * v.x, y * v.y, z * v.z);
	}
	
	Vec3f &operator*=(const Vec3f &v)
	{
		return *this = *this * v;
	}

	Vec3f operator/(const Vec3f &v) const
	{
		return Vec3f( x / v.x, y / v.y, z / v.z);
	}
	
	Vec3f &operator/=(const Vec3f &v)
	{
		return *this = *this / v;
	}

	// ----------------
	// Special products
	// ----------------
	float dot( const Vec3f &v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3f cross( const Vec3f &v ) const
	{
		return Vec3f( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	// ----------------
	// Other operations
	// ----------------
	float length() const 
	{
		return sqrtf( x * x + y * y + z * z );
	}

	Vec3f normalized() const
	{
		float invLen = 1.0f / length();
		return Vec3f( x * invLen, y * invLen, z * invLen );
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}

	/*void fromRotation( float angleX, float angleY )
	{
		x = cosf( angleX ) * sinf( angleY ); 
		y = -sinf( angleX );
		z = cosf( angleX ) * cosf( angleY );
	}*/

	Vec3f toRotation() const
	{
		// Assumes that the unrotated view vector is (0, 0, -1)
		Vec3f v;
		
		if( y != 0 ) v.x = atan2f( y, sqrtf( x*x + z*z ) );
		if( x != 0 || z != 0 ) v.y = atan2f( -x, -z );

		return v;
	}

	Vec3f lerp( const Vec3f &v, float f ) const
	{
		return Vec3f( x + (v.x - x) * f, y + (v.y - y) * f, z + (v.z - z) * f ); 
	}
};


class Vec4f
{
public:
	
	float x, y, z, w;


	Vec4f() : x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{
	}
	
	explicit Vec4f( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}

	explicit Vec4f( Vec3f v ) : x( v.x ), y( v.y ), z( v.z ), w( 1.0f )
	{
	}

	Vec4f operator+( const Vec4f &v ) const
	{
		return Vec4f( x + v.x, y + v.y, z + v.z, w + v.w );
	}
	
	Vec4f operator*( const float f ) const
	{
		return Vec4f( x * f, y * f, z * f, w * f );
	}
};


// -------------------------------------------------------------------------------------------------
// Quaternion
// -------------------------------------------------------------------------------------------------

class Quaternion
{
public:	
	
	float x, y, z, w;

	// ------------
	// Constructors
	// ------------
	Quaternion() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) 
	{ 
	}
	
	explicit Quaternion( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}
	
	Quaternion( const float eulerX, const float eulerY, const float eulerZ )
	{
		Quaternion roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
		Quaternion pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
		Quaternion yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
	
		// Order: y * x * z
		*this = pitch * roll * yaw;
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Quaternion operator*( const Quaternion &q ) const
	{
		return Quaternion(
			y * q.z - z * q.y + q.x * w + x * q.w,
			z * q.x - x * q.z + q.y * w + y * q.w,
			x * q.y - y * q.x + q.z * w + z * q.w,
			w * q.w - (x * q.x + y * q.y + z * q.z) );
	}

	Quaternion &operator*=( const Quaternion &q )
	{
		return *this = *this * q;
	}

	// ----------------
	// Other operations
	// ----------------

	Quaternion slerp( const Quaternion &q, const float t ) const
	{
		// Spherical linear interpolation between two quaternions
		// Note: SLERP is not commutative
		
		Quaternion q1( q );

        // Calculate cosine
        float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

        // Use the shortest path
        if( cosTheta < 0 )
		{
			cosTheta = -cosTheta; 
			q1.x = -q.x; q1.y = -q.y;
			q1.z = -q.z; q1.w = -q.w;
        }

        // Initialize with linear interpolation
		float scale0 = 1 - t, scale1 = t;
		
		// Use spherical interpolation only if the quaternions are not very close
		if( (1 - cosTheta) > 0.001f )
		{
			// SLERP
			float theta = acosf( cosTheta );
			float sinTheta = sinf( theta );
			scale0 = sinf( (1 - t) * theta ) / sinTheta;
			scale1 = sinf( t * theta ) / sinTheta;
		} 
		
		// Calculate final quaternion
		return Quaternion( x * scale0 + q1.x * scale1, y * scale0 + q1.y * scale1,
		                   z * scale0 + q1.z * scale1, w * scale0 + q1.w * scale1 );
	}

	Quaternion nlerp( const Quaternion &q, const float t ) const
	{
		// Normalized linear quaternion interpolation
		// Note: NLERP is faster than SLERP and commutative but does not yield constant velocity

		Quaternion qt;
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;
		
		// Use the shortest path and interpolate linearly
		if( cosTheta < 0 )
			qt = Quaternion( x + (-q.x - x) * t, y + (-q.y - y) * t,
							 z + (-q.z - z) * t, w + (-q.w - w) * t );
		else
			qt = Quaternion( x + (q.x - x) * t, y + (q.y - y) * t,
							 z + (q.z - z) * t, w + (q.w - w) * t );

		// Return normalized quaternion
		float invLen = 1.0f / sqrtf( qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w );
		return Quaternion( qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen );
	}

	Quaternion inverted() const
	{
		float len = x * x + y * y + z * z + w * w;
		if( len > 0 )
        {
            float invLen = 1.0f / len;
            return Quaternion( -x * invLen, -y * invLen, -z * invLen, w * invLen );
		}
		else return Quaternion();
	}
};


// -------------------------------------------------------------------------------------------------
// Matrix
// -------------------------------------------------------------------------------------------------

class Matrix3f
{
public:
	union
	{
		float c[3][3];
		float x[9];
	};

	static Matrix3f RotMat( float x, float y, float z )
	{
		// Rotation order: YXZ [* Vector]
		return Matrix3f( Quaternion( x, y, z ) );
	}
	// ------------
	// Constructors
	// ------------
	Matrix3f()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1;
	}

	explicit Matrix3f( Math::NoInitHint )
	{
		// Constructor without default initialization
	}

	Matrix3f( const float *floatArray9 )
	{
		for( unsigned int i = 0; i < 3; ++i )
		{
			for( unsigned int j = 0; j < 3; ++j )
			{
				c[i][j] = floatArray9[i * 3 + j];
			}
		}
	}

	Matrix3f( const Quaternion &q )
	{
		// Calculate coefficients
		float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
		float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
		float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
		float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

		c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;		c[2][0] = xz + wy;
		c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);	c[2][1] = yz - wx;
		c[0][2] = xz - wy;        c[1][2] = yz + wx;		c[2][2] = 1 - (xx + yy);
	}

	// ----------
	// Matrix sum
	// ----------
	Matrix3f operator+( const Matrix3f &m ) const 
	{
		Matrix3f mf( Math::NO_INIT );
		
		mf.x[0] = x[0] + m.x[0];
		mf.x[1] = x[1] + m.x[1];
		mf.x[2] = x[2] + m.x[2];
		mf.x[3] = x[3] + m.x[3];
		mf.x[4] = x[4] + m.x[4];
		mf.x[5] = x[5] + m.x[5];
		mf.x[6] = x[6] + m.x[6];
		mf.x[7] = x[7] + m.x[7];
		mf.x[8] = x[8] + m.x[8];

		return mf;
	}

	Matrix3f &operator+=( const Matrix3f &m )
	{
		return *this = *this + m;
	}

	// ---------------------
	// Matrix multiplication
	// ---------------------
	Matrix3f operator*( const Matrix3f &m ) const 
	{
		Matrix3f mf( Math::NO_INIT );
		
		mf.x[0] = x[0] * m.x[0] + x[3] * m.x[1] + x[6] * m.x[2];
		mf.x[1] = x[1] * m.x[0] + x[4] * m.x[1] + x[7] * m.x[2];
		mf.x[2] = x[2] * m.x[0] + x[5] * m.x[1] + x[8] * m.x[2];

		mf.x[3] = x[0] * m.x[3] + x[3] * m.x[4] + x[6] * m.x[5];
		mf.x[4] = x[1] * m.x[3] + x[4] * m.x[4] + x[7] * m.x[5];
		mf.x[5] = x[2] * m.x[3] + x[5] * m.x[4] + x[8] * m.x[5];

		mf.x[6] = x[0] * m.x[6] + x[3] * m.x[7] + x[6] * m.x[8];
		mf.x[7] = x[1] * m.x[6] + x[4] * m.x[7] + x[7] * m.x[8];
		mf.x[8] = x[2] * m.x[6] + x[5] * m.x[7] + x[8] * m.x[8];

		return mf;
	}

	Matrix3f operator*( const float f ) const
	{
		Matrix3f m( *this );
		
		m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
		m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
		m.x[8]  *= f; 

		return m;
	}

	// ----------------------------
	// Vector-Matrix multiplication
	// ----------------------------
	Vec3f operator*( const Vec3f &v ) const
	{
		return Vec3f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
	}


	// ---------------
	// Transformations
	// ---------------
	void rotate( const float x, const float y, const float z )
	{
		*this = RotMat( x, y, z ) * *this;
	}

	// ---------------
	// Other
	// ---------------

	Matrix3f transposed() const
	{
		Matrix3f m( *this );
		
		for( unsigned int y = 0; y < 3; ++y )
		{
			for( unsigned int x = y + 1; x < 3; ++x ) 
			{
				float tmp = m.c[x][y];
				m.c[x][y] = m.c[y][x];
				m.c[y][x] = tmp;
			}
		}

		return m;
	}

	float determinant() const
	{
		return 
			c[0][0]*c[1][1]*c[2][2] + c[0][1]*c[1][2]*c[2][0]+ c[0][2]*c[1][0]*c[2][1]
		  - c[0][2]*c[1][1]*c[2][0] - c[0][1]*c[1][0]*c[2][2] - c[0][0]*c[1][2]*c[2][1];
	}

	Matrix3f inverted() const
	{
		float d = determinant();
		if( d == 0 )
			return Matrix3f();
		d = 1.0f / d;
		
		return this->transposed() * d;
	}

	Vec3f getRot() const
	{
		Vec3f rot;

		rot.x = radToDeg(sinf(-x[7]));

		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		float f = abs(x[7]);
		if (f > 0.999f && f < 1.001f)
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rot.y = 0;

			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rot.z = radToDeg(atan2f(-x[3], x[0]));
		}
		// Standard case
		else
		{
			rot.y = radToDeg(atan2f(x[6], x[8]));
			rot.z = radToDeg(atan2f(x[1], x[4]));
		}

		return rot;
	}

	Vec4f getCol( unsigned int col ) const
	{
		return Vec4f( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
	}

	Vec4f getRow( unsigned int row ) const
	{
		return Vec4f( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
	}
};

class Matrix4f
{
public:
	
	union
	{
		float c[4][4];	// Column major order for OpenGL: c[column][row]
		float x[16];
	};
	
	// --------------
	// Static methods
	// --------------
	static Matrix4f TransMat( float x, float y, float z )
	{
		Matrix4f m;

		m.c[3][0] = x;
		m.c[3][1] = y;
		m.c[3][2] = z;

		return m;
	}

	static Matrix4f ScaleMat( float x, float y, float z )
	{
		Matrix4f m;
		
		m.c[0][0] = x;
		m.c[1][1] = y;
		m.c[2][2] = z;

		return m;
	}

	static Matrix4f RotMat( float x, float y, float z )
	{
		// Rotation order: YXZ [* Vector]
		return Matrix4f( Quaternion( x, y, z ) );
	}

	static Matrix4f RotMat( Vec3f axis, float angle )
	{
		axis = axis * sinf( angle * 0.5f );
		return Matrix4f( Quaternion( axis.x, axis.y, axis.z, cosf( angle * 0.5f ) ) );
	}

	static Matrix4f PerspectiveMat( float l, float r, float b, float t, float n, float f )
	{
		Matrix4f m;

		m.x[0] = 2 * n / (r - l);
		m.x[5] = 2 * n / (t - b);
		m.x[8] = (r + l) / (r - l);
		m.x[9] = (t + b) / (t - b);
		m.x[10] = -(f + n) / (f - n);
		m.x[11] = -1;
		m.x[14] = -2 * f * n / (f - n);
		m.x[15] = 0;

		return m;
	}

	static Matrix4f OrthoMat( float l, float r, float b, float t, float n, float f )
	{
		Matrix4f m;

		m.x[0] = 2 / (r - l);
		m.x[5] = 2 / (t - b);
		m.x[10] = -2 / (f - n);
		m.x[12] = -(r + l) / (r - l);
		m.x[13] = -(t + b) / (t - b);
		m.x[14] = -(f + n) / (f - n);

		return m;
	}

	static void fastMult43( Matrix4f &dst, const Matrix4f &m1, const Matrix4f &m2 )
	{
		// Note: dst may not be the same as m1 or m2

		float *dstx = dst.x;
		const float *m1x = m1.x;
		const float *m2x = m2.x;
		
		dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
		dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
		dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
		dstx[3] = 0.0f;

		dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
		dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
		dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
		dstx[7] = 0.0f;

		dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
		dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
		dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
		dstx[11] = 0.0f;

		dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
		dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
		dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
		dstx[15] = 1.0f;
	}

	// ------------
	// Constructors
	// ------------
	Matrix4f()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
		c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
	}

	explicit Matrix4f( Math::NoInitHint )
	{
		// Constructor without default initialization
	}

	Matrix4f( const float *floatArray16 )
	{
		for( unsigned int i = 0; i < 4; ++i )
		{
			for( unsigned int j = 0; j < 4; ++j )
			{
				c[i][j] = floatArray16[i * 4 + j];
			}
		}
	}

	Matrix4f( const Quaternion &q )
	{
		// Calculate coefficients
		float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
		float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
		float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
		float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

		c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;	
		c[2][0] = xz + wy;        c[3][0] = 0;
		c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);
		c[2][1] = yz - wx;        c[3][1] = 0;
		c[0][2] = xz - wy;        c[1][2] = yz + wx;
		c[2][2] = 1 - (xx + yy);  c[3][2] = 0;
		c[0][3] = 0;              c[1][3] = 0;
		c[2][3] = 0;              c[3][3] = 1;
	}

	// ----------
	// Matrix sum
	// ----------
	Matrix4f operator+( const Matrix4f &m ) const 
	{
		Matrix4f mf( Math::NO_INIT );
		
		mf.x[0] = x[0] + m.x[0];
		mf.x[1] = x[1] + m.x[1];
		mf.x[2] = x[2] + m.x[2];
		mf.x[3] = x[3] + m.x[3];
		mf.x[4] = x[4] + m.x[4];
		mf.x[5] = x[5] + m.x[5];
		mf.x[6] = x[6] + m.x[6];
		mf.x[7] = x[7] + m.x[7];
		mf.x[8] = x[8] + m.x[8];
		mf.x[9] = x[9] + m.x[9];
		mf.x[10] = x[10] + m.x[10];
		mf.x[11] = x[11] + m.x[11];
		mf.x[12] = x[12] + m.x[12];
		mf.x[13] = x[13] + m.x[13];
		mf.x[14] = x[14] + m.x[14];
		mf.x[15] = x[15] + m.x[15];

		return mf;
	}

	Matrix4f &operator+=( const Matrix4f &m )
	{
		return *this = *this + m;
	}

	// ---------------------
	// Matrix multiplication
	// ---------------------
	Matrix4f operator*( const Matrix4f &m ) const 
	{
		Matrix4f mf( Math::NO_INIT );
		
		mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
		mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
		mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
		mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];

		mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
		mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
		mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
		mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];

		mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
		mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
		mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
		mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];

		mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
		mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
		mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
		mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];

		return mf;
	}

	Matrix4f operator*( const float f ) const
	{
		Matrix4f m( *this );
		
		m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
		m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
		m.x[8]  *= f; m.x[9]  *= f; m.x[10] *= f; m.x[11] *= f;
		m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;

		return m;
	}

	// ----------------------------
	// Vector-Matrix multiplication
	// ----------------------------
	Vec3f operator*( const Vec3f &v ) const
	{
		return Vec3f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2] );
	}

	Vec4f operator*( const Vec4f &v ) const
	{
		return Vec4f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
		              v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3] );
	}

	Vec3f mult33Vec( const Vec3f &v ) const
	{
		return Vec3f( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
	}
	
	// ---------------
	// Transformations
	// ---------------
	void translate( const float x, const float y, const float z )
	{
		*this = TransMat( x, y, z ) * *this;
	}

	void scale( const float x, const float y, const float z )
	{
		*this = ScaleMat( x, y, z ) * *this;
	}

	void rotate( const float x, const float y, const float z )
	{
		*this = RotMat( x, y, z ) * *this;
	}

	// ---------------
	// Other
	// ---------------

	Matrix4f transposed() const
	{
		Matrix4f m( *this );
		
		for( unsigned int y = 0; y < 4; ++y )
		{
			for( unsigned int x = y + 1; x < 4; ++x ) 
			{
				float tmp = m.c[x][y];
				m.c[x][y] = m.c[y][x];
				m.c[y][x] = tmp;
			}
		}

		return m;
	}

	float determinant() const
	{
		return 
			c[0][3]*c[1][2]*c[2][1]*c[3][0] - c[0][2]*c[1][3]*c[2][1]*c[3][0] - c[0][3]*c[1][1]*c[2][2]*c[3][0] + c[0][1]*c[1][3]*c[2][2]*c[3][0] +
			c[0][2]*c[1][1]*c[2][3]*c[3][0] - c[0][1]*c[1][2]*c[2][3]*c[3][0] - c[0][3]*c[1][2]*c[2][0]*c[3][1] + c[0][2]*c[1][3]*c[2][0]*c[3][1] +
			c[0][3]*c[1][0]*c[2][2]*c[3][1] - c[0][0]*c[1][3]*c[2][2]*c[3][1] - c[0][2]*c[1][0]*c[2][3]*c[3][1] + c[0][0]*c[1][2]*c[2][3]*c[3][1] +
			c[0][3]*c[1][1]*c[2][0]*c[3][2] - c[0][1]*c[1][3]*c[2][0]*c[3][2] - c[0][3]*c[1][0]*c[2][1]*c[3][2] + c[0][0]*c[1][3]*c[2][1]*c[3][2] +
			c[0][1]*c[1][0]*c[2][3]*c[3][2] - c[0][0]*c[1][1]*c[2][3]*c[3][2] - c[0][2]*c[1][1]*c[2][0]*c[3][3] + c[0][1]*c[1][2]*c[2][0]*c[3][3] +
			c[0][2]*c[1][0]*c[2][1]*c[3][3] - c[0][0]*c[1][2]*c[2][1]*c[3][3] - c[0][1]*c[1][0]*c[2][2]*c[3][3] + c[0][0]*c[1][1]*c[2][2]*c[3][3];
	}

	Matrix4f inverted() const
	{
		Matrix4f m( Math::NO_INIT );

		float d = determinant();
		if( d == 0 ) return m;
		d = 1.0f / d;
		
		m.c[0][0] = d * (c[1][2]*c[2][3]*c[3][1] - c[1][3]*c[2][2]*c[3][1] + c[1][3]*c[2][1]*c[3][2] - c[1][1]*c[2][3]*c[3][2] - c[1][2]*c[2][1]*c[3][3] + c[1][1]*c[2][2]*c[3][3]);
		m.c[0][1] = d * (c[0][3]*c[2][2]*c[3][1] - c[0][2]*c[2][3]*c[3][1] - c[0][3]*c[2][1]*c[3][2] + c[0][1]*c[2][3]*c[3][2] + c[0][2]*c[2][1]*c[3][3] - c[0][1]*c[2][2]*c[3][3]);
		m.c[0][2] = d * (c[0][2]*c[1][3]*c[3][1] - c[0][3]*c[1][2]*c[3][1] + c[0][3]*c[1][1]*c[3][2] - c[0][1]*c[1][3]*c[3][2] - c[0][2]*c[1][1]*c[3][3] + c[0][1]*c[1][2]*c[3][3]);
		m.c[0][3] = d * (c[0][3]*c[1][2]*c[2][1] - c[0][2]*c[1][3]*c[2][1] - c[0][3]*c[1][1]*c[2][2] + c[0][1]*c[1][3]*c[2][2] + c[0][2]*c[1][1]*c[2][3] - c[0][1]*c[1][2]*c[2][3]);
		m.c[1][0] = d * (c[1][3]*c[2][2]*c[3][0] - c[1][2]*c[2][3]*c[3][0] - c[1][3]*c[2][0]*c[3][2] + c[1][0]*c[2][3]*c[3][2] + c[1][2]*c[2][0]*c[3][3] - c[1][0]*c[2][2]*c[3][3]);
		m.c[1][1] = d * (c[0][2]*c[2][3]*c[3][0] - c[0][3]*c[2][2]*c[3][0] + c[0][3]*c[2][0]*c[3][2] - c[0][0]*c[2][3]*c[3][2] - c[0][2]*c[2][0]*c[3][3] + c[0][0]*c[2][2]*c[3][3]);
		m.c[1][2] = d * (c[0][3]*c[1][2]*c[3][0] - c[0][2]*c[1][3]*c[3][0] - c[0][3]*c[1][0]*c[3][2] + c[0][0]*c[1][3]*c[3][2] + c[0][2]*c[1][0]*c[3][3] - c[0][0]*c[1][2]*c[3][3]);
		m.c[1][3] = d * (c[0][2]*c[1][3]*c[2][0] - c[0][3]*c[1][2]*c[2][0] + c[0][3]*c[1][0]*c[2][2] - c[0][0]*c[1][3]*c[2][2] - c[0][2]*c[1][0]*c[2][3] + c[0][0]*c[1][2]*c[2][3]);
		m.c[2][0] = d * (c[1][1]*c[2][3]*c[3][0] - c[1][3]*c[2][1]*c[3][0] + c[1][3]*c[2][0]*c[3][1] - c[1][0]*c[2][3]*c[3][1] - c[1][1]*c[2][0]*c[3][3] + c[1][0]*c[2][1]*c[3][3]);
		m.c[2][1] = d * (c[0][3]*c[2][1]*c[3][0] - c[0][1]*c[2][3]*c[3][0] - c[0][3]*c[2][0]*c[3][1] + c[0][0]*c[2][3]*c[3][1] + c[0][1]*c[2][0]*c[3][3] - c[0][0]*c[2][1]*c[3][3]);
		m.c[2][2] = d * (c[0][1]*c[1][3]*c[3][0] - c[0][3]*c[1][1]*c[3][0] + c[0][3]*c[1][0]*c[3][1] - c[0][0]*c[1][3]*c[3][1] - c[0][1]*c[1][0]*c[3][3] + c[0][0]*c[1][1]*c[3][3]);
		m.c[2][3] = d * (c[0][3]*c[1][1]*c[2][0] - c[0][1]*c[1][3]*c[2][0] - c[0][3]*c[1][0]*c[2][1] + c[0][0]*c[1][3]*c[2][1] + c[0][1]*c[1][0]*c[2][3] - c[0][0]*c[1][1]*c[2][3]);
		m.c[3][0] = d * (c[1][2]*c[2][1]*c[3][0] - c[1][1]*c[2][2]*c[3][0] - c[1][2]*c[2][0]*c[3][1] + c[1][0]*c[2][2]*c[3][1] + c[1][1]*c[2][0]*c[3][2] - c[1][0]*c[2][1]*c[3][2]);
		m.c[3][1] = d * (c[0][1]*c[2][2]*c[3][0] - c[0][2]*c[2][1]*c[3][0] + c[0][2]*c[2][0]*c[3][1] - c[0][0]*c[2][2]*c[3][1] - c[0][1]*c[2][0]*c[3][2] + c[0][0]*c[2][1]*c[3][2]);
		m.c[3][2] = d * (c[0][2]*c[1][1]*c[3][0] - c[0][1]*c[1][2]*c[3][0] - c[0][2]*c[1][0]*c[3][1] + c[0][0]*c[1][2]*c[3][1] + c[0][1]*c[1][0]*c[3][2] - c[0][0]*c[1][1]*c[3][2]);
		m.c[3][3] = d * (c[0][1]*c[1][2]*c[2][0] - c[0][2]*c[1][1]*c[2][0] + c[0][2]*c[1][0]*c[2][1] - c[0][0]*c[1][2]*c[2][1] - c[0][1]*c[1][0]*c[2][2] + c[0][0]*c[1][1]*c[2][2]);
		
		return m;
	}

	void decompose( Vec3f &trans, Vec3f &rot, Vec3f &scale ) const
	{
		// Getting translation is trivial
		trans = Vec3f( c[3][0], c[3][1], c[3][2] );

		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );

		if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;

		// Detect negative scale with determinant and flip one arbitrary axis
		if( determinant() < 0 ) scale.x = -scale.x;

		// Combined rotation matrix YXZ
		//
		// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]	
		// Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
		// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

		rot.x = asinf( -c[2][1] / scale.z );
		
		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		float f = fabsf( c[2][1] / scale.z );
		if( f > 0.999f && f < 1.001f )
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rot.y = 0;
			
			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rot.z = atan2f( -c[1][0] / scale.y, c[0][0] / scale.x );
		}
		// Standard case
		else
		{
			rot.y = atan2f( c[2][0] / scale.z, c[2][2] / scale.z );
			rot.z = atan2f( c[0][1] / scale.x, c[1][1] / scale.y );
		}
	}

	Vec4f getCol( unsigned int col ) const
	{
		return Vec4f( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
	}

	Vec4f getRow( unsigned int row ) const
	{
		return Vec4f( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
	}

	Vec3f getTrans() const
	{
		return Vec3f( c[3][0], c[3][1], c[3][2] );
	}
	
	Vec3f getScale() const
	{
		Vec3f scale;
		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
		return scale;
	}
};


// -------------------------------------------------------------------------------------------------
// Plane
// -------------------------------------------------------------------------------------------------

class Plane
{
public:
	Vec3f normal; 
	float dist;

	// ------------
	// Constructors
	// ------------
	Plane() 
	{ 
		normal.x = 0; normal.y = 0; normal.z = 0; dist = 0; 
	};

	explicit Plane( const float a, const float b, const float c, const float d )
	{
		normal = Vec3f( a, b, c );
		float invLen = 1.0f / normal.length();
		normal *= invLen;	// Normalize
		dist = d * invLen;
	}

	Plane( const Vec3f &v0, const Vec3f &v1, const Vec3f &v2 )
	{
		normal = v1 - v0;
		normal = normal.cross( v2 - v0 );
		normal.normalize();
		dist = -normal.dot( v0 );
	}

	// ----------------
	// Other operations
	// ----------------
	float distToPoint( const Vec3f &v ) const
	{
		return normal.dot( v ) + dist;
	}
};


// -------------------------------------------------------------------------------------------------
// Intersection
// -------------------------------------------------------------------------------------------------

static inline bool rayTriangleIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, 
                                     const Vec3f &vert0, const Vec3f &vert1, const Vec3f &vert2,
                                     Vec3f &intsPoint )
{
	// Idea: Tomas Moeller and Ben Trumbore
	// in Fast, Minimum Storage Ray/Triangle Intersection 
	
	// Find vectors for two edges sharing vert0
	Vec3f edge1 = vert1 - vert0;
	Vec3f edge2 = vert2 - vert0;

	// Begin calculating determinant - also used to calculate U parameter
	Vec3f pvec = rayDir.cross( edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	float det = edge1.dot( pvec );


	// *** Culling branch ***
	/*if( det < Math::Epsilon )return false;

	// Calculate distance from vert0 to ray origin
	Vec3f tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec );
	if (u < 0 || u > det ) return false;

	// Prepare to test V parameter
	Vec3f qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec );
	if (v < 0 || u + v > det ) return false;

	// Calculate t, scale parameters, ray intersects triangle
	float t = edge2.dot( qvec ) / det;*/


	// *** Non-culling branch ***
	if( det > -Math::Epsilon && det < Math::Epsilon ) return 0;
	float inv_det = 1.0f / det;

	// Calculate distance from vert0 to ray origin
	Vec3f tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec ) * inv_det;
	if( u < 0.0f || u > 1.0f ) return 0;

	// Prepare to test V parameter
	Vec3f qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec ) * inv_det;
	if( v < 0.0f || u + v > 1.0f ) return 0;

	// Calculate t, ray intersects triangle
	float t = edge2.dot( qvec ) * inv_det;


	// Calculate intersection point and test ray length and direction
	intsPoint = rayOrig + rayDir * t;
	Vec3f vec = intsPoint - rayOrig;
	if( vec.dot( rayDir ) < 0 || vec.length() > rayDir.length() ) return false;

	return true;
}


static inline bool rayAABBIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, 
                                 const Vec3f &mins, const Vec3f &maxs )
{
	// SLAB based optimized ray/AABB intersection routine
	// Idea taken from http://ompf.org/ray/
	
	float l1 = (mins.x - rayOrig.x) / rayDir.x;
	float l2 = (maxs.x - rayOrig.x) / rayDir.x;
	float lmin = minf( l1, l2 );
	float lmax = maxf( l1, l2 );

	l1 = (mins.y - rayOrig.y) / rayDir.y;
	l2 = (maxs.y - rayOrig.y) / rayDir.y;
	lmin = maxf( minf( l1, l2 ), lmin );
	lmax = minf( maxf( l1, l2 ), lmax );
		
	l1 = (mins.z - rayOrig.z) / rayDir.z;
	l2 = (maxs.z - rayOrig.z) / rayDir.z;
	lmin = maxf( minf( l1, l2 ), lmin );
	lmax = minf( maxf( l1, l2 ), lmax );

	if( (lmax >= 0.0f) & (lmax >= lmin) )
	{
		// Consider length
		const Vec3f rayDest = rayOrig + rayDir;
		Vec3f rayMins( minf( rayDest.x, rayOrig.x), minf( rayDest.y, rayOrig.y ), minf( rayDest.z, rayOrig.z ) );
		Vec3f rayMaxs( maxf( rayDest.x, rayOrig.x), maxf( rayDest.y, rayOrig.y ), maxf( rayDest.z, rayOrig.z ) );
		return 
			(rayMins.x < maxs.x) && (rayMaxs.x > mins.x) &&
			(rayMins.y < maxs.y) && (rayMaxs.y > mins.y) &&
			(rayMins.z < maxs.z) && (rayMaxs.z > mins.z);
	}
	else
		return false;
}


static inline float nearestDistToAABB( const Vec3f &pos, const Vec3f &mins, const Vec3f &maxs )
{
	const Vec3f center = (mins + maxs) * 0.5f;
	const Vec3f extent = (maxs - mins) * 0.5f;
	
	Vec3f nearestVec;
	nearestVec.x = maxf( 0, fabsf( pos.x - center.x ) - extent.x );
	nearestVec.y = maxf( 0, fabsf( pos.y - center.y ) - extent.y );
	nearestVec.z = maxf( 0, fabsf( pos.z - center.z ) - extent.z );
	
	return nearestVec.length();
}


// And some more additional functions
static inline Vec3f xnJointToVec3f(const XnSkeletonJointPosition& jointPos)
{
	return Vec3f(jointPos.position.X, jointPos.position.Y, jointPos.position.Z);
}

static inline void radToDeg( Vec3f& v ) 
{
	v *= 57.29577951f;
}
static inline void degToRad( Vec3f& v ) 
{
	v *= 0.017453293f;
}

static inline float distancePointToRay( const Vec3f& point, const Vec3f& rayOrigin, const Vec3f& rayDir, bool withinLineSegmentOnly = false)
{
	float rayLength = rayDir.length();

	if (rayLength == 0)
		return -1.f;
 
	float u = (point - rayOrigin).dot(rayDir) / ( rayLength * rayLength );
 
    if( withinLineSegmentOnly && (u < 0.0f || u > 1.0f))
        return -1.f;   // closest point does not fall within the line segment
 
	Vec3f intersect;
    intersect.x = rayOrigin.x + u * rayDir.x;
    intersect.y = rayOrigin.y + u * rayDir.y;
    intersect.z = rayOrigin.z + u * rayDir.z;
 
    return (point - intersect).length();
}

// Transform a rotation matrix (3x3 = 9) to a rotation vector (in degree)
static inline Vec3f rotMatToRotation(const float* mat, bool inDegree = true)
{
	Vec3f rot;

    rot.x = sinf(-mat[7]);

    // Special case: Cos[x] == 0 (when Sin[x] is +/-1)
    float f = fabsf(mat[7]);
    if (f > 0.999f && f < 1.001f)
    {
        // Pin arbitrarily one of y or z to zero
        // Mathematical equivalent of gimbal lock
        rot.y = 0;

        // Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
        // => m[0][0] = Cos[z] and m[1][0] = Sin[z]
        rot.z = atan2f(-mat[3], mat[0]);
    }
    // Standard case
    else
    {
        rot.y = atan2f(mat[6], mat[8]);
        rot.z = atan2f(mat[1], mat[4]);
    }

	if (inDegree)
		radToDeg(rot);

	return rot;
}

static inline void calculateLocalPosition(const XnSkeletonJointPosition& absPos, const XnSkeletonJointPosition& absParentPos, XnSkeletonJointPosition& dstPos)
{
	Vec3f localPos = xnJointToVec3f(absPos) - xnJointToVec3f(absParentPos);
	dstPos.position.X = localPos.x;
	dstPos.position.Y = localPos.y;
	dstPos.position.Z = localPos.z;
	dstPos.fConfidence = minf(absPos.fConfidence, absParentPos.fConfidence);
}

static inline void calculateLocalRotMat(const float* absRotMat, const float* absParentMat, float* dstMat)
{
	Fubi::Matrix3f matLocal = Fubi::Matrix3f(absRotMat) * Fubi::Matrix3f(absParentMat).inverted();
	for (int i = 0; i< 9; i++)
	{
		dstMat[i] = matLocal.x[i];
	}
}

static inline void calculateLocalRotation(const XnSkeletonJointOrientation& absRot, const XnSkeletonJointOrientation& absParentRot, XnSkeletonJointOrientation& dstRot)
{
	calculateLocalRotMat(absRot.orientation.elements, absParentRot.orientation.elements, dstRot.orientation.elements);
	dstRot.fConfidence = minf(absRot.fConfidence, absParentRot.fConfidence);
}


static const Vec3f DefaultMinVec = Vec3f(-Math::MaxFloat, -Math::MaxFloat, -Math::MaxFloat);
static const Vec3f DefaultMaxVec = Vec3f(Math::MaxFloat, Math::MaxFloat, Math::MaxFloat);

// Copy the three orientation columns to the right position in the orientation matrix
static void orientationVectorsToXnRotMat(XnSkeletonJointOrientation &jointOrientation, const Vec3f& xCol,const Vec3f& yCol, const Vec3f& zCol)
{
	jointOrientation.orientation.elements[0] = xCol.x;
	jointOrientation.orientation.elements[3] = xCol.y;
	jointOrientation.orientation.elements[6] = xCol.z;

	jointOrientation.orientation.elements[1] = yCol.x;
	jointOrientation.orientation.elements[4] = yCol.y;
	jointOrientation.orientation.elements[7] = yCol.z;

	jointOrientation.orientation.elements[2] = zCol.x;
	jointOrientation.orientation.elements[5] = zCol.y;
	jointOrientation.orientation.elements[8] = zCol.z;
}

// Calculate orientation columns from a vector specifying the x axis
static void orientVectorsFromVecX(const Vec3f& x, XnSkeletonJointOrientation &jointOrientation)
{		
	// Columnn vectors (initialized with 0,0,0)
	Vec3f xCol;
	Vec3f yCol;
	Vec3f zCol;

	// x vector is used directly
	xCol = x.normalized();

	// y vector is set to be orthogonal to x, and pointing in parallel to the y-axis if the x vector is pointing in parallel to the x axis
	if (xCol.y != 0 || xCol.x != 0)
	{
		yCol.x = -xCol.y;
		yCol.y = xCol.x;
		yCol.z = 0;
		yCol.normalize();
	}
	else
	{
		yCol.y = 1.0f;
	}

	// z vector can now be calculated as the cross product of the others
	zCol = xCol.cross(yCol);

	// Now copy the values into matrix
	orientationVectorsToXnRotMat(jointOrientation, xCol, yCol, zCol);
}

// Calculate orientation columns from a vector specifying the y axis
static void orientVectorsFromVecY(const Vec3f& v1, XnSkeletonJointOrientation &jointOrientation)
{		
	// Columnn vectors (initialized with 0,0,0)
	Vec3f xCol;
	Vec3f yCol;
	Vec3f zCol;


	// y vector is used directly
	yCol = v1.normalized();

	// x vector is set to be orthogonal to y, and pointing in parallel to the x-axis if the y vector is pointing in parallel to the y axis
	if (yCol.x != 0 || yCol.y != 0)
	{
		xCol.x = yCol.y;
		xCol.y = -yCol.x;
		xCol.z = 0.0f;
		xCol.normalize();
	}
	else
	{
		xCol.x = 1.0f;
	}

	// z vector can now be calculated as the cross product of the others
	zCol = xCol.cross(yCol);

	// Now copy the values into matrix
	orientationVectorsToXnRotMat(jointOrientation, xCol, yCol, zCol);
}	

	
// Calculate orientation columns from a vector specifying the y axis and one specifying the x axis
// y axis is used directly, the rest is calculated according to cross products
static void orientVectorsFromVecYX(const Vec3f& yUnnormalized, const Vec3f& xUnnormalized, XnSkeletonJointOrientation &jointOrientation)
{		
	// Columnn vectors (initialized with 0,0,0)
	Vec3f xCol;
	Vec3f yCol;
	Vec3f zCol;

	// y vector is used directly
	yCol = yUnnormalized.normalized();
	// z vector is calculated as the cross product of x and y
	zCol = xUnnormalized.normalized().cross(yCol).normalized();
	// x vector is again calculated as the cross product of y and z (may be different from given x vector)
	xCol = yCol.cross(zCol);

	//copy values into matrix
	orientationVectorsToXnRotMat(jointOrientation, xCol, yCol, zCol);
}

// Calculate xnJointOrientation from a vector specifying the y axis and one specifying the x axis
// y axis is used directly, the rest is calculated according to cross products
static void xnJointOrientationFromPositionsYX(const XnSkeletonJointPosition& yStart, const XnSkeletonJointPosition& yEnd,
	const XnSkeletonJointPosition& xStart, const XnSkeletonJointPosition& xEnd, XnSkeletonJointOrientation &jointOrientation)
{
	float xConf = (xStart.fConfidence + xEnd.fConfidence) / 2.0f;
	float yConf = (yStart.fConfidence + yEnd.fConfidence) / 2.0f;
	jointOrientation.fConfidence = (xConf + yConf) / 2.0f;

	if (xConf > 0 && yConf > 0)
	{
		Vec3f vx = xnJointToVec3f(xEnd)-xnJointToVec3f(xStart);
		Vec3f vy = xnJointToVec3f(yEnd)-xnJointToVec3f(yStart);
		orientVectorsFromVecYX(vy, vx, jointOrientation);
	}
	else if (xConf > 0)
	{
		Vec3f vx = xnJointToVec3f(xEnd)-xnJointToVec3f(xStart);
		orientVectorsFromVecX(vx, jointOrientation);
	}
	else if (yConf > 0)
	{
		Vec3f vy = xnJointToVec3f(yEnd)-xnJointToVec3f(yStart);
		orientVectorsFromVecY(vy, jointOrientation);
	}
}

// Calculate xnJointOrientation from a vector specifying the y axis
static void xnJointOrientationFromPositionY(const XnSkeletonJointPosition& yStart, const XnSkeletonJointPosition& yEnd, XnSkeletonJointOrientation &jointOrientation)
{
	jointOrientation.fConfidence = (yStart.fConfidence + yEnd.fConfidence) / 2.0f;

	if (jointOrientation.fConfidence > 0)
	{
		Vec3f vy = xnJointToVec3f(yEnd)-xnJointToVec3f(yStart);
		orientVectorsFromVecY(vy, jointOrientation);
	}
}

// Calculate xnJointOrientation from a vector specifying the x axis
static void xnJointOrientationFromPositionX(const XnSkeletonJointPosition& xStart, const XnSkeletonJointPosition& xEnd,
	XnSkeletonJointOrientation &jointOrientation)
{
	jointOrientation.fConfidence = (xStart.fConfidence + xEnd.fConfidence) / 2.0f;
	if (jointOrientation.fConfidence > 0)
	{
		Vec3f vx = xnJointToVec3f(xEnd)-xnJointToVec3f(xStart);
		orientVectorsFromVecX(vx, jointOrientation);
	}
}

};
