
#Check if script is called as a module in another script or not
import sys
if __name__ != "__main__":
	print("\nThis program should be called as a standalone script!")
	sys.exit(1)


#Imports
import os
import time
import random
import json
import keyboard
import msvcrt
import threading as thread

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame

#Variables
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

#Initalization
pygame.init()
SONG_END = pygame.USEREVENT + 1					#Song end event
pygame.mixer_music.set_endevent(SONG_END)

#Classes
class Globals:
	def __init__(self):
		self.shouldRunB = True
globals = Globals()
class PressedCharacter:
	def __init__(self):
		self.char = None
		self.globalChar = keyboard.KeyboardEvent(None, None)
pressedCharacter = PressedCharacter()
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
		self.logged = False
		if self.path != None:
			self.length = int(pygame.mixer.Sound(self.path).get_length())
		else:
			self.length = None

nowPlaying = NowPlaying(None, None, None)		#Standard object for the current playing song

#Functions
def getChar():
	while shouldRun:
		pressedCharacter.char = msvcrt.getwch()
def loading_animation(str):
	array = ["/", "—", "\\", "|"]
	n = 0
	while loadingAnimation:
		print("\r" + array[n] + " " + str, end="")
		time.sleep(0.15)
		if n == 3:
			n = 0
		else:
			n += 1
	sys.exit(0)
def wait():
	pressedCharacter.char = ""
	time.sleep(0.2)
def play(files):
	if len(files) == 1:
		type = "song"
	elif len(files) > 1:
		type = "playlist"
	while shouldRun:
		if globals.shouldRunB:
			for event in pygame.event.get():
				if event.type == SONG_END:
					saveFiles() #Save files
					if loopSong:
						pygame.mixer_music.play()

						#Add to log
						songLog.append(nowPlaying.path)
					else:
						nowPlaying.paused = None
		else:
			globals.shouldRunB = True
		if (not pygame.mixer_music.get_busy()) and (nowPlaying.paused == None):
			if files:
				index = -1
				if (type == "playlist"):
					while True:
						if options["shuffle"]:
							index = random.randint(0, len(files)-1)
						if nowPlaying.path != files[index].path: break
						if not options["shuffle"]: break
				pygame.mixer_music.load(files[index].path)

				#Change nowPlaying object
				nowPlaying.name = files[index].name
				nowPlaying.path = files[index].path
				nowPlaying.paused = False
				nowPlaying.length = files[index].length
				nowPlaying.positionOffset = files[index].positionOffset
				nowPlaying.logged = False

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
				nowPlaying.logged = False
				
				sys.exit(0)		#Exit thread

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

#Prepare some things
with open(scriptDir + "/play.log","a") as file:
	file.write(time.strftime("#time: %Y-%m-%d %H:%M:%S\n", time.localtime(time.time())))

#Start standard song
if options["onStartSong"] != None:
	files = []
	loadingAnimation = True
	thread._start_new_thread(loading_animation, ("Loading playlist!",))
	for entry in os.scandir(options["onStartSong"]["path"]):
		if entry.is_file() and entry.name.endswith(".mp3"):
			files.append(NowPlaying(entry.name, entry.path, False))
	loadingAnimation = False
	print("\r", end="")
	if len(files) > 0:
		thread._start_new_thread(play, (files,))
		print("Standard playlist loaded!")
	else:
		print("Error")

#Define saveFiles function
def saveFiles():
	#Save options
	options["volume"] = pygame.mixer_music.get_volume()
	with open(scriptDir + "/options.json","w") as file:
		json.dump(options, file, indent=4)

	#Append played songs to log
	with open(scriptDir + "/play.log","a") as file:
		if not nowPlaying.logged:
			file.write(nowPlaying.path + "\n")
			nowPlaying.logged = True

thread._start_new_thread(getChar, ())
def setGlobalChar(event):
	pressedCharacter.globalChar = event
