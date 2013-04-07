//created by Wang Jianwei on 30 Nov 2012

#ifndef _FILEUTIL_H
#define _FILEUTIL_H

#include <iostream>
#include <string>
#include <vector>


namespace utilities {
	class FileUtil {

	public:

		static std::vector<std::string> split(const std::string &s, char delim);

	};
}

#endif