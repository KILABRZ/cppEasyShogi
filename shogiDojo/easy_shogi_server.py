import sys
import os
import queue
import threading
from time import time
from socket import socket, AF_INET, SOCK_STREAM
from ShogiAgent import Shogi

class Player:
	def __init__(self, token, name, state):
		self.token = token
		self.name = name
		self.state = state
		self.create_time = time()

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