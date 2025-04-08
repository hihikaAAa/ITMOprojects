#ifndef QUAT_H
#define QUAT_H

#include <vector>

struct vector3_t
{
	float m_x;
	float m_y;
	float m_z;

	explicit vector3_t(float x, float y, float z);
};

class Quat
{
	float m_a, m_b, m_c, m_d;

	static float deg_2(float x);

  public:
	Quat();

	explicit Quat(float a, float b, float c, float d);

	explicit Quat(float T, bool angle, vector3_t axis);

	Quat(const Quat &other);

	Quat &operator+=(const Quat &other);

	Quat &operator-=(const Quat &other);

	Quat operator+(const Quat &other) const;

	Quat operator-(const Quat &other) const;

	Quat &operator*=(const Quat &other);

	Quat operator*(const Quat &other) const;

	Quat &operator*=(float scalar);

	Quat operator*(float scalar) const;

	bool operator==(const Quat &other) const;

	bool operator!=(const Quat &other) const;

	Quat operator~() const;

	explicit operator float() const;

	Quat operator*(const vector3_t &v) const;

	std::vector< float > data() const;

	std::vector< float > rotation_matrix() const;

	std::vector< float > matrix() const;
};

#endif	  // QUAT_H
