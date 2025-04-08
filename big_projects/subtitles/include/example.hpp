#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include "my_vector.hpp"
#include "replica.hpp"
#include "styler.hpp"

#include <regex>
#include <string>

template< typename T >
using vector = Vector< T >;
typedef std::regex regex;
typedef std::smatch smatch;
typedef std::string string;

class Subtitles
{
  public:
	Styler my_style;

	virtual ~Subtitles() = default;

	virtual void read(std::fstream &input_file) = 0;

	virtual void write(std::fstream &output_file, int writing_indx) = 0;

	virtual void change_delta(int format, std::string delta) = 0;

	[[nodiscard]] virtual Subtitles *find_collisions() const = 0;

	virtual void removeFormatting(std::fstream &input_file) = 0;

	virtual void set_style() = 0;
};

class SrtSubtitles final : public Subtitles
{
  public:
	void read(std::fstream &input_file) override;

	void write(std::fstream &output_file, int writing_indx) override;

	void change_delta(int format, std::string delta) override;

	[[nodiscard]] Subtitles *find_collisions() const override;

	void removeFormatting(std::fstream &input_file) override;

	void set_style() override;

  private:
	vector< Replica > replicas;

	static int delta_breaker(const std::string &delta);
};

class SmiSubtitles final : public Subtitles
{
  public:
	void read(std::fstream &input_file) override;

	void write(std::fstream &output_file, int writing_indx) override;

	void change_delta(int format, std::string delta) override;

	[[nodiscard]] Subtitles *find_collisions() const override;

	void removeFormatting(std::fstream &input_file) override;

	void set_style() override;

  private:
	vector< Replica > replicas;

	static int delta_breaker(const std::string &delta);
};

class AssSubtitles final : public Subtitles
{
  public:
	void read(std::fstream &input_file) override;

	void write(std::fstream &output_file, int writing_indx) override;

	void change_delta(int format, std::string delta) override;

	[[nodiscard]] Subtitles *find_collisions() const override;

	void removeFormatting(std::fstream &input_file) override;

	void set_style() override;

  private:
	vector< Replica > replicas;

	static int delta_breaker(const std::string &delta);

	static string resetAssStyles(const string &line);
};

class EncoreSubtitles final : public Subtitles
{
  public:
	void read(std::fstream &input_file) override;

	void write(std::fstream &output_file, int writing_indx) override;

	void change_delta(int format, std::string delta) override;

	[[nodiscard]] Subtitles *find_collisions() const override;

	void removeFormatting(std::fstream &input_file) override;

	void set_style() override;

  private:
	vector< Replica > replicas;

	static int delta_breaker(const std::string &delta);
};
#endif	  // EXAMPLE_HPP
