
#Imports
import os
import sys
import time
import msvcrt
import json

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame

#Variables
welcome_message = "Welcome to Python AT Audio Player!"

#Welcome the user
print("\n" + welcome_message)
for var in welcome_message:
	print("-", end="", flush=True)
	time.sleep(0.05)
print()

pygame.mixer.init()									#Initalize mixer
pygame.mixer.music.load("song.mp3")					#Load song
pygame.mixer.music.play()							#Play song!

while True:
	char = msvcrt.getwch()
	if char == "p" and pygame.mixer.music.get_busy():
		pygame.mixer.music.pause()
		print("Paused!")
	elif char == "p" and not pygame.mixer.music.get_busy():
		pygame.mixer.music.unpause()
		print("Unpaused!")

	if char == "s":
		pygame.mixer.music.stop()
		pygame.mixer.music.unload()
		print("Stopped!")

	if char == "l":
		new_song = input("Type the new song here: ")
		if os.path.exists(new_song):
			pygame.mixer.music.load(new_song)
			pygame.mixer.music.play()
			print("New song loaded!")
		else:
			print("Song doesn't exist.")

	if char == "i":
		playlist = input("Type the playlist here: ")
		if os.path.exists(playlist):
			with open(playlist) as file:
				data = json.load(file)
			pygame.mixer.music.load(data["songs"][0])
			for entry in data["songs"][1:len(data["songs"])]:
				pygame.mixer.music.queue(entry)
			pygame.mixer.music.play()
		else:
			print("Playlist doesn't exist.")