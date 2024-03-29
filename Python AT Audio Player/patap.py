
#Check if script is called as a module in another script or not
import sys
if __name__ != "__main__":
	print("\nPython AT Audio Player should be called as a standalone script!")
	sys.exit(0)


#Imports
import os
import time
import random
import json
import keyboard
import msvcrt
import threading as thread

from patapfuncs import *
import patapvars
from patapclasses import *

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame
pygame.mixer_music.set_endevent(pygame.USEREVENT + 1)		#Song end event

#Welcome the user
print("\n" + patapvars.welcome_message)
for var in patapvars.welcome_message:
	print("-", end="", flush=True)
	time.sleep(0.02)
print()

#Prepare some things
with open(patapvars.scriptDir + "/play.log","a") as file:
	file.write(time.strftime("#time: %Y-%m-%d %H:%M:%S\n", time.localtime(time.time())))

#Start standard song
if patapvars.options["onStartSong"] != None:
	files = []
	patapvars.loadingAnimation = True
	thread._start_new_thread(loading_animation, ("Loading playlist!",))
	for entry in os.scandir(patapvars.options["onStartSong"]["path"]):
		if entry.is_file() and entry.name.endswith(".mp3"):
			files.append(NowPlaying(entry.name, entry.path, False))
	patapvars.loadingAnimation = False
	print("\r", end="")
	if len(files) > 0:
		thread._start_new_thread(play, (files,))
		print("Standard playlist loaded!")
	else:
		print("Error")

