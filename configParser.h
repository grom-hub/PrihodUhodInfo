#pragma once

#include <iostream>
#include <libconfig.h++>

#include "curlConnector.h"


class ConfigParser
{
public:
	bool setSettings(CurlConnector &cc, int &checkDelay);

};
