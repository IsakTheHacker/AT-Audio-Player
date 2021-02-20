import keyboard
import os

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
import pygame

class Globals:
	def __init__(self):
		self.shouldRunB = True
		self.lastCommandWasVolume = False
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