thread._start_new_thread(getChar, ())
keyboard.on_press(setGlobalChar)
while patapvars.shouldRun:

	if (keyboard.is_pressed(73)) and (pressedCharacter.globalChar.is_keypad):
		pygame.mixer_music.set_volume(pygame.mixer_music.get_volume()+0.01)
		if (globals.lastCommandWasVolume):
			print("\rVolume set to {} ".format(int(pygame.mixer_music.get_volume()*100)), end="")
		else:
			print("Volume set to {} ".format(int(pygame.mixer_music.get_volume()*100)), end="", flush=True)
			globals.lastCommandWasVolume = True
		wait(0.1)
	elif (keyboard.is_pressed(81)) and (pressedCharacter.globalChar.is_keypad):
		pygame.mixer_music.set_volume(pygame.mixer_music.get_volume()-0.01)
		if (globals.lastCommandWasVolume):
			print("\rVolume set to {} ".format(int(pygame.mixer_music.get_volume()*100)), end="")
		else:
			print("Volume set to {} ".format(int(pygame.mixer_music.get_volume()*100)), end="", flush=True)
			globals.lastCommandWasVolume = True
		wait(0.1)

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
		lineBreak()
		pygame.mixer_music.pause()
		nowPlaying.paused = True
		print("Paused!")
		wait()
	elif p_event and not pygame.mixer_music.get_busy():
		lineBreak()
		pygame.mixer_music.unpause()
		nowPlaying.paused = False
		print("Unpaused!")
		wait()

	if s_event:
		lineBreak()
		loopSongTmp = patapvars.loopSong
		patapvars.loopSong = False
		pygame.mixer_music.stop()
		pygame.mixer_music.unload()
		nowPlaying.paused = None

		globals.shouldRunB = False
		while not globals.shouldRunB:
			pass
		if loopSongTmp:
			patapvars.loopSong = True
		globals.shouldRunB = True

		print("Stopped/Next!")
		wait()

	if l_event:
		lineBreak()
		new_song = input("Type the new song here: ")
		if os.path.exists(new_song):
			patapvars.loadingAnimation = True
			thread._start_new_thread(loading_animation, ("Loading song!",))
			song = [NowPlaying(os.path.basename(new_song), os.path.abspath(new_song), False)]
			patapvars.loadingAnimation = False
			print("\r", end="")
			thread._start_new_thread(play, (song,))
			print("Song was loaded!")
		else:
			print("Song doesn't exist.")
		wait()

	if i_event:
		lineBreak()
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
				patapvars.loadingAnimation = True
				thread._start_new_thread(loading_animation, ("Loading playlist!",))
				for entry in os.scandir(playlist):
					if entry.is_file() and entry.name.endswith(".mp3"):
						files.append(NowPlaying(entry.name, entry.path, False))
				patapvars.loadingAnimation = False
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
		lineBreak()
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
		lineBreak()
		pygame.mixer_music.play()
		if nowPlaying.paused:
			pygame.mixer_music.pause()
			nowPlaying.paused = True
		print("Rewind!")
		wait()

	if t_event:
		lineBreak()
		if patapvars.loopSong:
			print("Stop looping current track.")
			patapvars.loopSong = False
		else:
			print("Looping current track!")
			patapvars.loopSong = True
		wait()
		
	if g_event:
		lineBreak()
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
		lineBreak()
		print("\nJOURNAL:")
		for song in patapvars.songLog:
			print(song)
		wait()

	if n_event:
		lineBreak()
		print("\nNOW PLAYING:")
		if not nowPlaying.path:
			print("No song is currently loaded!")
		else:
			print("Name: {}".format(nowPlaying.name))
			print("Path: {}".format(nowPlaying.path))
			print("Paused: {}".format(nowPlaying.paused))
			print("Looping: {}".format(patapvars.loopSong))
			print("Length: {}:{}{}".format(nowPlaying.length // 60, (lambda int: "0" if int < 10 else "")(nowPlaying.length % 60), nowPlaying.length % 60))
			seconds = int(pygame.mixer_music.get_pos() / 1000) + nowPlaying.positionOffset
			print("Position: {}:{}{}".format(seconds // 60, (lambda int: "0" if int < 10 else "")(seconds % 60), seconds % 60))
		wait()

	if h_event:
		lineBreak()
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
		lineBreak()
		print("\nCREDITS:")
		print("Author: Isak Brynielsson Neri")
		print("Libs:")
		print("    Pygame: {}".format(pygame.version.ver))
		print()
		wait()

	if o_event:
		lineBreak()
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
			print("Shuffle setting is currently set to {}. ".format(patapvars.options["shuffle"]), end="")
			while True:
				newShuffleStr = input("Type 'True' or 'False': ")
				if newShuffleStr == "True":
					newShuffle = True
				elif newShuffleStr == "False":
					newShuffle = False
				elif newShuffleStr == "e":
					newShuffle = patapvars.options["shuffle"]
				else:
					print("Not convertible to bool. Try again!")
					continue
				break
			patapvars.options["shuffle"] = newShuffle
			print("New shuffle settings: {}!\n".format(patapvars.options["shuffle"]))
		if char2 == "i":
			print("Shuffle indefinitely setting is currently set to {}. ".format(patapvars.options["shuffleIndefinitely"]), end="")
			while True:
				newShuffleIndefinitelyStr = input("Type 'True' or 'False': ")
				if newShuffleIndefinitelyStr == "True":
					newShuffleIndefinitely = True
				elif newShuffleIndefinitelyStr == "False":
					newShuffleIndefinitely = False
				elif newShuffleIndefinitelyStr == "e":
					newShuffleIndefinitely = patapvars.options["shuffleIndefinitely"]
				else:
					print("Not convertible to bool. Try again!")
					continue
				break
			patapvars.options["shuffleIndefinitely"] = newShuffleIndefinitely
			print("New shuffle indefinitely settings: {}!\n".format(patapvars.options["shuffleIndefinitely"]))
		saveFiles()
		wait()

	if e_event:
		lineBreak()
		print("Bye!\n")
		patapvars.shouldRun = False
	if q_event:
		lineBreak()
		print("Goodbye!\n")
		patapvars.shouldRun = False

saveFiles()

#Uninit mixer
pygame.mixer.quit()