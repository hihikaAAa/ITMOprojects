#include "../include/styler.hpp"

#include <string>

Styler::Styler()
{
	m_font_name = "Arial";
	m_font_size = 48;
	m_primary_color = 16777215;
	m_outline = 2;
	m_shadow = 2;
	m_alignment = 2;
}

Styler::Styler(const std::string &f_n, const int f_s, const int p_c, const int m_o, const int m_s, const int m_a)
{
	m_font_name = f_n;
	m_font_size = f_s;
	m_primary_color = p_c;
	m_outline = m_o;
	m_shadow = m_s;
	m_alignment = m_a;
}

std::string Styler::get_font_name() const
{
	return m_font_name;
}

int Styler::get_font_size() const
{
	return m_font_size;
}

int Styler::get_primary_color() const
{
	return m_primary_color;
}

int Styler::get_outline() const
{
	return m_outline;
}

int Styler::get_shadow() const
{
	return m_shadow;
}

int Styler::get_alignment() const
{
	return m_alignment;
}
