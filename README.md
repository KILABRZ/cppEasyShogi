## introduction
- 這個專案實作了「日本將棋」這個棋盤遊戲。
- 以高效率、可延續開發為主要目標。
- 將棋除了一般棋類的基本規則外，還包括「王手無視」、「打步詰」這兩個複雜的反則。
- 這些反則基本上都可以透過再多一次的盤面搜索去監測，但重視高效率的前提下，額外一次的盤面搜索會成為效率上的阻礙。
- 這個專案把這兩個反則中可能遇到的情況全部列舉了一次，獲得了很好的執行時間成績。

## motivation
- 將棋推廣。
- 詰將棋解題器。
- 之後拿來做機器學習的 base work。
- ELD#1

## shogi rule
- 基本規則：[中文](https://zh.wikipedia.org/wiki/%E5%B0%86%E6%A3%8B_(%E6%97%A5%E6%9C%AC)) & [English](https://en.wikipedia.org/wiki/Shogi)
- 和一般棋類不同之處在於「打入」（打回吃掉的棋子）的規則，這使得將棋成為世界主流棋類中，遊戲樹複雜度僅次於圍棋的存在。職業比賽時間也是以天計的。
- 其中實作上最難處理的兩個規則為王手無視和打步詰。王手無視指的是「移動後王會在下一手被吃掉」、打步詰則是「以打入步兵做出詰棋（殺）」。
- 總之，是很不錯的棋類遊戲。

## design (short)
- 把遊戲的主流程分成三步：FetchMove、CheckMove、MakeMove。
- 在 FetchMove 時，如果找不到任何一手可以做，則遊戲結束。
- 在 MakeMove 時，若是做出投降，則遊戲結束（沒有寫就是了）。
- 這個專案 FetchMove 和 CheckMove 是寫在一起的，為了效率的因素。FetchMove 有幾種 request 可以給，表示要求的 Move 需要嚴格符合那些規則。
- FetchMove 會回傳一 vector 的 move 和更新全局的盤面狀況，這個全局的盤面狀況是處理王手無視和打步詰的關鍵。
- 更細部的設計的話，將將棋的大部分要素全部用編碼的方式進行。例如棋種、位置、行動等。基本全部都在 int 範圍內可以表示。也可以說這個專案的目標就是將將棋這個遊戲丟到像是 FSM 的架構下重新解釋。

## performence
- 在 main.cpp 中有一個方法 speedtest(n)，可以透過各種指令去測試這個函數的速度，作為 performence 的指標。
- 其含義為模擬 n 次 random walk，也就是「雙方遵循一般規則隨機下棋直到結束」重複 n 次。
- 在 <code>g++ *.cpp -O3</code> ，i7-8525U 的編譯和執行環境下，獲得了 speedtest(1000) 只需 2565ms 的成績，也就是說平均一盤只需要 2.5ms。
- 相較於 previous work 在 python 上的將棋實作，這裡的效率增加了一萬倍左右，算是十足的進步。甚至 code 還比較短。

## future work
- 徹底除錯。由於王手無視和打步詰的狀況很多，或許存在沒有找到的 bug。
- 實作 FetchMove 的 request 4。
- 把正規的棋譜、棋盤標示方式改一改。做個可以讀棋譜的程序。
- 串 GUI。

## update record
- 2020/2/20
- 更新了 judger.cpp, gameAnalysis.cpp, shogi.cpp, main.cpp
- 新增了一個簡單的評分模型（基於人擬定的決策模式），一個會做 alphaBeta 搜索的 bot，可以達成幾乎必定擊敗 random walk 的成績。
- 寫了一個遺傳算法來看看能不能 train 出好的結果，結果是無法。
