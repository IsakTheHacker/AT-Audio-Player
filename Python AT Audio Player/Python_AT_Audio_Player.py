
#Imports
import os
import time
import random
import msvcrt
import json
import threading as thread

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame

#Variables
welcome_message = "Welcome to Python AT Audio Player!"
shuffle = False
shuffleIndefinitely = True
shouldRun = True

#Classes
class NowPlaying:
	def __init__(self, name, path):
		self.name = name
		self.path = path

nowPlaying = NowPlaying(None, None)

#Functions
def eventGet(files):
	while shouldRun:
		#for event in pygame.event.get():
		#	if event.type == SONG_END:
		#		print("the song ended!")
		if not pygame.mixer_music.get_busy():
			if files:
				if shuffle:
					index = random.randint(0, len(files)-1)
				else:
					index = -1
				pygame.mixer_music.load(files[index].path)

				#Change nowPlaying object
				nowPlaying.name = files[index].name
				nowPlaying.path = files[index].path

				pygame.mixer_music.play()
				if (not shuffle) or (shuffle and not shuffleIndefinitely):
					files.pop(index)
			else:

				#Change nowPlaying object
				nowPlaying.name = None
				nowPlaying.path = None

#Welcome the user
print("\n" + welcome_message)
for var in welcome_message:
	print("-", end="", flush=True)
	time.sleep(0.02)
print()

#Initalization
pygame.mixer.init()
#SONG_END = pygame.USEREVENT + 1
#pygame.mixer.music.set_endevent(SONG_END)

while shouldRun:
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
			nowPlaying.name = os.path.basename(new_song)
			nowPlaying.path = os.path.abspath(new_song)
			print("New song loaded!")
		else:
			print("Song doesn't exist.")

	if char == "i":
		playlist = input("Type the playlist here: ")
		if os.path.exists(playlist):
			#if os.path.isfile(playlist):
			#	with open(playlist) as file:
			#		data = json.load(file)
			#	pygame.mixer.music.load(data["songs"][-1])
			#	for entry in data["songs"][-2::-1]:
			#		pygame.mixer.music.queue(entry)
			#	pygame.mixer.music.play()
			if os.path.isdir(playlist):
				files = []
				for entry in os.scandir(playlist):
					if entry.is_file() and entry.name.endswith(".mp3"):
						files.append(NowPlaying(entry.name, entry.path))
				if len(files) > 0:
					thread._start_new_thread(eventGet, (files,))
				else:
					print("Directory does not contain any mp3-files")
		else:
			print("Playlist doesn't exist.")

	if char == "r":
		pygame.mixer.music.rewind()
		print("Rewind!")

	if char == "n":
		print("\nNOW PLAYING:")
		print("Name: {}".format(nowPlaying.name))
		print("Path: {}".format(nowPlaying.path))

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
		print("s - Change shuffle setting.")
		print("i - Change shuffle indefinitely setting.")
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
		if char2 == "s":
			print("Shuffle setting is currently set to {}. ".format(shuffle), end="")
			while True:
				newShuffleStr = input("Type 'True' or 'False': ")
				if newShuffleStr == "True":
					newShuffle = True
				elif newShuffleStr == "False":
					newShuffle = False
				else:
					print("Not convertible to bool. Try again!")
					continue
				break
			shuffle = newShuffle
			print("New shuffle settings: {}!\n".format(shuffle))
		if char2 == "i":
			print("Shuffle indefinitely setting is currently set to {}. ".format(shuffleIndefinitely), end="")
			while True:
				newShuffleIndefinitelyStr = input("Type 'True' or 'False': ")
				if newShuffleIndefinitelyStr == "True":
					newShuffleIndefinitely = True
				elif newShuffleIndefinitelyStr == "False":
					newShuffleIndefinitely = False
				else:
					print("Not convertible to bool. Try again!")
					continue
				break
			shuffleIndefinitely = newShuffleIndefinitely
			print("New shuffle indefinitely settings: {}!\n".format(shuffleIndefinitely))

	if char == "e":
		print("Bye!\n")
		shouldRun = False
	if char == "q":
		print("Goodbye!\n")
		shouldRun = False

pygame.mixer.quit()