#include "song.h"

#include <filesystem>
namespace fs = std::filesystem;

Song::Song() {

}

Song::Song(std::string path) {
	fs::path songPath(path);
	name = songPath.filename().string();
	this->path = path;
}

std::string Song::getPath() { return path; }
std::string Song::getName() { return name; }