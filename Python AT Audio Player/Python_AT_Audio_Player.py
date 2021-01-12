
#Imports
import sys
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
scriptDir = os.path.dirname(os.path.realpath(__file__))
shouldRun = True
loopSong = False
songLog = []
standardOptions = {
	"shuffle": False,
	"shuffleIndefinitely": True,
	"volume": 1,
	"onStartSong": None
}

#Initalization
pygame.init()
SONG_END = pygame.USEREVENT + 1					#Song end event
pygame.mixer_music.set_endevent(SONG_END)

#Classes
class PlaylistData:
	def __init__(self):
		self.shuffleIndefinitely = False
		self.files = []
class NowPlaying:
	def __init__(self, name, path, paused):
		self.name = name
		self.path = path
		self.paused = paused
		self.positionOffset = 0
		self.playlist = None
		if self.path != None:
			self.length = int(pygame.mixer.Sound(self.path).get_length())
		else:
			self.length = None

nowPlaying = NowPlaying(None, None, None)

#Functions
def play(files):
	if len(files) == 1:
		type = "song"
	elif len(files) > 1:
		type = "playlist"
	while shouldRun:
		for event in pygame.event.get():
			if event.type == SONG_END:
				if loopSong:
					pygame.mixer_music.play()

					#Add to log
					songLog.append(nowPlaying.path)
				else:
					nowPlaying.paused = None
		if (not pygame.mixer_music.get_busy()) and (nowPlaying.paused == None):
			if files:
				if options["shuffle"]:
					index = random.randint(0, len(files)-1)
				else:
					index = -1
				pygame.mixer_music.load(files[index].path)

				#Change nowPlaying object
				nowPlaying.name = files[index].name
				nowPlaying.path = files[index].path
				nowPlaying.paused = False
				nowPlaying.length = files[index].length
				nowPlaying.positionOffset = files[index].positionOffset

				#Add to log
				songLog.append(files[index].path)

				pygame.mixer_music.play()
				if (not options["shuffle"]) or (options["shuffle"] and not options["shuffleIndefinitely"]):
					files.pop(index)
				if (type == "playlist"):
					nowPlaying.playlist = PlaylistData()
					if (options["shuffle"]) and (options["shuffleIndefinitely"]):
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
				
				sys.exit(0)

#Welcome the user
print("\n" + welcome_message)
for var in welcome_message:
	print("-", end="", flush=True)
	time.sleep(0.02)
print()

#Load options
if (os.path.exists(scriptDir + "/options.json")) and (os.path.isfile(scriptDir + "/options.json")):
	with open(scriptDir + "/options.json","r") as file:
		options = json.load(file)
	print("Options loaded!")
else:
	options = standardOptions
	print("Options file could not be found...")
pygame.mixer_music.set_volume(options["volume"])

#Start standard song
if options["onStartSong"] != None:
	files = []
	for entry in os.scandir(options["onStartSong"]["path"]):
		if entry.is_file() and entry.name.endswith(".mp3"):
			files.append(NowPlaying(entry.name, entry.path, False))
	if len(files) > 0:
		thread._start_new_thread(play, (files,))
		print("Standard playlist loaded!")
	else:
		print("Error")

