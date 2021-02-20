import sys
import os
import msvcrt
import random
#Check if script is called as main module
import sys
if __name__ == "__main__":
	print("\nThis module is not meant to be executed standalone!")
	sys.exit(0)

from time import sleep
import json

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame

import patapvars
from patapclasses import globals, nowPlaying, pressedCharacter, PlaylistData

def saveFiles():
	#Save options
	patapvars.options["volume"] = pygame.mixer_music.get_volume()
	with open(patapvars.scriptDir + "/options.json","w") as file:
		json.dump(patapvars.options, file, indent=4)

	#Append played songs to log
	with open(patapvars.scriptDir + "/play.log","a") as file:
		if not nowPlaying.logged:
			file.write(nowPlaying.path + "\n")
			nowPlaying.logged = True
def countfiles(path):
	return len([entry for entry in os.scandir(path) if entry.is_file() and entry.name.endswith(".mp3")])
def setGlobalChar(event):
	pressedCharacter.globalChar = event
def getChar():
	while patapvars.shouldRun:
		pressedCharacter.char = msvcrt.getwch()
def loading_animation(str):
	array = ["/", "—", "\\", "|"]
	n = 0
	while patapvars.loadingAnimation:
		print("\r" + array[n] + " " + str, end="")
		sleep(0.15)
		if n == 3:
			n = 0
		else:
			n += 1
	sys.exit(0)
def wait(seconds = 0.2):
	pressedCharacter.char = ""
	sleep(seconds)
def lineBreak():
	if (globals.lastCommandWasVolume):
		globals.lastCommandWasVolume = False
		print()
def play(files):
	if len(files) == 1:
		type = "song"
	elif len(files) > 1:
		type = "playlist"
	while patapvars.shouldRun:
		if globals.shouldRunB:
			for event in pygame.event.get():
				if event.type == pygame.USEREVENT + 1:
					saveFiles() #Save files
					if patapvars.loopSong:
						pygame.mixer_music.play()

						#Add to log
						patapvars.songLog.append(nowPlaying.path)
					else:
						nowPlaying.paused = None
		else:
			globals.shouldRunB = True
		if (not pygame.mixer_music.get_busy()) and (nowPlaying.paused == None):
			if files:
				index = -1
				if (type == "playlist"):
					while True:
						if patapvars.options["shuffle"]:
							index = random.randint(0, len(files)-1)
						if nowPlaying.path != files[index].path: break
						if not patapvars.options["shuffle"]: break
				pygame.mixer_music.load(files[index].path)

				#Change nowPlaying object
				nowPlaying.name = files[index].name
				nowPlaying.path = files[index].path
				nowPlaying.paused = False
				nowPlaying.length = files[index].length
				nowPlaying.positionOffset = files[index].positionOffset
				nowPlaying.logged = False

				#Add to log
				patapvars.songLog.append(files[index].path)

				pygame.mixer_music.play()
				if (not patapvars.options["shuffle"]) or (patapvars.options["shuffle"] and not patapvars.options["shuffleIndefinitely"]):
					files.pop(index)
				if (type == "playlist"):
					nowPlaying.playlist = PlaylistData()
					if (patapvars.options["shuffle"]) and (patapvars.options["shuffleIndefinitely"]):
						nowPlaying.playlist.files = []
						nowPlaying.playlist.shuffleIndefinitely = True
					else:
						nowPlaying.playlist.files = files
						nowPlaying.playlist.shuffleIndefinitely = False
				else:
					nowPlaying.playlist = None
			else:
				print("{} ended! Start a new song or playlist?".format(type.capitalize()))

				#Change nowPlaying object
				nowPlaying.name = None
				nowPlaying.path = None
				nowPlaying.paused = None
				nowPlaying.length = None
				nowPlaying.positionOffset = 0
				nowPlaying.playlist = None
				nowPlaying.logged = False
				
				sys.exit(0)		#Exit thread