#include "../include/replica.hpp"

Replica::Replica()
{
	m_number = 0;
	m_conv_start_time = 0;
	m_conv_end_time = 0;
	info = "";
	m_start_time = "";
	m_end_time = "";
}

void Replica::setNumber(int number)
{
	m_number = number;
}

int Replica::getNumber() const
{
	return m_number;
}

void Replica::setInfo(const string &i)
{
	info = i;
}

string Replica::getInfo() const
{
	return info;
}

void Replica::setConvStartTime(int start)
{
	m_conv_start_time = start;
}

int Replica::getConvStartTime() const
{
	return m_conv_start_time;
}

void Replica::setConvEndTime(int end)
{
	m_conv_end_time = end;
}

int Replica::getConvEndTime() const
{
	return m_conv_end_time;
}

void Replica::setStartTime(const string &s)
{
	m_start_time = s;
}

string Replica::getStartTime() const
{
	return m_start_time;
}

void Replica::setEndTime(const string &s)
{
	m_end_time = s;
}

string Replica::getEndTime() const
{
	return m_end_time;
}
void Replica::addInfo(const string &s)
{
	string new_str = info + "\n" + s;
	info = new_str;
}
