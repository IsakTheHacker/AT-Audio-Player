#include "song.h"

Song::Song() {

}

Song::Song(std::string path) {
	song = engine->addSoundSourceFromFile(path.c_str(), irrklang::ESM_AUTO_DETECT, true);		//Third parameter set to true = preload
	if (!song) {
		song = engine->getSoundSource(path.c_str());
	}
	fs::path songPath(path);
	name = songPath.filename().string();
	this->path = path;
}

std::string Song::getPath() { return path; }
std::string Song::getName() { return name; }
irrklang::ISoundSource* Song::getSource() { return song; }