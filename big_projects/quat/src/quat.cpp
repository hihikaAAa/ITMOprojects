#include "../include/quat.h"

#include <cmath>
#include <vector>

vector3_t::vector3_t(const float x, const float y, const float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

float Quat::deg_2(const float x)
{
	return x * x;
}

Quat::Quat()
{
	m_a = 0;
	m_b = 0;
	m_c = 0;
	m_d = 0;
}

Quat::Quat(const float a, const float b, const float c, const float d)
{
	m_a = a;
	m_b = b;
	m_c = c;
	m_d = d;
}

Quat::Quat(float T, const bool angle, const vector3_t axis)
{
	if (!angle)
	{
		T = T * static_cast< float >(M_PI) / 180.0f;
	}
	const float half_T = T / 2.0f;
	float norm = std::sqrt(axis.m_x * axis.m_x + axis.m_y * axis.m_y + axis.m_z * axis.m_z);
	if (norm == 0.0f)
	{
		m_a = m_b = m_c = m_d = 0;
	}
	else
	{
		const float ux = axis.m_x / norm;
		const float uy = axis.m_y / norm;
		const float uz = axis.m_z / norm;

		m_a = std::cos(half_T);
		float s = std::sin(half_T);
		m_b = ux * s;
		m_c = uy * s;
		m_d = uz * s;
	}
}

Quat::Quat(const Quat &other)
{
	m_a = other.m_a;
	m_b = other.m_b;
	m_c = other.m_c;
	m_d = other.m_d;
}

Quat &Quat::operator+=(const Quat &other)
{
	m_a += other.m_a;
	m_b += other.m_b;
	m_c += other.m_c;
	m_d += other.m_d;
	return *this;
}

Quat &Quat::operator-=(const Quat &other)
{
	m_a -= other.m_a;
	m_b -= other.m_b;
	m_c -= other.m_c;
	m_d -= other.m_d;
	return *this;
}

Quat Quat::operator+(const Quat &other) const
{
	Quat result = *this;
	result += other;
	return result;
}

Quat Quat::operator-(const Quat &other) const
{
	Quat result = *this;
	result -= other;
	return result;
}

Quat &Quat::operator*=(const Quat &other)
{
	const float a = m_a, b = m_b, c = m_c, d = m_d;
	m_a = a * other.m_a - b * other.m_b - c * other.m_c - d * other.m_d;
	m_b = a * other.m_b + b * other.m_a + c * other.m_d - d * other.m_c;
	m_c = a * other.m_c - b * other.m_d + c * other.m_a + d * other.m_b;
	m_d = a * other.m_d + b * other.m_c - c * other.m_b + d * other.m_a;
	return *this;
}

Quat Quat::operator*(const Quat &other) const
{
	Quat result = *this;
	result *= other;
	return result;
}

bool Quat::operator==(const Quat &other) const
{
	return m_a == other.m_a && m_b == other.m_b && m_c == other.m_c && m_d == other.m_d;
}

bool Quat::operator!=(const Quat &other) const
{
	return !(*this == other);
}

Quat &Quat::operator*=(const float scalar)
{
	m_a *= scalar;
	m_b *= scalar;
	m_c *= scalar;
	m_d *= scalar;
	return *this;
}

Quat Quat::operator*(const float scalar) const
{
	Quat result = *this;
	result *= scalar;
	return result;
}

Quat Quat::operator~() const
{
	return Quat(m_a, -m_b, -m_c, -m_d);
}

Quat::operator float() const
{
	return std::sqrt(m_a * m_a + m_b * m_b + m_c * m_c + m_d * m_d);
}

Quat Quat::operator*(const vector3_t &v) const
{
	const Quat qv(0, v.m_x, v.m_y, v.m_z);
	Quat result = (*this) * qv * (~(*this));
	return result;
}

std::vector< float > Quat::data() const
{
	return { m_b, m_c, m_d, m_a };
}

std::vector< float > Quat::rotation_matrix() const
{
	return { 1 - 2 * (deg_2(m_c) + deg_2(m_d)), 2 * (m_b * m_c - m_a * m_d),	   2 * (m_b * m_d + m_a * m_c),
			 2 * (m_b * m_c + m_a * m_d),		1 - 2 * (deg_2(m_b) + deg_2(m_d)), 2 * (m_c * m_d - m_a * m_b),
			 2 * (m_b * m_d - m_a * m_c),		2 * (m_c * m_d + m_a * m_b),	   1 - 2 * (deg_2(m_b) + deg_2(m_c)) };
}

std::vector< float > Quat::matrix() const
{
	return { m_a, -m_b, -m_c, -m_d, m_b, m_a, -m_d, m_c, m_c, m_d, m_a, -m_b, m_d, -m_c, m_b, m_a };
}
