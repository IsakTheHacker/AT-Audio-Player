
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
			if os.path.isfile(playlist):
				with open(playlist) as file:
					data = json.load(file)
				pygame.mixer.music.load(data["songs"][0])
				for entry in data["songs"][1:len(data["songs"])]:
					pygame.mixer.music.queue(entry)
				pygame.mixer.music.play()
			elif os.path.isdir(playlist):
				files = []
				for entry in os.scandir(playlist):
					if (entry.is_file()):
						if entry.name.endswith(".mp3"):
							files.append(entry.name)
				if len(files) > 0:
					pygame.mixer.music.load(playlist + "/" + files[0])
					for file in files[1:len(files)]:
						pygame.mixer.music.queue(playlist + "/" + file)
					pygame.mixer.music.play()
				else:
					print("Directory does not contain any mp3-files")
		else:
			print("Playlist doesn't exist.")

	if char == "h":
		print("\nHELP:")
		print("p - Pause/Unpause")
		print("s - Stop")
		print("l - Load new song")
		print("i - Load playlist (can be json or folder)")
		print("e/q - Quit program")
		print()

	if char == "o":
		print("\nONETIME OPTIONS:")
		print("v - Change volume!")
		char2 = msvcrt.getwch()
		if char2 == "v":
			newVolume = float(input("Volume is currently set to {}/100. Type a value between 0 and 100: ".format(pygame.mixer.music.get_volume() * 100)))
			pygame.mixer.music.set_volume(newVolume / 100)
			print("New volume set to {}!\n".format(newVolume))

	if char == "e":
		print("Bye!\n")
		sys.exit(0)
	if char == "q":
		print("Goodbye!\n")
		sys.exit(0)