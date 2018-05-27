/**
* @class Structs
* @brief File containing all structs
* @author Jack Matters
*
* @date 22/05/2017
* @version 1.0	Initial start.
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <vector>

/// Struct to hold all of a model types data (positions, scales, filePath to load)
struct ModelsData
{
	std::string filePath;
	std::string texFilePath;
	std::vector<std::vector<float>> modelPositions;
	std::vector<std::vector<float>> modelScales;
};

/// Struct to hold all of heightmaps data (positions, scales, size, filePath to load and texture)
struct HeightmapsData
{
	std::string filePath;
	std::string texFilePath;
	int fileSize;
	std::vector<float> modelPositions;
	std::vector<float> modelScales;
};



/// Contains all the operations required by Vector3 variables
/// Taken from old assignment
///TODO: Fix comments
struct Vector3
{
public:
	//x-axis value
	float x;
	//y-axis value
	float y;
	//z-axis value
	float z;

	/**
	* @brief A contructor to define a 3D vector.
	* @param x - x-axis value
	* @param y - y-axis value
	* @param z - z-axis value
	*/
	Vector3(float x, float y, float z) {
		Vector3::x = x;
		Vector3::y = y;
		Vector3::z = z;
	};

	/**
	* @brief A default constructor to set the
	* vector's variables to 0.
	*/
	Vector3() { x = 0; y = 0; z = 0; };

	/**
	* @brief A constructor to set all variables to the parameter's value.
	* @param num - the number to be set to.
	*/
	Vector3(float num) {
		x = num;
		y = num;
		z = num;
	}

	/**
	* @brief Overloads the equality operator to copy vectors.
	* @param A vector to be copied from.
	* @return a bew vector
	*/
	Vector3 operator = (const Vector3 original) {
		x = original.x;
		y = original.y;
		z = original.z;
		return Vector3(x, y, z);
	};

	/**
	* @brief Returns the dot product of two vectors.
	* @param one - the first vector
	* @param two - the second vector
	* @return a scalar value
	*/
	static float Dot(Vector3 one, Vector3 two) {
		return one.x*two.x + one.y*two.y + one.z *two.z;
	}

	/**
	* @brief Uses IO to display one vector.
	* @param os - reference to an output stream
	* @param vector - a vector to be displayed.
	*/
	friend std::ostream & operator <<(std::ostream & os, const Vector3 & vector) {
		os << vector.x << "  " << vector.y << "  " << vector.z;
		return os;
	}

	/**
	* @brief The normal vector of a face.
	* @param a - the first point
	* @param b - the second point
	* @param c - the third point
	* @return Normal vector
	*/
	static Vector3 Normal(Vector3 a, Vector3 b, Vector3 c) {
		return Cross(a - b, c - b);
	}

	/**
	* @brief The cross product of two vectors.
	* @param first - the first vector
	* @param second - the second vector
	* @return the cross product
	*/
	static Vector3 Cross(const Vector3 first, const Vector3 second) {
		return Vector3(first.y*second.z - first.z*second.y, first.z*second.x - first.x*second.z, first.x*second.y - first.y*second.x);
	}

	/**
	* @brief Calculates the distance between to 3D points.
	* @param first - the first point
	* @param second - the second point
	* @return the distance between the points.
	*/
	static float Distance(const Vector3 first, const Vector3 second) {
		return sqrt((second.x - first.x)*(second.x - first.x) +
			(second.y - first.y)*(second.y - first.y) +
			(second.z - first.z)*(second.z - first.z));
	}

	/*
	* @brief Multiplies this vector by the parameter.
	* @param the scalar
	* @return the result of multiplication.
	*/
	Vector3 operator * (const float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return Vector3(x, y, z);
	};

	/*
	* @brief Adds two vectors together.
	* @param second - a vector which has to be added
	* @return the result of addition.
	*/
	Vector3 operator + (const Vector3 second) {
		this->x += second.x;
		this->y += second.y;
		this->z += second.z;
		return *this;
	}

	/*
	* @brief Subtracts the parameter from this vector.
	* @param second - a vector which has to be subtracted from this vector.
	* @return the result of subtraction.
	*/
	Vector3 operator - (const Vector3 second) {
		this->x -= second.x;
		this->y -= second.y;
		this->z -= second.z;
		return *this;
	}

	/**
	* @brief An overlaoded array access operator to get one of the vector's values.
	* @param index - 0 - x value, 1 - y value, 2 - z value
	* @return the value of a particular variable or -1000000
	*/
	float operator[](const int index)  const {
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
		else if (index == 2)
			return z;
		else
			return -10000000;
	}

	/**
	* @brief Changes one of the variables of this vector.
	* @param index - 0 - x value, 1 - y value, 2 - z value
	* @param value - a new value to be assigned
	*/
	void Set(int index, float value) {
		if (index == 0)
			x = value;
		else if (index == 1)
			y = value;
		else if (index == 2)
			z = value;
	}

	/**
	* @brief Divides this vector by a scalar
	* @param a float to be divided by
	* @return the result of division
	*
	*/
	Vector3 operator / (const float copy) {
		x /= copy;
		y /= copy;
		z /= copy;
		return Vector3(x, y, z);
	}

	/*
	* @brief Calculates the length of this vector.
	* @return the length of the vector.
	*/
	float Length() {
		return sqrt((x*x) + (y*y) + (z*z));
	}

	/**
	* @brief Calculates a normalized version of the vector.
	* @return the normalized vector.
	*/
	Vector3 Normalized() const {
		float length = Vector3(x, y, z).Length();
		if (length == 0)
			return Vector3(0);
		else {
			return Vector3(x / length, y / length, z / length);
		}
	}
};
#endif