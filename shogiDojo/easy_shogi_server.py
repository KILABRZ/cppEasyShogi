import sys
import os
import queue
import threading
from time import time
from socket import socket, AF_INET, SOCK_STREAM
from base.gameroom import GameRoom
from base.player import Player

all_players = dict()

class HelloThread(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
	def run(self):
		# work do in here
		pass 

class CoreThread(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
	def run(self):
		pass

ht = HelloThread()
ht.start()

ct = CoreThread()
ct.start()

# match up system