#pragma once

#include <string>
#include <chrono>

class Message {
private:
	std::string message;
	std::chrono::time_point<std::chrono::steady_clock> end;
public:
	Message(const std::string& message, const unsigned int& secondsOnScreen);
	void setMessage(const std::string& message);
	std::string getMessage();
	std::chrono::time_point<std::chrono::steady_clock> getEnd();
};