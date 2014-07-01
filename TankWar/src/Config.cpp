/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   Config.cpp
			created:    5/28/2012
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "Config.h"

namespace Tank
{


	Config::Config(const std::string &filename) : mError(false)
	{
		std::ifstream in;
		in.open(filename.c_str());

		if (in.good())
		{
			std::string line;
			std::string section = "";
			StringMap setting;

			while (in.good())
			{
				std::getline(in, line);
				line = Util::trim(line);

				// Delete blank lines and comment lines
				if (line.size() == 0 || line[0] == '#')
					continue;

				if (line[0] == '[')	// ÐÂsection
				{
					mParams[section] = setting;
					section = line.substr(1, line.size() - 2);
					setting.clear();
				}
				else
				{
					std::size_t index = line.find('=');
					std::string key = line.substr(0, index);
					std::string value = line.substr(index + 1);
					setting[key] = value;
				}
			}

			mParams.erase("");
			mParams[section] = setting;

			in.close();
		}
		else
			mError = true;
	}



	Config::~Config()
	{
		mParams.clear();
	}



	bool Config::sectionExsit(const std::string &section)
	{
		return mParams.count(section) > 0;
	}



	bool Config::keyExsit(const std::string &secion, const std::string &key)
	{
		if (!sectionExsit(secion))
			return false;

		return mParams[secion].count(key) > 0;
	}



	std::string Config::getString(const std::string &secion, const std::string &key)
	{
		return (mParams[secion])[key];
	}



	float Config::getFloat(const std::string &secion, const std::string &key)
	{
		return atof(getString(secion, key).c_str());
	}



	bool Config::getBoolean(const std::string &secion, const std::string &key)
	{
		std::string result = getString(secion, key);

		if (Util::equal(result, "True"))
			return true;
		else
			return false;
	}



	bool Config::hasError()
	{
		return mError;
	}



	std::vector<std::string> Config::getKeyIterator(const std::string &section)
	{
		std::vector<std::string> keys;

		for (SItr itr = mParams[section].begin(); itr != mParams[section].end(); ++itr)
			keys.push_back(itr->first);

		return keys;
	}
}