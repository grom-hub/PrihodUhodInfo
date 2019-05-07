#pragma once

#include <curl/curl.h>
#include <fstream>


const int BUF_SIZE = 10000;


class CurlConnector
{
public:
	void setChatid(std::string inputid);
	void connectMail();
	void getTargetMsg();
	void cleanup();



	std::string myName;
	std::string myPass;
	std::string mailURL;
	std::string mySubject;
	std::string storageFile;
	std::string botURL;
	std::string proxyAddress;
	std::string proxyUserPWD;
	std::string botOutFile;
	std::string chatid;
	bool useProxy;



private:
	void handleReqest();
	void sendToBot();


	CURL *curlMail;
	CURLcode resultMail;

	CURL *curlBot;
  	CURLcode resultBot;
  	std::string msgToBot;

	char writeData[BUF_SIZE];

	std::string writeDataStr;
	std::string myReqest;

	int letterCount;
	int findStatus;
	int headerSize;

	std::ofstream fileOut;
  	std::ifstream fileIn;

  	std::string line;
	std::string compareBuff;

	std::string msgBody;



};


