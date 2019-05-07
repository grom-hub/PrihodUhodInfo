#include <string>
#include <iostream>
#include <unistd.h>

#include "configParser.h"
#include "curlConnector.h"


int main(int argc, char const *argv[])
{
	int checkDelay;
	ConfigParser conf;
	CurlConnector cc;


	std::cout << "-- PRIHOD UHOD INFO v1.0 --\n";


	if(!conf.setSettings(cc, checkDelay))
	{
		std::cerr << "ConfigParser error.." << std::endl;
		return 1;
	}

	if(argc > 2)
	{
		cc.myName = argv[1];
		cc.myPass = argv[2];
	}
	else
	{
		std::cout << "Input username:\n";
		std::cin >> cc.myName;

		std::cout << "Input pasword:\n";
		std::cin >> cc.myPass;
	}

	std::cout << "Program is running...\n(Inbox check every " << checkDelay / 1000000 << " second)\n";

	while(true)
	{
		cc.connectMail();

		cc.getTargetMsg();

		cc.cleanup();

		usleep(checkDelay);
	}

	std::cout << "Program is stopped.\n";
	return 1;
}


