#ifndef STYLER_HPP
#define STYLER_HPP
#include <string>
typedef std::string string;

class Styler
{
	string m_font_name;
	int m_font_size;
	int m_primary_color;
	int m_outline;
	int m_shadow;
	int m_alignment;

  public:
	Styler();
	explicit Styler(const string &f_n, int f_s, int p_c, int m_o, int m_s, int m_a);
	[[nodiscard]] string get_font_name() const;
	[[nodiscard]] int get_primary_color() const;
	[[nodiscard]] int get_font_size() const;
	[[nodiscard]] int get_outline() const;
	[[nodiscard]] int get_shadow() const;
	[[nodiscard]] int get_alignment() const;
};
#endif	  // STYLER_HPP
