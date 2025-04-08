#include "../include/example.hpp"

#include "../include/replica.hpp"
#include "../include/styler.hpp"

#include <fstream>
#include <iostream>

void SrtSubtitles::read(std::fstream &input_file)
{
	string line;
	bool firstLine = true;
	while (std::getline(input_file, line))
	{
		if (firstLine)
		{
			if (line.size() >= 3 && static_cast< unsigned char >(line[0]) == 0xEF &&
				static_cast< unsigned char >(line[1]) == 0xBB && static_cast< unsigned char >(line[2]) == 0xBF)
			{
				line.erase(0, 3);
			}
			firstLine = false;
		}
		if (line.empty())
			continue;

		Replica replica;
		replica.setNumber(std::stoi(line));

		string time_line;
		if (!std::getline(input_file, time_line))
			break;

		regex pattern(R"((\d{2}):(\d{2}):(\d{2})\,(\d{3})\s*-->\s*(\d{2}):(\d{2}):(\d{2})\,(\d{3}))");
		smatch match;
		if (regex_search(time_line, match, pattern))
		{
			replica.setStartTime(time_line.substr(0, time_line.find(' ')));
			replica.setEndTime(time_line.substr(time_line.rfind(' ') + 1));
			string start_time = match[1].str() + match[2].str() + match[3].str() + match[4].str();
			string end_time = match[5].str() + match[6].str() + match[7].str() + match[8].str();
			replica.setConvStartTime(std::stoi(start_time));
			replica.setConvEndTime(std::stoi(end_time));
		}

		string text_line;
		while (true)
		{
			if (!std::getline(input_file, text_line))
				break;
			if (text_line.empty())
				break;
			if (replica.getInfo().empty())
			{
				replica.setInfo(text_line);
			}
			else
			{
				replica.addInfo(text_line);
			}
		}
		replicas.push_back(replica);
	}
}

void SrtSubtitles::write(std::fstream &output_file, const int writing_indx)
{
	switch (writing_indx)
	{
	case 1:
		for (const auto &replica : replicas)
		{
			output_file << replica.getNumber() << std::endl;
			output_file << replica.getStartTime() << " --> " << replica.getEndTime() << std::endl;
			output_file << replica.getInfo() << std::endl;
			output_file << std::endl;
		}
		break;
	case 2:
		output_file << "<SAMI>" << std::endl;
		output_file << "<BODY>" << std::endl;
		for (const auto &replica : replicas)
		{
			output_file << "<SYNC Start= " << replica.getConvStartTime() << " End=" << replica.getConvEndTime()
						<< "><P>" << replica.getInfo() << "</P></SYNC>";
		}
		output_file << "</BODY>" << std::endl;
		output_file << "</SAMI>" << std::endl;
		break;
	case 3:
	{
		output_file << "[Script Info]\n";
		output_file << "; Script generated by SrtSubtitles\n";
		output_file << "ScriptType: v4.00+\n";
		output_file << "WrapStyle: 0\n";
		output_file << "ScaledBorderAndShadow: yes\n";
		output_file << "YCbCr Matrix: None\n\n";

		output_file << "[V4+ Styles]\n";
		output_file
			<< "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, "
			   "Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, "
			   "Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n";

		const int r = my_style.get_primary_color() & 0xFF;
		const int g = (my_style.get_primary_color() >> 8) & 0xFF;
		const int b = (my_style.get_primary_color() >> 16) & 0xFF;
		char assColor[16];
		std::snprintf(assColor, sizeof(assColor), "&H%02X%02X%02X%02X", 0, b, g, r);
		output_file
			<< "Style: Default," << my_style.get_font_name() << "," << my_style.get_font_size() << "," << assColor << ","
			<< "&H000000FF,"
			<< "&H00000000,"
			<< "&H00000000,"
			<< "0,0,0,0,"
			<< "100,100,0,0,"
			<< "1," << my_style.get_outline() << "," << my_style.get_shadow() << "," << my_style.get_alignment() << ",10,10,10,0\n\n";
		output_file << "[Events]\n";
		output_file << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n";

		auto formatAssTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%d:%02d:%02d.%02d", h, m, s, cs);
			return buffer;
		};

		for (const auto &replica : replicas)
		{
			string start_ass = formatAssTime(replica.getConvStartTime());
			string end_ass = formatAssTime(replica.getConvEndTime());
			output_file << "Dialogue: 0," << start_ass << "," << end_ass << ",Default,,0,0,0,," << replica.getInfo() << "\n";
		}
		break;
	}
	case 4:
	{
		auto formatEncoreTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d:%02d", h, m, s, cs);
			return buffer;
		};
		for (const auto &replica : replicas)
		{
			output_file << replica.getNumber() << ' ' << formatEncoreTime(replica.getConvStartTime()) << ' '
						<< formatEncoreTime(replica.getConvEndTime()) << ' ' << replica.getInfo() << std::endl;
		}
	}
	default:
	{
		break;
	}
	}
}

