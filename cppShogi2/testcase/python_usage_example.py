from ShogiAgent import Shogi
import numpy as np
s = Shogi()
s.BoardInit()
s.EasyBoardPrint()
ks = np.array(s.BoardKnowledgeExtract())
print(ks.shape)