keyboard.on_press(setGlobalChar)
while shouldRun:

	p_event = (pressedCharacter.char == "p") or ((keyboard.is_pressed(76)) and (pressedCharacter.globalChar.is_keypad))		#Trigger on "p" inside console or "5" on numpad (globally)
	s_event = (pressedCharacter.char == "s") or ((keyboard.is_pressed(77)) and (pressedCharacter.globalChar.is_keypad))		#Trigger on "s" inside console or "6" on numpad (globally)
	l_event = pressedCharacter.char == "l"
	i_event = pressedCharacter.char == "i"
	u_event = pressedCharacter.char == "u"
	r_event = (pressedCharacter.char == "r") or ((keyboard.is_pressed(75)) and (pressedCharacter.globalChar.is_keypad))		#Trigger on "r" inside console or "4" on numpad (globally)
	t_event = (pressedCharacter.char == "t") or ((keyboard.is_pressed(71)) and (pressedCharacter.globalChar.is_keypad))		#Trigger on "t" inside console or "7" on numpad (globally)
	g_event = pressedCharacter.char == "g"
	j_event = pressedCharacter.char == "j"
	n_event = (pressedCharacter.char == "n") or ((keyboard.is_pressed(72)) and (pressedCharacter.globalChar.is_keypad))		#Trigger on "n" inside console or "8" on numpad (globally)
	h_event = pressedCharacter.char == "h"
	c_event = pressedCharacter.char == "c"
	o_event = pressedCharacter.char == "o"
	e_event = (pressedCharacter.char == "e") or ((keyboard.is_pressed(79)) and (pressedCharacter.globalChar.is_keypad))		#Trigger on "e" inside console or "1" on numpad (globally)
	q_event = pressedCharacter.char == "q"

	if p_event and pygame.mixer_music.get_busy():
		pygame.mixer_music.pause()
		nowPlaying.paused = True
		print("Paused!")
		wait()
	elif p_event and not pygame.mixer_music.get_busy():
		pygame.mixer_music.unpause()
		nowPlaying.paused = False
		print("Unpaused!")
		wait()

	if s_event:
		loopSongTmp = loopSong
		loopSong = False
		pygame.mixer_music.stop()
		pygame.mixer_music.unload()
		nowPlaying.paused = None

		globals.shouldRunB = False
		while not globals.shouldRunB:
			pass
		if loopSongTmp:
			loopSong = True
		globals.shouldRunB = True

		print("Stopped/Next!")
		wait()

	if l_event:
		new_song = input("Type the new song here: ")
		if os.path.exists(new_song):
			loadingAnimation = True
			thread._start_new_thread(loading_animation, ("Loading song!",))
			song = [NowPlaying(os.path.basename(new_song), os.path.abspath(new_song), False)]
			loadingAnimation = False
			print("\r", end="")
			thread._start_new_thread(play, (song,))
			print("Song was loaded!")
		else:
			print("Song doesn't exist.")
		wait()

	if i_event:
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
				loadingAnimation = True
				thread._start_new_thread(loading_animation, ("Loading playlist!",))
				for entry in os.scandir(playlist):
					if entry.is_file() and entry.name.endswith(".mp3"):
						files.append(NowPlaying(entry.name, entry.path, False))
				loadingAnimation = False
				print("\r", end="")
				if len(files) > 0:
					thread._start_new_thread(play, (files,))
					print("Playlist was loaded!")
				else:
					print("Directory does not contain any mp3-files")
		else:
			print("Playlist doesn't exist.")
		wait()

	if u_event:
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
		wait()

	if r_event:
		pygame.mixer_music.play()
		if nowPlaying.paused:
			pygame.mixer_music.pause()
			nowPlaying.paused = True
		print("Rewind!")
		wait()

	if t_event:
		if loopSong:
			print("Stop looping current track.")
			loopSong = False
		else:
			print("Looping current track!")
			loopSong = True
		wait()
		
	if g_event:
		print("\nGOTO POSITION:")
		seconds = int(pygame.mixer_music.get_pos() / 1000) + nowPlaying.positionOffset
		print("Position is currently: {}:{}{}. ".format(seconds // 60, (lambda int: "0" if int < 10 else "")(seconds % 60), seconds % 60), end="")
		while True:
			newPositionStr = input("New position (seconds): ")
			try:
				if newPositionStr == "e":
					newPosition = -1
				else:
					newPosition = int(newPositionStr)
			except ValueError:
				print("Not convertible to int. Try again!")
				continue
			break
		if newPosition != -1:
			position = newPosition
			pygame.mixer_music.play(start=position)
			nowPlaying.positionOffset = position
			if nowPlaying.paused:
				pygame.mixer_music.pause()
				nowPlaying.paused = True
			print("Go to {} seconds!".format(position))
		wait()

	if j_event:
		print("\nJOURNAL:")
		for song in songLog:
			print(song)
		wait()

	if n_event:
		print("\nNOW PLAYING:")
		if not nowPlaying.path:
			print("No song is currently loaded!")
		else:
			print("Name: {}".format(nowPlaying.name))
			print("Path: {}".format(nowPlaying.path))
			print("Paused: {}".format(nowPlaying.paused))
			print("Looping: {}".format(loopSong))
			print("Length: {}:{}{}".format(nowPlaying.length // 60, (lambda int: "0" if int < 10 else "")(nowPlaying.length % 60), nowPlaying.length % 60))
			seconds = int(pygame.mixer_music.get_pos() / 1000) + nowPlaying.positionOffset
			print("Position: {}:{}{}".format(seconds // 60, (lambda int: "0" if int < 10 else "")(seconds % 60), seconds % 60))
		wait()

	if h_event:
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
		wait()

	if c_event:
		print("\nCREDITS:")
		print("Author: Isak Brynielsson Neri")
		print("Libs:")
		print("    Pygame: {}".format(pygame.version.ver))
		print()
		wait()

	if o_event:
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
		saveFiles()
		wait()

	if e_event:
		print("Bye!\n")
		shouldRun = False
	if q_event:
		print("Goodbye!\n")
		shouldRun = False

saveFiles()

#Uninit mixer
pygame.mixer.quit()