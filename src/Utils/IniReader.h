// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include <fstream>
#include <map>
#include <memory>

#include "tinyformat/tinyformat.h"

namespace Raycer
{
	class BaseLog;
	class NamedLog;

	class IniReader
	{
	public:

		IniReader(BaseLog& baseLog);

		void readFile(const std::string& fileName);

		template<typename T>
		T getValue(const std::string& sectionName, const std::string& keyName);

	private:

		std::unique_ptr<NamedLog> log;

		std::map<std::string, std::map<std::string, std::string>> sections;
	};
}

template<typename T>
T Raycer::IniReader::getValue(const std::string& sectionName, const std::string& keyName)
{
	if (sections.count(sectionName) == 0 || sections[sectionName].count(keyName) == 0)
		throw std::runtime_error(tfm::format("Could not find %s::%s", sectionName, keyName));

	T result = T();
	std::istringstream ss(sections[sectionName][keyName]);
	ss.exceptions(std::ifstream::failbit);

	try
	{
		ss >> std::boolalpha >> result;
	}
	catch (const std::istringstream::failure& f)
	{
		throw std::runtime_error(tfm::format("Could not convert %s::%s to %s (%s)", sectionName, keyName, typeid(T).name(), f.what()));
	}

	return result;
}
