#Check if script is called as main module
import sys
if __name__ == "__main__":
	print("\nThis module is not meant to be executed standalone!")
	sys.exit(0)

import os
import json

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame
pygame.init()

welcome_message = "Welcome to Python AT Audio Player!"
scriptDir = os.path.dirname(os.path.realpath(__file__))
shouldRun = True
loopSong = False
loadingAnimation = False
songLog = []
standardOptions = {
	"shuffle": False,
	"shuffleIndefinitely": True,
	"volume": 1,
	"onStartSong": None
}

#Load options
if (os.path.exists(scriptDir + "/options.json")) and (os.path.isfile(scriptDir + "/options.json")):
	with open(scriptDir + "/options.json","r") as file:
		options = json.load(file)
	print("Options loaded!")
else:
	options = standardOptions
	print("Options file could not be found...")
pygame.mixer_music.set_volume(options["volume"])