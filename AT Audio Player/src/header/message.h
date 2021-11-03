#pragma once

#include <string>

class Message {
private:
	std::string message;
	unsigned int secondsOnScreen;
public:
	Message(const std::string& message, const unsigned int& secondsOnScreen);
	void setMessage(const std::string& message);
	std::string getMessage();
};