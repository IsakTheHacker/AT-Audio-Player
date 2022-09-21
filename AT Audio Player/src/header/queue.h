#pragma once

#include <queue>

#include "queueItem.h"

class Queue {
private:
	std::queue<QueueItem> queue;
public:
	void pushItem(QueueItem item);
	void pushItem(Song song);
	void pushItem(Playlist playlist);
	QueueItem getFront();
	QueueItem popFront();
	bool getEmpty();
	std::vector<std::string> getItemNames();
};