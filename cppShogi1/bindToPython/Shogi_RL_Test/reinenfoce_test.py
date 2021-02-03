import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.distributions import Categorical

import numpy as np

class GradientNetwork(nn.Module):
	def __init__(self):
		super().__init__()
		self.board_catch = nn.Linear(810, 243)
		self.board_mixed = nn.Linear(257, 257)
		self.combine = nn.Linear(353, 100)
		self.mixed1 = nn.Linear(100, 100)
		self.mixed2 = nn.Linear(100, 20)
		self.digest = nn.Linear(20, 1)

	def forward(self, board, hold, moves):
		board = self.board_catch(board)
		fullboard = torch.cat((board, hold), 2)
		fullboard = self.board_mixed(fullboard)
		fullstate = torch.cat((fullboard.repeat(1, moves.shape[1], 1), moves), 2)
		fullstate = self.combine(fullstate)
		fullstate = self.mixed1(fullstate)
		fullstate = self.mixed2(fullstate)
		scores = self.digest(fullstate)
		return scores

class Shogi_Test_v1():
	def __init__(self, network):
		self.network = network.cuda()
		self.optimizer = optim.Adam(self.network.parameters(), lr = 0.01)

	def learn(self, boardlist, holdlist, movelist, rewardlist):
		self.network.train()
		boards = torch.FloatTensor(boardlist).permute(0, 2, 1).cuda()
		holds = torch.FloatTensor(holdlist).reshape(-1, 1, 14).cuda()
		moves = torch.FloatTensor(movelist).reshape(-1, 1, 96).cuda()
		rewards = torch.FloatTensor(rewardlist).cuda()
		pred_rewards = self.network(boards, holds, moves)
		pred_rewards = pred_rewards.reshape(-1)
		loss = ((rewards - pred_rewards) ** 2).mean() ** 0.5
		self.optimizer.zero_grad()
		loss.backward()
		self.optimizer.step()
		return loss.item()

	def sample(self, board, hold, moves):
		self.network.eval()
		board = torch.FloatTensor(board).reshape(1, 1, 810).cuda()
		hold = torch.FloatTensor(hold).reshape(1, 1, 14).cuda()
		moves = torch.FloatTensor(moves).reshape(1, -1, 96).cuda()
		scores = self.network(board, hold, moves)
		scores = (scores - scores.mean())
		action_prob = F.softmax(scores, dim=1)
		if True in torch.isnan(action_prob):
			print(scores[0].T)
			print(action_prob[0].T)

		action_dist = Categorical(action_prob[0].T)
		action = action_dist.sample()
		return action.item(), moves.detach().cpu().numpy()[0][action.item()]



from ShogiAgent import *
import os

agent_network = GradientNetwork()

if os.path.exists('./Model/Shogi_Test_v1.bin'):
	print('Load from record')
	agent_network.load_state_dict(torch.load('./Model/Shogi_Test_v1.bin'))
agent = Shogi_Test_v1(agent_network)

agent_network.train()

TOTALRUN = 1
SAVE_EVERY_RUM = 100

shogi = Shogi()


nowloss = 999999
for batch in range(TOTALRUN):

	print('[{}/{}] Loss = {:2.4f}'.format(batch+1, TOTALRUN, nowloss), end='\r')
	shogi.InitialBoard()

	boardlist = list()
	holdlist = list()
	movelist = list()
	log_probs = list()

	LIMIT_ROUND = 1000
	TOTAL_ROUND = 0

	WINFLAG = -1

	for i in range(LIMIT_ROUND):
		shogi.EasyBoardPrint()
		state = np.array(shogi.BoardState()).astype(np.int8)
		origin_moves = shogi.FetchMove(3)
		moves = np.array([DecodeMove(move) for move in origin_moves]).astype(np.int8)
		if len(moves) == 0:
			WINFLAG = i % 2
			break

		board, hold = state[:81], state[81:]
		board = np.concatenate((np.eye(8)[board % 8], (board // 16).reshape(81, 1), ((board % 16) // 8).reshape(81, 1)), axis=1)
		board = board.reshape(810, -1)
		
		boardmove = moves[moves[:,3] == 0]
		holdmove = moves[moves[:,3] == 1]
		boardmove = np.concatenate((
				np.eye(95)[boardmove[:,0]]*(-1) + np.eye(95)[boardmove[:,1]],
				boardmove[:,2].reshape(-1, 1)
			), axis=1)

		rearrange = np.array([0, 3, 2, 1, 6, 5, -1, 4])
		holdmove = np.concatenate((
				np.eye(95)[rearrange[holdmove[:,0]]+81+(i%2)*7]*(-1) + np.eye(95)[holdmove[:,1]],
				holdmove[:,2].reshape(-1, 1)
			), axis=1)
		
		moves = np.concatenate((boardmove, holdmove))

		idx, move = agent.sample(board, hold, moves)

		boardlist.append(board)
		holdlist.append(hold)
		movelist.append(move)
		shogi.MakeMove(origin_moves[idx])

	total_round = len(movelist)
	reward_list = np.zeros((total_round))
	if total_round != LIMIT_ROUND:
		WIN_BASE = 1000
		WIN_MAX = 10000
		LOSE_BASE = -1000
		LOSE_MAX = -10000
	else:
		WIN_BASE = 0
		WIN_MAX = -100000
		LOSE_BASE = 0
		LOSE_MAX = -100000

	if WINFLAG: # Sente win
		sente_reward = np.linspace(WIN_BASE, WIN_MAX, num=(total_round-1)//2+1)
		gote_reward = np.linspace(LOSE_BASE, LOSE_MAX, num=(total_round)//2)
	else:
		sente_reward = np.linspace(LOSE_BASE, LOSE_MAX, num=(total_round-1)//2+1)
		gote_reward = np.linspace(WIN_BASE, WIN_MAX, num=(total_round)//2)
	
	lossa = agent.learn(boardlist[0::2], holdlist[0::2], movelist[0::2], sente_reward)
	lossb = agent.learn(boardlist[1::2], holdlist[1::2], movelist[1::2], gote_reward)
	nowloss = (lossa + lossb) / total_round

	if batch % SAVE_EVERY_RUM == SAVE_EVERY_RUM - 1:
		print('Save Model ...                        ')
		torch.save(agent.network.state_dict(), './Model/Shogi_Test_v1.bin')


