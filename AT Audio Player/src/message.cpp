#include "message.h"

Message::Message(const std::string& message, const unsigned int& secondsOnScreen) {
	this->message = message;
	this->secondsOnScreen = secondsOnScreen;
}
void Message::setMessage(const std::string& message) {
	this->message = message;
}
std::string Message::getMessage() {
	return message;
}