import socket
import sys
import threading
import os

#####
#####
#####

# Parsing Network parameters

try:
	hello_hostport, core_hostport = sys.argv[1], sys.argv[2]
	hello_host, hello_port = hello_hostport.split(':')
	hello_port = int(hello_port)
	core_host, core_port = core_hostport.split(':')
	core_port = int(core_port)
except:
	print('usage: python3 shogiserver.py hh:hp ch:cp')
	exit()

#####
#####
#####

# Server Thread Sharing State

server_token_book = set()
token_bind_user = dict()
token_bind_gamethread = dict()
user_bind_token = dict()

#####
#####
#####

# General Functioning Method

def WriteLog(s):
	print(s)
	# todo

def VerifyPassToken(user, token):
	return True
	# todo

def GenerateToken(nlen):
	return os.urandom(nlen).hex().upper()

#####
#####
#####

# Hello Thread

def HelloCounter(hello_host, hello_port, core_host, core_port):

	try:
		hello_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		hello_server.bind((hello_host, hello_port))
		hello_server.listen(100)
	except:
		WriteLog('Hello Fail')
		exit()

	while True:
		client, addr = hello_server.accept()
		try:
			msg = str(conn.recvall(), 'utf-8')
			helloheader, passtoken, username = msg.split(',')
		except:
			WriteLog('Message Parsing Fail')
			continue

		if not VerifyPassToken(username, passtoken):
			WriteLog('Token Invalid')
			continue

		if helloheader != 'Hello':
			WriteLog('No Hello')
			continue

		gametoken = GenerateToken(8)
		
		server_token_book.add(gametoken)
		token_bind_user[gametoken] = username
		user_bind_token[username] = token

		client.sendall('Hello{},{},{}:{}'.format(username, gametoken, core_host, core_port))

hello_thread = threading.Thread(target = HelloCounter, args=(hello_host, hello_port, core_host, core_port, ))
hello_thread.run()

#####
#####
#####

# Game Thread : todo

#####
#####
#####

# Core Thread (now is raw script, it can be encapsulated into a class to do load balancing)


try:
	core_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	core_server.bind((core_host, core_port))
	core_server.listen(100)
except:
	WriteLog('Core Fail')
	exit()

while True:

	# dealing with packet format : todo