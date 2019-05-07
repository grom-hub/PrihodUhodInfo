#include <iostream>
#include <libconfig.h++>
#include <fstream>

#include "configParser.h"


bool ConfigParser::setSettings(CurlConnector &cc, int &checkDelay)
{
	libconfig::Config cfg;

	try
	{
	cfg.readFile("PrihodUhodInfo.conf");
	}
	catch(const libconfig::FileIOException &fioex)
	{
	std::cerr << "I/O error while reading file." << std::endl;
	return false;
	}
	catch(const libconfig::ParseException &pex)
	{
	std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
	          << " - " << pex.getError() << std::endl;
	return false;
	}


	try
	{
		libconfig::Setting &root = cfg.getRoot()["options"];

		if(!(root.lookupValue("ChatId", cc.chatid)
			&& root.lookupValue("MailURL", cc.mailURL)
			&& root.lookupValue("Subject", cc.mySubject)
			&& root.lookupValue("BotURL", cc.botURL)
			&& root.lookupValue("ProxyAddress", cc.proxyAddress)
			&& root.lookupValue("ProxyUserPWD", cc.proxyUserPWD)
			&& root.lookupValue("BotOutFile", cc.botOutFile)
			&& root.lookupValue("UseProxy", cc.useProxy)
			&& root.lookupValue("CheckDelay", checkDelay)
			&& root.lookupValue("StorageFile", cc.storageFile)))
		{
			std::cerr << "Configuration parametr not found!" << std::endl;
			return false;
		}

	}
	catch(const libconfig::SettingNotFoundException &nfex)
	{
		std::cerr << "No 'name' setting in configuration file." << std::endl;
	}

	return true;
  }