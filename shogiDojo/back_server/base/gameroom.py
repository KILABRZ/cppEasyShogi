from ShogiAgent import Shogi

class GameRoom:

	def __init__(self, token_a, token_b, player_a, player_b):

		self.shogi = Shogi()
		self.shogi.BoardInit()

		self.token_a = token_a
		self.token_b = token_b
		self.player_a = player_a
		self.player_b = player_b

		self.ready_a = False
		self.ready_b = False
		self.end_a = False
		self.end_b = False

		self.chesser_token = token_a
		self.another_token = token_b
		self.winner_token = None
		self.loser_token = None
		self.gameround = 0

		self.state = 'WaitReady'
		self.movelist = list()
		self.boardstate = ''

	def GetReady(self, token):
		if self.state != 'WaitReady':
			return 'StateError,' + self.state

		if token == self.token_a:
			self.ready_a = True
		elif token == self.token_b:
			self.ready_b = True
		else:
			return 'TokenError'

		if self.ready_a and self.ready_b:
			self.movelist = self.shogi.StrFetchMoves(1)
			self.boardstate = self.shogi.StrSaveBoardState()
			self.state = 'WaitMove'

		return 'MatchInfo,{},{}'.format(self.player_a.Extract(), self.player_b.Extract())

	def FetchGameState(self):

		if self.state == 'WaitEnd':
			return 'Winloss,{},{},{}'.format(self.winner_token, self.loser_token, self.boardstate)
		if self.state != 'WaitMove':
			return 'StateError,' + self.state
		else:
			movedump = ':'.join(self.movelist)
			return 'WaitMove,{},{},{}'.format(self.chesser_token, self.boardstate, movedump)

	def MakeMove(self, move):

		if self.state != 'WaitMove':
			return 'StateError,' + self.state

		if move not in self.movelist:
			return 'MoveError'

		self.shogi.StrMakeMove(move)
		self.gameround += 1

		self.movelist = self.shogi.StrFetchMoves(1)
		self.boardstate = self.shogi.StrSaveBoardState()

		if len(self.movelist) == 0:
			self.winner_token = self.chesser_token
			self.loser_token = self.another_token
			self.state = 'WaitEnd'
			if self.winner_token == self.token_a:
				self.end_a = True
			else:
				self.end_b = True
			return 'Winloss,{},{},{}'.format(self.winner_token, self.loser_token, self.boardstate)
		else:
			self.chesser_token, self.another_token = self.another_token, self.chesser_token
			return 'MoveSuccess'

	def GetEnd(self, token):
		if self.state != 'WaitEnd':
			return 'StateError,' + self.state

		if token == self.token_a:
			self.end_a = True
		elif token == self.token_b:
			self.end_b = True
		else:
			return 'TokenError'

		if self.end_a and self.end_b:
			self.state = 'GameOver'
		
		return 'GameOver'