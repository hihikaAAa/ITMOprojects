#include "../include/subtitleFactory.hpp"

Subtitles *SubtitleFactory::createSubtitle(const string &filename)
{
	const int token = sub_tokenization(filename);
	switch (token)
	{
	case SRT_TOKEN:
		return new SrtSubtitles();
	case SMI_TOKEN:
		return new SmiSubtitles();
	case ASS_TOKEN:
		return new AssSubtitles();
	case ENCORE_TOKEN:
		return new EncoreSubtitles();
	default:
		throw std::invalid_argument("Unsupported subtitle format.");
	}
}