int SrtSubtitles::delta_breaker(const string &delta)
{
	const regex delta_pattern(R"((\d{2}):(\d{2}):(\d{2}),(\d{3}))");
	smatch delta_match;
	int delta_ms = 0;
	if (regex_match(delta, delta_match, delta_pattern))
	{
		const int dh = std::stoi(delta_match[1].str());
		const int dm = std::stoi(delta_match[2].str());
		const int ds = std::stoi(delta_match[3].str());
		const int dms = std::stoi(delta_match[4].str());
		delta_ms = dh * 3600000 + dm * 60000 + ds * 1000 + dms;
	}
	else
	{
		return -1;
	}
	return delta_ms;
}

void SrtSubtitles::change_delta(const int format, const string delta)
{
	const int delta_ms = delta_breaker(delta);
	if (delta_ms == -1)
	{
		std::cerr << "Неверный формат delta: " << delta << std::endl;
		return;
	}

	auto formatTime = [](int total_ms) -> string
	{
		const int h = total_ms / 3600000;
		total_ms %= 3600000;
		const int m = total_ms / 60000;
		total_ms %= 60000;
		const int s = total_ms / 1000;
		const int ms = total_ms % 1000;
		char buffer[32];
		std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d,%03d", h, m, s, ms);
		return buffer;
	};

	for (auto &replica : replicas)
	{
		switch (format)
		{
		case 1:
			replica.setConvStartTime(replica.getConvStartTime() + delta_ms);
			replica.setConvEndTime(replica.getConvEndTime() + delta_ms);
			replica.setStartTime(formatTime(replica.getConvStartTime()));
			replica.setEndTime(formatTime(replica.getConvEndTime()));
			break;
		case 2:
			replica.setConvStartTime(replica.getConvStartTime() + delta_ms);
			replica.setStartTime(formatTime(replica.getConvStartTime()));
			break;
		case 3:
			replica.setConvEndTime(replica.getConvEndTime() + delta_ms);
			replica.setEndTime(formatTime(replica.getConvEndTime()));
			break;
		default:
			break;
		}
	}
	std::cout << "Дельта изменена.\n";
}

Subtitles *SrtSubtitles::find_collisions() const
{
	auto *collisions = new SrtSubtitles();
	for (size_t i = 0; i + 1 < replicas.size(); i++)
	{
		const int startA = replicas[i].getConvStartTime();
		const int endA = replicas[i].getConvEndTime();
		const int startB = replicas[i + 1].getConvStartTime();
		const int endB = replicas[i + 1].getConvEndTime();
		if (startB <= endA && startA <= endB)
		{
			collisions->replicas.push_back(replicas[i]);
			collisions->replicas.push_back(replicas[i + 1]);
		}
	}
	return collisions;
}

void SrtSubtitles::removeFormatting(std::fstream &input_file)
{
	string line;
	vector< string > lines;
	while (std::getline(input_file, line))
	{
		line = regex_replace(line, std::regex(R"(<\\[biu]>)", std::regex_constants::icase), "");
		line = regex_replace(line, std::regex(R"(<\/\\[biu]>)", std::regex_constants::icase), "");
		line = regex_replace(line, std::regex(R"(<[^>]+>)", std::regex_constants::icase), "");
		lines.push_back(line);
	}
	input_file.clear();
	input_file.seekp(0, std::ios::beg);
	for (const auto &n_line : lines)
	{
		input_file << n_line << std::endl;
	}
	input_file.seekg(0, std::ios::beg);
}

