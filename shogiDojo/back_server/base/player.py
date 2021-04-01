from time import time

class Player:
	def __init__(self, token, name, rank, state):
		self.token = token
		self.name = name
		self.rank = rank
		self.state = state
		self.create_time = time()
	
	def Extract(self):
		return '{}({})'.format(name, rank)