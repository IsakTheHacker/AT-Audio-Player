
#Imports
import os
import sys
import time
import msvcrt
import json
import threading as thread

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame

#Variables
welcome_message = "Welcome to Python AT Audio Player!"

#Functions
def eventGet():
	while True:
		for event in pygame.event.get():
			if event.type == SONG_END:
				print("the song ended!")

#Welcome the user
print("\n" + welcome_message)
for var in welcome_message:
	print("-", end="", flush=True)
	time.sleep(0.05)
print()

#Initalization
pygame.init()
SONG_END = pygame.USEREVENT + 1
pygame.mixer.music.set_endevent(SONG_END)
thread._start_new_thread(eventGet,())

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
				pygame.mixer.music.load(data["songs"][-1])
				for entry in data["songs"][-2::-1]:
					pygame.mixer.music.queue(entry)
				pygame.mixer.music.play()
			elif os.path.isdir(playlist):
				files = []
				for entry in os.scandir(playlist):
					if (entry.is_file()):
						if entry.name.endswith(".mp3"):
							files.append(entry.name)
				if len(files) > 0:
					pygame.mixer.music.load(playlist + "/" + files[-1])
					for file in files[-2::-1]:
						pygame.mixer.music.queue(playlist + "/" + file)
					pygame.mixer.music.play()
				else:
					print("Directory does not contain any mp3-files")
		else:
			print("Playlist doesn't exist.")

	if char == "r":
		pygame.mixer.music.rewind()
		print("Rewind!")

	if char == "h":
		print("\nHELP:")
		print("p - Pause/Unpause")
		print("s - Stop")
		print("l - Load new song")
		print("i - Load playlist (can be json or folder)")
		print("r - Rewind")
		print("o - Change onetime options")
		print("c - View credits")
		print("h - Access this menu")
		print("e/q - Quit program")
		print()

	if char == "c":
		print("\nCREDITS:")
		print("Author: Isak Brynielsson Neri")
		print("Libs:")
		print("    Pygame: {}".format(pygame.version.ver))
		print()

	if char == "o":
		print("\nONETIME OPTIONS:")
		print("v - Change volume!")
		char2 = msvcrt.getwch()
		if char2 == "v":
			print("Volume is currently set to {}/100. ".format(pygame.mixer.music.get_volume() * 100), end="")
			while True:
				newVolumeStr = input("Type a value between 0 and 100: ")
				try:
					newVolume = float(newVolumeStr)
				except ValueError:
					print("Not convertible to float. Try again!")
					continue
				if newVolume > 100:
					print("You entered a value which was too big. Volume will be set to max (100)")
				elif newVolume < 0:
					print("You entered a value which was too low. Volume will be set to minimum (0)")
				break
			pygame.mixer.music.set_volume(newVolume / 100)
			print("New volume set to {}!\n".format(pygame.mixer.music.get_volume() * 100))

	if char == "e":
		print("Bye!\n")
		break
	if char == "q":
		print("Goodbye!\n")
		break

pygame.mixer.quit()