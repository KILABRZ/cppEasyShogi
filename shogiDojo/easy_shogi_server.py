import sys
import os
import queue
import threading
import gc
from time import time
import socket
from base.gameroom import GameRoom
from base.player import Player

all_players = dict()
token_gameroom = dict()

def WriteLog(msg):
	print(msg)
def CreateToken(nlen):
	return os.urandom(nlen).hex().upper()
	# Can be changed to a meaningful token format

class HelloThread(threading.Thread):
	def __init__(self, host, port, chost, cport):
		threading.Thread.__init__(self)
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.bind((host, port))
		self.server.listen(100)
		self.chost = chost
		self.cport = cport
		self.timeout = 5000


	def run(self):
		while True:
			client, addr = self.server.accept()
			client.settimeout(self.waittime)

			try:
				msg = str(client.recvall(), 'utf-8')
			except socket.timeout:
				WriteLog('Hello Server Timeout')
				continue

			try:
				hello_header, pname, prank = msg.split(',')
			except:
				WriteLog('Hello Message Format Error')
				continue

			token = CreateToken(8)
			player = Player(token, pname, prank, 'Unregistered')
			all_players[token] = player

			return_msg = 'NextConnect,{},{},{}'.format(token, self.chost, self.cport)
			client.sendall(bytes(return_msg, 'utf-8'))

class CoreThread(threading.Thread):
	def __init__(self, host, port):
		threading.Thread.__init__(self)
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.bind((host, port))
		self.server.listen(100)
		self.timeout = 5000

	def run(self):
		while True:
			client, addr = self.server.accept()
			client.settimeout(self.timeout)

			try:
				msg = str(client.recvall(), 'utf-8')
			except socket.timeout:
				WriteLog('Core Server Timeout')
				continue

			try:
				header, token, content = msg.split(',')
			except:
				WriteLog('Core Message Format Error')	
				continue

				# Parsing Message

				if token not in all_players.keys():
					WriteLog('Invalid Token')
					continue
				player = all_players[token]

				if header == 'SecondHello':

					if player.state != 'Unregistered':
						WriteLog('Core Player State Error')
						client.sendall(b'StateError')
						continue

					player.state = 'Idle'
					client.sendall(b'Hello')

				elif header == 'Register':
					
					if player.state != 'Idle':
						WriteLog('Core Player State Error')
						client.sendall(b'StateError')
						continue

					player.state = 'Registered'
					client.sendall(b'Accept')

				elif header == 'Cancel':

					if player.state != 'Registered':
						WriteLog('Core Player State Error')
						client.sendall(b'StateError')
						continue

					player.state = 'Idle'
					client.sendall(b'Accept')

				elif header == 'GameReady':

					if player.state != 'WaitReady':
						WriteLog('Core Player State Error')
						client.sendall(b'StateError')
						continue

					gr = token_gameroom[token]
					msg = gr.GetReady(token)
					client.sendall(bytes(msg, 'utf-8'))
					player.state = 'InGame'

				elif header == 'FetchState':

					if player.state != 'InGame':
						WriteLog('Core Player State Error')
						client.sendall(b'StateError')
						continue

					gr = token_gameroom[token]
					msg = gr.FetchGameState()
					client.sendall(bytes(msg, 'utf-8'))

				elif header == 'MakeMove':

					if player.state != 'InGame':
						WriteLog('Core Player State Error')
						client.sendall(b'StateError')
						continue

					gr = token_gameroom[token]
					msg = gr.MakeMove(content)
					client.sendall(bytes(msg, 'utf-8'))

				elif header == 'EndGame':

					if player.state != 'InGame':
						WriteLog('Core Player State Error')
						client.sendall(b'StateError')
						continue

					gr = token_gameroom[token]
					msg = gr.GetEnd(token)
					client.sendall(bytes(msg, 'utf-8'))
					del token_gameroom[token]
					gc.collect()


hello_host = '127.0.0.1'
hello_port = 8000

core_host = '127.0.0.1'
core_port = 8001

ct = CoreThread(core_host, core_port)
ct.start()
ht = HelloThread(hello_host, hello_port, core_host, core_port)
ht.start()

# match up system

while True:

	servertime = time()
	max_state_waittime = 5
	
	matchup_list = list()

	for token, player in all_players.items():

		if player.state == 'Unregistered' and servertime - player.create_time >= max_state_waittime:
			del all_players[token]

		if player.state == 'Register':
			matchup_list.append(player)

	sort(matchup_list, key = lambda x : x.rank)
	sort(matchup_list, key = lambda x : x.state)

	for i in range(0, len(matchup_list), 2):

		if matchup_list[i].state == 'Unregistered' or matchup_list[i+1].state == 'Unregistered':
			break

		p1 = matchup_list[i]
		p2 = matchup_list[i+1]

		gr = GameRoom(p1.token, p2.token, p1, p2)
		
		token_gameroom[p1.token] = gr
		token_gameroom[p2.token] = gr

		p1.state = 'WaitReady'
		p2.state = 'WaitReady'
		