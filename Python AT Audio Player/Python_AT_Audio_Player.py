
#Imports
import pygame
import sys
import time
import msvcrt

#Variables
welcome_message = "Welcome to Python AT Audio Player!"

#Welcome the user
print(welcome_message)
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
		print("Stopped!")

	if char == "l":
		new_song = input("Type the new song here: ")
		pygame.mixer.music.load(new_song)
		pygame.mixer.music.play()
		print("New song loaded!")