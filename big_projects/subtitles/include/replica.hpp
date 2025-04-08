#ifndef REPLICA_HPP
#define REPLICA_HPP

#include <string>
typedef std::string string;

class Replica
{
	int m_number;
	string info;
	int m_conv_start_time;
	int m_conv_end_time;
	string m_start_time;
	string m_end_time;

  public:
	Replica();

	void setNumber(int number);
	[[nodiscard]] int getNumber() const;

	void setInfo(const string &i);
	[[nodiscard]] string getInfo() const;

	void setConvStartTime(int start);
	[[nodiscard]] int getConvStartTime() const;

	void setConvEndTime(int end);
	[[nodiscard]] int getConvEndTime() const;

	void setStartTime(const string &s);
	[[nodiscard]] string getStartTime() const;

	void setEndTime(const string &s);
	[[nodiscard]] string getEndTime() const;

	void addInfo(const string &s);
};

#endif	  // REPLICA_HPP
