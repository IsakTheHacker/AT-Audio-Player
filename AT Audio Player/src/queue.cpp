#include "queue.h"

void Queue::pushItem(QueueItem item) {
	queue.push(item);
}
void Queue::pushItem(Song song) {
	queue.push(QueueItem(song));
}
void Queue::pushItem(Playlist playlist) {
	queue.push(QueueItem(playlist));
}
QueueItem Queue::getFront() {
	return queue.front();
}
QueueItem Queue::popFront() {
	QueueItem queueItem = queue.front();
	queue.pop();
	return queueItem;
}
bool Queue::getEmpty() {
	if (!queue.size()) return true;
	return false;
}
std::vector<std::string> Queue::getItemNames() {
	std::vector<std::string> itemNames;
	std::queue tmp_q = queue;		//copy the original queue to the temporary queue
	while (!tmp_q.empty()) {
		itemNames.push_back(tmp_q.front().getName());
		tmp_q.pop();
	}
	return itemNames;
}