void SrtSubtitles::set_style()
{
	my_style = Styler();
}

//---------------------------------------------------------------------------------------------------------------------------------
// SAMI -> other

void SmiSubtitles::read(std::fstream &input_file)
{
	string text_line;
	int replica_number = 0;
	while (true)
	{
		std::getline(input_file, text_line);
		if (text_line == "</BODY>")
		{
			break;
		}
		Replica replica;
		regex samiRegex(R"(<SYNC\s+Start=(\d+)\s+End=(\d+)>\s*<P>(.*?)<\/P><\/SYNC>)", std::regex_constants::icase);
		smatch match;
		if (regex_search(text_line, match, samiRegex))
		{
			replica_number += 1;
			replica.setNumber(replica_number);
			replica.setConvStartTime(std::stoi(match[1]));
			replica.setConvEndTime(std::stoi(match[2]));
			replica.setInfo(match[3]);
			replicas.push_back(replica);
		}
	}
}

void SmiSubtitles::write(std::fstream &output_file, const int writing_indx)
{
	switch (writing_indx)
	{
	case 1:
	{
		auto formatSrtTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = total_ms % 1000;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d,%03d", h, m, s, cs);
			return buffer;
		};
		for (auto &replica : replicas)
		{
			output_file << replica.getNumber() << std::endl;
			output_file << formatSrtTime(replica.getConvStartTime()) << "-->" << formatSrtTime(replica.getConvEndTime()) << std::endl;
			output_file << replica.getInfo() << std::endl;
			output_file << std::endl;
		}
		break;
	}
	case 2:
		output_file << "<SAMI>" << std::endl;
		output_file << "<BODY>" << std::endl;
		for (const auto &replica : replicas)
		{
			output_file << "<SYNC Start= " << replica.getConvStartTime() << " End=" << replica.getConvEndTime()
						<< "><P>" << replica.getInfo() << "</P></SYNC>";
		}
		output_file << "</BODY>" << std::endl;
		output_file << "</SAMI>" << std::endl;
		break;
	case 3:
	{
		output_file << "[Script Info]\n";
		output_file << "; Script generated by SrtSubtitles\n";
		output_file << "ScriptType: v4.00+\n";
		output_file << "WrapStyle: 0\n";
		output_file << "ScaledBorderAndShadow: yes\n";
		output_file << "YCbCr Matrix: None\n\n";

		output_file << "[V4+ Styles]\n";
		output_file
			<< "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, "
			   "Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, "
			   "Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n";

		const int r = my_style.get_primary_color() & 0xFF;
		const int g = (my_style.get_primary_color() >> 8) & 0xFF;
		const int b = (my_style.get_primary_color() >> 16) & 0xFF;
		char assColor[16];
		std::snprintf(assColor, sizeof(assColor), "&H%02X%02X%02X%02X", 0, b, g, r);
		output_file
			<< "Style: Default," << my_style.get_font_name() << "," << my_style.get_font_size() << "," << assColor << ","
			<< "&H000000FF,"
			<< "&H00000000,"
			<< "&H00000000,"
			<< "0,0,0,0,"
			<< "100,100,0,0,"
			<< "1," << my_style.get_outline() << "," << my_style.get_shadow() << "," << my_style.get_alignment() << ",10,10,10,0\n\n";
		output_file << "[Events]\n";
		output_file << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n";

		auto formatAssTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%d:%02d:%02d.%02d", h, m, s, cs);
			return buffer;
		};

		for (const auto &replica : replicas)
		{
			string start_ass = formatAssTime(replica.getConvStartTime());
			string end_ass = formatAssTime(replica.getConvEndTime());
			output_file << "Dialogue: 0," << start_ass << "," << end_ass << ",Default,,0,0,0,," << replica.getInfo() << "\n";
		}
		break;
	}
	case 4:
	{
		auto formatEncoreTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d:%02d", h, m, s, cs);
			return buffer;
		};
		for (const auto &replica : replicas)
		{
			output_file << replica.getNumber() << ' ' << formatEncoreTime(replica.getConvStartTime()) << ' '
						<< formatEncoreTime(replica.getConvEndTime()) << ' ' << replica.getInfo() << std::endl;
		}
	}
	default:
	{
		break;
	}
	}
}

