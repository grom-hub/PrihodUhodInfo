// #include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>

#include "curlConnector.h"



int writeIndex = 0;



size_t writeFunc(char* ptr, size_t size, size_t nmemb, char* data)
{

    memcpy(&data[writeIndex],ptr,size*nmemb); // add data to the end
    writeIndex+=size*nmemb; // change write position
 
    return size*nmemb;
}



void CurlConnector::connectMail()
{
	curlMail = curl_easy_init();

	curl_easy_setopt(curlMail, CURLOPT_USERNAME, myName.c_str());
	curl_easy_setopt(curlMail, CURLOPT_PASSWORD, myPass.c_str());

	curl_easy_setopt(curlMail, CURLOPT_URL, mailURL.c_str());

	curl_easy_setopt(curlMail, CURLOPT_WRITEFUNCTION, writeFunc); // function for write buffer

	curl_easy_setopt(curlMail, CURLOPT_WRITEDATA, writeData); // take recorded buffer (inbox list)

	memset(writeData, 0, BUF_SIZE);
	resultMail = curl_easy_perform(curlMail); // execute specified commands
	writeIndex = 0;
	writeDataStr = writeData;

	if(resultMail != CURLE_OK)
	  std::cout << curl_easy_strerror(resultMail) << std::endl;

	// curl_easy_setopt(curlMail, CURLOPT_VERBOSE, 1L); // debug info
	// curl_easy_perform(curlMail);


}



void CurlConnector::getTargetMsg()
{

	if(writeDataStr.size() < 4) // if no latters - exit
		return;

	letterCount = 0;
	for (int i = 0; i < writeDataStr.size(); ++i) // Count the number of latters (lines of list)
	{
		if(writeDataStr[i] == '\n')
			letterCount ++;
	}

		
    for (int i = 1; i <= letterCount; ++i) // Search the emails with the specified title
    {
		myReqest = "TOP " + std::to_string(i) + " 0";

		handleReqest(); // Record the result of the myReqest to writeData

		findStatus = writeDataStr.find(mySubject);

		if(findStatus >= 0)
		{
			headerSize = writeDataStr.size();

			myReqest = "TOP " + std::to_string(i) + " 1";

			handleReqest();

			msgBody = writeDataStr.substr(headerSize);

			fileIn.open(storageFile); // Search for the found latter in the archive

			while(std::getline(fileIn, line))
			{
				compareBuff += line;
				compareBuff += "\n";
			}

			fileIn.close();

			findStatus = compareBuff.find(msgBody); // If not found add to archive and send to bot
			if(findStatus == -1)
			{
				std::cout << msgBody << std::endl;
				sendToBot();

				fileOut.open(storageFile, std::ios_base::app);
				fileOut << msgBody << std::endl;
				fileOut.close();
			}
		}
    }
}



void CurlConnector::handleReqest()
{
	curl_easy_setopt(curlMail, CURLOPT_CUSTOMREQUEST, myReqest.c_str());
	memset(writeData, 0, BUF_SIZE);
	resultMail = curl_easy_perform(curlMail); // Record the result of the myReqest to writeData
	writeIndex = 0;
	writeDataStr = writeData;

	if(resultMail != CURLE_OK)
		std::cout << curl_easy_strerror(resultMail) << std::endl;
}



void CurlConnector::sendToBot()
{
	msgToBot = "chat_id=" + chatid + "&text=" + msgBody;

	curlBot = curl_easy_init();

	curl_easy_setopt(curlBot, CURLOPT_URL, botURL.c_str());

	if(useProxy)
	{
		curl_easy_setopt(curlBot, CURLOPT_PROXY, proxyAddress.c_str());
		curl_easy_setopt(curlBot, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
		curl_easy_setopt(curlBot, CURLOPT_PROXYUSERPWD , proxyUserPWD.c_str());
	}

	curl_easy_setopt(curlBot, CURLOPT_POSTFIELDS, msgToBot.c_str());

	FILE *botOut = fopen(botOutFile.c_str(), "wb"); // remove the output to the terminal
	curl_easy_setopt(curlBot, CURLOPT_WRITEDATA, botOut);

	resultBot = curl_easy_perform(curlBot);

	if(resultBot != CURLE_OK)
	  std::cout << curl_easy_strerror(resultBot) << std::endl;

	//---
	msgToBot = "chat_id=226082970&text=" + msgBody;
	curl_easy_setopt(curlBot, CURLOPT_POSTFIELDS, msgToBot.c_str());

	resultBot = curl_easy_perform(curlBot);

	if(resultBot != CURLE_OK)
	  std::cout << curl_easy_strerror(resultBot) << std::endl;
	//---

	fclose(botOut);
	curl_easy_cleanup(curlBot);
}



void CurlConnector::cleanup()
{
	curl_easy_cleanup(curlMail);
}
