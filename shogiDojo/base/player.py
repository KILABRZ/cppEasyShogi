class Player:
	def __init__(self, token, name, rank, state):
		self.token = token
		self.name = name
		self.state = state
		self.rank = rank
		self.create_time = time()
	
	def Extract(self):
		return '{}({})'.format(name, rank)