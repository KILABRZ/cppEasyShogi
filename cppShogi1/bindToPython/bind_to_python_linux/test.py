from ShogiAgent import *
from random import choice
from time import time


starttime = time()
N = 10000

for i in range(N):
	s = Shogi()
	s.InitialBoard()
	print('[{}/{}]'.format(i+1, N), end='\r')
	cnt = 0
	while True:
		movelist = s.FetchMove(3)
		if len(movelist) == 0:
			break
		move = choice(movelist)
		s.BoardState()
		s.MakeMove(move)
		cnt += 1


elaspedtime = time() - starttime

print('Average time of random walk: {:2.2} ms'.format(elaspedtime / N * 1000))