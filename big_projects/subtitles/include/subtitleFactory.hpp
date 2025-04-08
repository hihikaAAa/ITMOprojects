#ifndef SUBTITLEFACTORY_HPP
#define SUBTITLEFACTORY_HPP
#include "example.hpp"
#include "tokenizator.hpp"

#include <string>
class SubtitleFactory
{
  public:
	static Subtitles *createSubtitle(const string &filename);
};
#endif	  // SUBTITLEFACTORY_HPP
