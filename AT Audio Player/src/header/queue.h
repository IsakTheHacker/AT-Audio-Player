#pragma once

#include <queue>

#include "queueItem.h"

class Queue {
private:
	std::queue<QueueItem> queue;
public:
	void pushItem(QueueItem item) {
		queue.push(item);
	}
	void pushItem(Song song) {
		queue.push(QueueItem(song));
	}
	void pushItem(Playlist playlist) {
		queue.push(QueueItem(playlist));
	}
	QueueItem getFront() {
		return queue.front();
	}
	QueueItem popFront() {
		QueueItem queueItem = queue.front();
		queue.pop();
		return queueItem;
	}
	bool getEmpty() {
		if (!queue.size()) return true;
		return false;
	}
	std::vector<std::string> getItemNames() {
		std::vector<std::string> itemNames;
		std::queue tmp_q = queue;		//copy the original queue to the temporary queue
		while (!tmp_q.empty()) {
			itemNames.push_back(tmp_q.front().getName());
			tmp_q.pop();
		}
		return itemNames;
	}
};