int SmiSubtitles::delta_breaker(const string &delta)
{
	const regex delta_pattern(R"((\d{2}):(\d{2}):(\d{2}),(\d{3}))");
	smatch delta_match;
	int delta_ms = 0;
	if (regex_match(delta, delta_match, delta_pattern))
	{
		const int dh = std::stoi(delta_match[1].str());
		const int dm = std::stoi(delta_match[2].str());
		const int ds = std::stoi(delta_match[3].str());
		const int dms = std::stoi(delta_match[4].str());
		delta_ms = dh * 3600000 + dm * 60000 + ds * 1000 + dms;
	}
	else
	{
		return -1;
	}
	return delta_ms;
}

void SmiSubtitles::change_delta(const int format, const string delta)
{
	const int delta_ms = delta_breaker(delta);
	if (delta_ms == -1)
	{
		std::cerr << "Неверный формат delta: " << delta << std::endl;
		return;
	}
	for (auto &replica : replicas)
	{
		if (format == 1 || format == 2)
		{
			replica.setConvStartTime(replica.getConvStartTime() + delta_ms);
		}
		if (format == 1 || format == 3)
		{
			replica.setConvEndTime(replica.getConvEndTime() + delta_ms);
		}
	}
	std::cout << "Дельта изменена.\n";
}

Subtitles *SmiSubtitles::find_collisions() const
{
	auto *collisions = new SmiSubtitles();
	for (size_t i = 0; i + 1 < replicas.size(); i++)
	{
		const int startA = replicas[i].getConvStartTime();
		const int endA = replicas[i].getConvEndTime();
		const int startB = replicas[i + 1].getConvStartTime();
		const int endB = replicas[i + 1].getConvEndTime();
		if (startB <= endA && startA <= endB)
		{
			collisions->replicas.push_back(replicas[i]);
			collisions->replicas.push_back(replicas[i + 1]);
		}
	}
	return collisions;
}

void SmiSubtitles::removeFormatting(std::fstream &input_file)
{
	string line;
	vector< string > lines;
	while (std::getline(input_file, line))
	{
		regex pTagRegex(R"(<P>(.*?)<\/P>)", std::regex_constants::icase);
		smatch match;
		if (std::regex_search(line, match, pTagRegex))
		{
			string inner = match[1].str();
			string cleaned = regex_replace(inner, regex(R"(<[^>]+>)", std::regex_constants::icase), "");
			;
			const string newLine = match.prefix().str() + "<P>" + cleaned + "</P>" + match.suffix().str();
			line = newLine;
		}
		lines.push_back(line);
	}
	input_file.clear();
	input_file.seekp(0, std::ios::beg);
	for (const auto &n_line : lines)
	{
		input_file << n_line << std::endl;
	}
	input_file.seekg(0, std::ios::beg);
}

void SmiSubtitles::set_style()
{
	my_style = Styler();
}

//---------------------------------------------------------------------------------------------------------------------------------
// Ass -> other

void AssSubtitles::read(std::fstream &input_file)
{
	string line;
	int number = 1;
	regex assRegex(R"(Dialogue:\s*\d+,\s*(\d{1,2}):(\d{2}):(\d{2})[.:](\d{2}),\s*(\d{1,2}):(\d{2}):(\d{2})[.:](\d{2}),\s*Default,\s*,\s*\d+,\s*\d+,\s*\d+,\s*,(.*))",
				   std::regex_constants::icase);
	smatch match;
	while (std::getline(input_file, line))
	{
		if (line.empty())
			continue;
		Replica replica;
		if (regex_search(line, match, assRegex))
		{
			replica.setNumber(number++);
			string s = match[1].str() + ":" + match[2].str() + ":" + match[3].str() + "." + match[4].str();
			string e = match[5].str() + ":" + match[6].str() + ":" + match[7].str() + "." + match[8].str();
			replica.setStartTime(s);
			replica.setEndTime(e);
			string start_time = match[1].str() + match[2].str() + match[3].str() + match[4].str() + '0';
			string end_time = match[5].str() + match[6].str() + match[7].str() + match[8].str() + '0';
			replica.setConvStartTime(std::stoi(start_time));
			replica.setConvEndTime(std::stoi(end_time));
			replica.setInfo(match[9].str());
			replicas.push_back(replica);
		}
	}
}

