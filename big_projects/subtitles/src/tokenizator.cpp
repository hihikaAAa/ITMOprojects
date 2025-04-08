#include "../include/tokenizator.hpp"

int sub_tokenization(const std::string &str)
{
	const size_t pos = str.rfind('.');
	const std::string format = str.substr(pos + 1, std::string::npos);
	if (format == "srt")
	{
		return SRT_TOKEN;
	}
	if (format == "smi")
	{
		return SMI_TOKEN;
	}
	if (format == "ass" || format == "ssa")
	{
		return ASS_TOKEN;
	}
	if (format == "txt")
	{
		const size_t new_pos = str.find('.');
		const std::string new_format = str.substr(new_pos + 1, std::string::npos);
		if (new_format == "encore.txt")
		{
			return ENCORE_TOKEN;
		}
	}
	return 0;
}
