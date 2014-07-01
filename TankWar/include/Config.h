/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   Config.h
			created:    5/28/2012
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "Util.h"

namespace Tank
{
	/**
	Profiles


	# comment
	[section1]
	key=value

	[section2]
	key1=value
	key2=value
	key3=value
	*/
	class Config
	{
		typedef std::map<std::string, std::string> StringMap;
		typedef std::map<std::string, StringMap> StringStringMap;

		typedef StringMap::iterator SItr;
		typedef StringStringMap::iterator SSItr;

	public:
		Config(const std::string &filename);

		~Config();

		// section if there are
		bool sectionExsit(const std::string &section);

		//Key value of the specified section exists
		bool keyExsit(const std::string &section, const std::string &key);

		// Gets the specified key value Secion --  string
		std::string getString(const std::string &section, const std::string &key);
		//Gets the key value specified Secion - float
		float getFloat(const std::string &section, const std::string &key);
		// Gets the specified key value Secion's - Bool
		bool getBoolean(const std::string &section, const std::string &key);

		// All key keys specified section
		std::vector<std::string> getKeyIterator(const std::string &section);

		bool hasError();
	private:
		StringStringMap mParams;

		bool						mError;
	};
}