void AssSubtitles::write(std::fstream &output_file, const int writing_indx)
{
	switch (writing_indx)
	{
	case 1:
	{
		auto convertTime = [](const string &assTime) -> string
		{
			const regex srt_time_helper(R"((\d{1,2}):(\d{2}):(\d{2})\.(\d{2}))");
			smatch match;
			if (regex_search(assTime, match, srt_time_helper))
			{
				const string hour = match[1].str().size() == 1 ? string("0") + match[1].str() : match[1].str();
				const string minute = match[2].str();
				const string second = match[3].str();
				const string m_sec = match[4].str() + "0";
				return hour + ":" + minute + ":" + second + "," + m_sec;
			}
			return assTime;
		};

		for (auto &replica : replicas)
		{
			output_file << replica.getNumber() << std::endl;
			string new_s_time = convertTime(replica.getStartTime());
			string new_e_time = convertTime(replica.getEndTime());
			output_file << new_s_time << " --> " << new_e_time << std::endl;
			output_file << replica.getInfo() << std::endl;
			output_file << std::endl;
		}
		break;
	}
	case 2:
	{
		output_file << "<SAMI>" << std::endl;
		output_file << "<BODY>" << std::endl;
		for (const auto &replica : replicas)
		{
			output_file << "<SYNC Start= " << replica.getConvStartTime() << " End=" << replica.getConvEndTime()
						<< "><P>" << replica.getInfo() << "</P></SYNC>";
		}
		output_file << "</BODY>" << std::endl;
		output_file << "</SAMI>" << std::endl;
		break;
	}
	case 3:
	{
		output_file << "[Script Info]\n";
		output_file << "; Script generated by SrtSubtitles\n";
		output_file << "ScriptType: v4.00+\n";
		output_file << "WrapStyle: 0\n";
		output_file << "ScaledBorderAndShadow: yes\n";
		output_file << "YCbCr Matrix: None\n\n";

		output_file << "[V4+ Styles]\n";
		output_file
			<< "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, "
			   "Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, "
			   "Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n";

		const int r = my_style.get_primary_color() & 0xFF;
		const int g = (my_style.get_primary_color() >> 8) & 0xFF;
		const int b = (my_style.get_primary_color() >> 16) & 0xFF;
		char assColor[16];
		std::snprintf(assColor, sizeof(assColor), "&H%02X%02X%02X%02X", 0, b, g, r);
		output_file
			<< "Style: Default," << my_style.get_font_name() << "," << my_style.get_font_size() << "," << assColor << ","
			<< "&H000000FF,"
			<< "&H00000000,"
			<< "&H00000000,"
			<< "0,0,0,0,"
			<< "100,100,0,0,"
			<< "1," << my_style.get_outline() << "," << my_style.get_shadow() << "," << my_style.get_alignment() << ",10,10,10,0\n\n";
		output_file << "[Events]\n";
		output_file << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n";

		auto formatAssTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%d:%02d:%02d.%02d", h, m, s, cs);
			return buffer;
		};

		for (const auto &replica : replicas)
		{
			string start_ass = formatAssTime(replica.getConvStartTime());
			string end_ass = formatAssTime(replica.getConvEndTime());
			output_file << "Dialogue: 0," << start_ass << "," << end_ass << ",Default,,0,0,0,," << replica.getInfo() << "\n";
		}
		break;
	}
	case 4:
	{
		auto formatEncoreTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d:%02d", h, m, s, cs);
			return buffer;
		};
		for (const auto &replica : replicas)
		{
			output_file << replica.getNumber() << ' ' << formatEncoreTime(replica.getConvStartTime()) << ' '
						<< formatEncoreTime(replica.getConvEndTime()) << ' ' << replica.getInfo() << std::endl;
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

int AssSubtitles::delta_breaker(const string &delta)
{
	const regex delta_pattern(R"((\d{2}):(\d{2}):(\d{2}),(\d{3}))");
	smatch delta_match;
	int delta_ms = 0;
	if (regex_match(delta, delta_match, delta_pattern))
	{
		const int dh = std::stoi(delta_match[1].str());
		const int dm = std::stoi(delta_match[2].str());
		const int ds = std::stoi(delta_match[3].str());
		const int dms = std::stoi(delta_match[4].str());
		delta_ms = dh * 3600000 + dm * 60000 + ds * 1000 + dms;
	}
	else
	{
		return -1;
	}
	return delta_ms;
}

void AssSubtitles::change_delta(const int format, const string delta)
{
	const int delta_ms = delta_breaker(delta);
	if (delta_ms == -1)
	{
		std::cerr << "Неверный формат delta: " << delta << std::endl;
		return;
	}

	auto formatAssTime = [](int total_ms) -> string
	{
		const int h = total_ms / 3600000;
		total_ms %= 3600000;
		const int m = total_ms / 60000;
		total_ms %= 60000;
		const int s = total_ms / 1000;
		const int cs = (total_ms % 1000) / 10;
		char buffer[32];
		std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d.%02d", h, m, s, cs);
		return buffer;
	};

	for (auto &replica : replicas)
	{
		if (format == 1 || format == 2)
		{
			replica.setConvStartTime(replica.getConvStartTime() + delta_ms);
			replica.setStartTime(formatAssTime(replica.getConvStartTime()));
		}
		if (format == 1 || format == 3)
		{
			replica.setConvEndTime(replica.getConvEndTime() + delta_ms);
			replica.setEndTime(formatAssTime(replica.getConvEndTime()));
		}
	}
	std::cout << "Дельта изменена.\n";
}

Subtitles *AssSubtitles::find_collisions() const
{
	auto *collisions = new AssSubtitles();
	for (size_t i = 0; i + 1 < replicas.size(); i++)
	{
		const int startA = replicas[i].getConvStartTime();
		const int endA = replicas[i].getConvEndTime();
		const int startB = replicas[i + 1].getConvStartTime();
		const int endB = replicas[i + 1].getConvEndTime();
		if (startB <= endA && startA <= endB)
		{
			collisions->replicas.push_back(replicas[i]);
			collisions->replicas.push_back(replicas[i + 1]);
		}
	}
	return collisions;
}

string AssSubtitles::resetAssStyles(const std::string &line)
{
	const size_t colonPos = line.find(':');
	const string prefix = line.substr(0, colonPos + 1);
	string content = line.substr(colonPos + 1);
	const size_t start = content.find_first_not_of(" \t");
	if (start != string::npos)
		content = content.substr(start);
	vector< string > fields;
	size_t current = 0;
	while (true)
	{
		const size_t commaPos = content.find(',', current);
		if (commaPos == std::string::npos)
		{
			fields.push_back(content.substr(current));
			break;
		}
		fields.push_back(content.substr(current, commaPos - current));
		current = commaPos + 1;
	}
	if (fields.size() >= 9)
	{
		fields[7] = "0";
		fields[8] = "0";
	}
	string newLine = prefix + " ";
	for (size_t i = 0; i < fields.size(); i++)
	{
		newLine += fields[i];
		if (i + 1 < fields.size())
			newLine += ",";
	}
	return newLine;
}

void AssSubtitles::removeFormatting(std::fstream &input_file)
{
	string line;
	vector< string > lines;
	while (std::getline(input_file, line))
	{
		if (line.find("Dialogue:") == 0)
		{
			line = regex_replace(line, std::regex(R"(\{\\[^}]+\})"), "");
		}
		else if (line.find("Style:") == 0)
		{
			line = resetAssStyles(line);
		}
		lines.push_back(line);
	}
	input_file.clear();
	input_file.seekp(0, std::ios::beg);
	for (const auto &n_line : lines)
	{
		input_file << n_line << std::endl;
	}
	input_file.seekg(0, std::ios::beg);
}

void AssSubtitles::set_style()
{
	my_style = Styler();
}

//________________________________________________________________________________________________________________________________
// Encore -> other
void EncoreSubtitles::read(std::fstream &input_file)
{
	string line;
	regex EncorePattern(R"(^\s*(\d+)\s+(\d{2}):(\d{2}):(\d{2}):(\d{2})\s+(\d{2}):(\d{2}):(\d{2}):(\d{2})\s+(.*)$)");
	smatch match;
	while (std::getline(input_file, line))
	{
		if (line.empty())
			continue;
		size_t pos = line.find_first_not_of(" \t");
		if (pos == string::npos || !std::isdigit(line[pos]))
			continue;
		if (regex_match(line, match, EncorePattern))
		{
			Replica replica;
			replica.setNumber(std::stoi(match[1].str()));
			string start_time_str = match[2].str() + ":" + match[3].str() + ":" + match[4].str() + ":" + match[5].str();
			string end_time_str = match[6].str() + ":" + match[7].str() + ":" + match[8].str() + ":" + match[9].str();
			replica.setStartTime(start_time_str);
			replica.setEndTime(end_time_str);
			string new_s_time = match[2].str() + match[3].str() + match[4].str() + match[5].str() + "0";
			string new_e_time = match[6].str() + match[7].str() + match[8].str() + match[9].str() + "0";
			replica.setConvStartTime(std::stoi(new_s_time));
			replica.setConvEndTime(std::stoi(new_e_time));
			replica.setInfo(match[10].str());
			replicas.push_back(replica);
		}
	}
}

void EncoreSubtitles::write(std::fstream &output_file, const int writing_indx)
{
	switch (writing_indx)
	{
	case 1:
	{
		auto convertTime = [](const string &EncoreTime) -> string
		{
			const regex srt_time_helper(R"((\d{1,2}):(\d{2}):(\d{2})\:(\d{2}))");
			smatch match;
			if (regex_search(EncoreTime, match, srt_time_helper))
			{
				const string hour = match[1].str().size() == 1 ? string("0") + match[1].str() : match[1].str();
				const string minute = match[2].str();
				const string second = match[3].str();
				const string m_sec = match[4].str() + "0";
				return hour + ":" + minute + ":" + second + "," + m_sec;
			}
			return EncoreTime;
		};
		for (auto &replica : replicas)
		{
			output_file << replica.getNumber() << std::endl;
			string new_s_time = convertTime(replica.getStartTime());
			string new_e_time = convertTime(replica.getEndTime());
			output_file << new_s_time << " --> " << new_e_time << std::endl;
			output_file << replica.getInfo() << std::endl;
			output_file << std::endl;
		}
		break;
	}
	case 2:
	{
		output_file << "<SAMI>" << std::endl;
		output_file << "<BODY>" << std::endl;
		for (const auto &replica : replicas)
		{
			output_file << "<SYNC Start= " << replica.getConvStartTime() << " End=" << replica.getConvEndTime()
						<< "><P>" << replica.getInfo() << "</P></SYNC>";
		}
		output_file << "</BODY>" << std::endl;
		output_file << "</SAMI>" << std::endl;
		break;
	}
	case 3:
	{
		output_file << "[Script Info]\n";
		output_file << "; Script generated by SrtSubtitles\n";
		output_file << "ScriptType: v4.00+\n";
		output_file << "WrapStyle: 0\n";
		output_file << "ScaledBorderAndShadow: yes\n";
		output_file << "YCbCr Matrix: None\n\n";

		output_file << "[V4+ Styles]\n";
		output_file
			<< "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, "
			   "Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, "
			   "Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n";

		const int r = my_style.get_primary_color() & 0xFF;
		const int g = (my_style.get_primary_color() >> 8) & 0xFF;
		const int b = (my_style.get_primary_color() >> 16) & 0xFF;
		char assColor[16];
		std::snprintf(assColor, sizeof(assColor), "&H%02X%02X%02X%02X", 0, b, g, r);
		output_file
			<< "Style: Default," << my_style.get_font_name() << "," << my_style.get_font_size() << "," << assColor << ","
			<< "&H000000FF,"
			<< "&H00000000,"
			<< "&H00000000,"
			<< "0,0,0,0,"
			<< "100,100,0,0,"
			<< "1," << my_style.get_outline() << "," << my_style.get_shadow() << "," << my_style.get_alignment() << ",10,10,10,0\n\n";
		output_file << "[Events]\n";
		output_file << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n";

		auto formatAssTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%d:%02d:%02d.%02d", h, m, s, cs);
			return buffer;
		};

		for (const auto &replica : replicas)
		{
			string start_ass = formatAssTime(replica.getConvStartTime());
			string end_ass = formatAssTime(replica.getConvEndTime());
			output_file << "Dialogue: 0," << start_ass << "," << end_ass << ",Default,,0,0,0,," << replica.getInfo() << "\n";
		}
		break;
	}
	case 4:
	{
		auto formatEncoreTime = [](int total_ms) -> string
		{
			const int h = total_ms / 3600000;
			total_ms %= 3600000;
			const int m = total_ms / 60000;
			total_ms %= 60000;
			const int s = total_ms / 1000;
			const int cs = (total_ms % 1000) / 10;
			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d:%02d", h, m, s, cs);
			return buffer;
		};
		for (const auto &replica : replicas)
		{
			output_file << replica.getNumber() << ' ' << formatEncoreTime(replica.getConvStartTime()) << ' '
						<< formatEncoreTime(replica.getConvEndTime()) << ' ' << replica.getInfo() << std::endl;
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

int EncoreSubtitles::delta_breaker(const string &delta)
{
	const regex delta_pattern(R"((\d{2}):(\d{2}):(\d{2}),(\d{3}))");
	smatch delta_match;
	int delta_ms = 0;
	if (regex_match(delta, delta_match, delta_pattern))
	{
		const int dh = std::stoi(delta_match[1].str());
		const int dm = std::stoi(delta_match[2].str());
		const int ds = std::stoi(delta_match[3].str());
		const int dms = std::stoi(delta_match[4].str());
		delta_ms = dh * 3600000 + dm * 60000 + ds * 1000 + dms;
	}
	else
	{
		return -1;
	}
	return delta_ms;
}

void EncoreSubtitles::change_delta(const int format, const string delta)
{
	const int delta_ms = delta_breaker(delta);
	if (delta_ms == -1)
	{
		std::cerr << "Неверный формат delta: " << delta << std::endl;
		return;
	}

	auto formatEncoreTime = [](int total_ms) -> string
	{
		const int h = total_ms / 3600000;
		total_ms %= 3600000;
		const int m = total_ms / 60000;
		total_ms %= 60000;
		const int s = total_ms / 1000;
		const int cs = (total_ms % 1000) / 10;
		char buffer[32];
		std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d:%02d", h, m, s, cs);
		return buffer;
	};

	for (auto &replica : replicas)
	{
		if (format == 1 || format == 2)
		{
			replica.setConvStartTime(replica.getConvStartTime() + delta_ms);
			replica.setStartTime(formatEncoreTime(replica.getConvStartTime()));
		}
		if (format == 1 || format == 3)
		{
			replica.setConvEndTime(replica.getConvEndTime() + delta_ms);
			replica.setEndTime(formatEncoreTime(replica.getConvEndTime()));
		}
	}
	std::cout << "Дельта изменена\n";
}

Subtitles *EncoreSubtitles::find_collisions() const
{
	auto *collisions = new EncoreSubtitles();
	for (size_t i = 0; i + 1 < replicas.size(); i++)
	{
		const int startA = replicas[i].getConvStartTime();
		const int endA = replicas[i].getConvEndTime();
		const int startB = replicas[i + 1].getConvStartTime();
		const int endB = replicas[i + 1].getConvEndTime();
		if (startB <= endA && startA <= endB)
		{
			collisions->replicas.push_back(replicas[i]);
			collisions->replicas.push_back(replicas[i + 1]);
		}
	}
	return collisions;
}

void EncoreSubtitles::removeFormatting(std::fstream &input_file)
{
	string line;
	vector< string > lines;
	while (std::getline(input_file, line))
	{
		line = regex_replace(line, std::regex(R"(<\\[biu]>)", std::regex_constants::icase), "");
		line = regex_replace(line, std::regex(R"(<\/\\[biu]>)", std::regex_constants::icase), "");
		line = regex_replace(line, std::regex(R"(<[^>]+>)", std::regex_constants::icase), "");
		lines.push_back(line);
	}
	input_file.clear();
	input_file.seekp(0, std::ios::beg);
	for (const auto &n_line : lines)
	{
		input_file << n_line << std::endl;
	}
	input_file.seekg(0, std::ios::beg);
}

void EncoreSubtitles::set_style()
{
	my_style = Styler();
}