while shouldRun:
	char = msvcrt.getwch()

	if char == "p" and pygame.mixer_music.get_busy():
		pygame.mixer_music.pause()
		nowPlaying.paused = True
		print("Paused!")
	elif char == "p" and not pygame.mixer_music.get_busy():
		pygame.mixer_music.unpause()
		nowPlaying.paused = False
		print("Unpaused!")

	if char == "s":
		pygame.mixer_music.stop()
		pygame.mixer_music.unload()
		nowPlaying.paused = None
		print("Stopped/Next!")

	if char == "l":
		new_song = input("Type the new song here: ")
		if os.path.exists(new_song):
			song = [NowPlaying(os.path.basename(new_song), os.path.abspath(new_song), False)]
			thread._start_new_thread(play, (song,))
			print("Song loaded!")
		else:
			print("Song doesn't exist.")

	if char == "i":
		playlist = input("Type the playlist here: ")
		if os.path.exists(playlist):
			#if os.path.isfile(playlist):
			#	with open(playlist) as file:
			#		data = json.load(file)
			#	pygame.mixer_music.load(data["songs"][-1])
			#	for entry in data["songs"][-2::-1]:
			#		pygame.mixer_music.queue(entry)
			#	pygame.mixer_music.play()
			if os.path.isdir(playlist):
				files = []
				for entry in os.scandir(playlist):
					if entry.is_file() and entry.name.endswith(".mp3"):
						files.append(NowPlaying(entry.name, entry.path, False))
				if len(files) > 0:
					thread._start_new_thread(play, (files,))
					print("Playlist loaded!")
				else:
					print("Directory does not contain any mp3-files")
		else:
			print("Playlist doesn't exist.")

	if char == "u":
		print("\nREMAINING SONGS:")
		if not nowPlaying.path:
			print("No song is currently loaded!")
		else:
			if nowPlaying.playlist == None:
				print("A song is currently playing!")
			elif nowPlaying.playlist.shuffleIndefinitely:
				print("The playlist will shuffle indefinitely!")
			else:
				if len(nowPlaying.playlist.files) > 0:
					for song in nowPlaying.playlist.files:
						print(song.name)
				else:
					print("The current playing song is the last one.")

	if char == "r":
		pygame.mixer_music.play()
		print("Rewind!")

	if char == "t":
		if loopSong:
			print("Stop looping current track.")
			loopSong = False
		else:
			print("Looping current track!")
			loopSong = True
		
	if char == "g":
		print("\nGOTO POSITION:")
		position = int(input("Position (seconds): "))
		pygame.mixer_music.play(start=position)
		nowPlaying.positionOffset = position
		if nowPlaying.paused:
			pygame.mixer_music.pause()
			nowPlaying.paused = True

	if char == "n":
		print("\nNOW PLAYING:")
		if not nowPlaying.path:
			print("No song is currently loaded!")
		else:
			print("Name: {}".format(nowPlaying.name))
			print("Path: {}".format(nowPlaying.path))
			print("Paused: {}".format(nowPlaying.paused))
			print("Length: {}:{}".format(nowPlaying.length // 60, nowPlaying.length % 60))
			seconds = int(pygame.mixer_music.get_pos() / 1000) + nowPlaying.positionOffset
			print("Position: {}:{}".format(seconds // 60, seconds % 60))

	if char == "h":
		print("\nHELP:")
		print("p - Pause/Unpause")
		print("s - Stop")
		print("l - Load new song")
		print("i - Load playlist (folder)")
		print("r - Rewind")
		print("g - Goto position in song")
		print("n - View information about the current playing song")
		print("o - Change options")
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
		print("\nOPTIONS:")
		print("v - Change volume!")
		print("s - Change shuffle setting.")
		print("i - Change shuffle indefinitely setting.")
		char2 = msvcrt.getwch()
		if char2 == "v":
			print("Volume is currently set to {}/100. ".format(pygame.mixer_music.get_volume() * 100), end="")
			while True:
				newVolumeStr = input("Type a value between 0 and 100: ")
				try:
					if newVolumeStr == "e":
						newVolume = pygame.mixer_music.get_volume() * 100;
					else:
						newVolume = float(newVolumeStr)
				except ValueError:
					print("Not convertible to float. Try again!")
					continue
				if newVolume > 100:
					print("You entered a value which was too big. Volume will be set to max (100)")
				elif newVolume < 0:
					print("You entered a value which was too low. Volume will be set to minimum (0)")
				break
			pygame.mixer_music.set_volume(newVolume / 100)
			print("New volume set to {}!\n".format(pygame.mixer_music.get_volume() * 100))
		if char2 == "s":
			print("Shuffle setting is currently set to {}. ".format(options["shuffle"]), end="")
			while True:
				newShuffleStr = input("Type 'True' or 'False': ")
				if newShuffleStr == "True":
					newShuffle = True
				elif newShuffleStr == "False":
					newShuffle = False
				elif newShuffleStr == "e":
					newShuffle = options["shuffle"]
				else:
					print("Not convertible to bool. Try again!")
					continue
				break
			options["shuffle"] = newShuffle
			print("New shuffle settings: {}!\n".format(options["shuffle"]))
		if char2 == "i":
			print("Shuffle indefinitely setting is currently set to {}. ".format(options["shuffleIndefinitely"]), end="")
			while True:
				newShuffleIndefinitelyStr = input("Type 'True' or 'False': ")
				if newShuffleIndefinitelyStr == "True":
					newShuffleIndefinitely = True
				elif newShuffleIndefinitelyStr == "False":
					newShuffleIndefinitely = False
				elif newShuffleIndefinitelyStr == "e":
					newShuffleIndefinitely = options["shuffleIndefinitely"]
				else:
					print("Not convertible to bool. Try again!")
					continue
				break
			options["shuffleIndefinitely"] = newShuffleIndefinitely
			print("New shuffle indefinitely settings: {}!\n".format(options["shuffleIndefinitely"]))

	if char == "e":
		print("Bye!\n")
		shouldRun = False
	if char == "q":
		print("Goodbye!\n")
		shouldRun = False

#Save options
options["volume"] = pygame.mixer_music.get_volume()
with open(scriptDir + "/options.json","w") as file:
	json.dump(options, file, indent=4)

#Append played songs to log
with open(scriptDir + "/play.log","a") as file:
	file.write(time.strftime("#time: %Y-%m-%d %H:%M:%S\n", time.localtime(time.time())))
	for song in songLog:
		file.write(song + "\n")

#Uninit mixer
pygame.mixer.quit()