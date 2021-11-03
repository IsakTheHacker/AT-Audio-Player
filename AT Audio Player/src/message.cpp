#include "message.h"

Message::Message(const std::string& message, const unsigned int& secondsOnScreen) {
	this->message = message;

	using namespace std::chrono_literals;
	end = std::chrono::high_resolution_clock::now() + secondsOnScreen * 1s;
}
void Message::setMessage(const std::string& message) {
	this->message = message;
}
std::string Message::getMessage() {
	return message;
}
std::chrono::time_point<std::chrono::steady_clock> Message::getEnd() {
	return end;
}