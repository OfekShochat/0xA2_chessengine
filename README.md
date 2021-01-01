## 0xA2 chess engine ##

0xA2 is a chess engine that uses a0 methods (or lc0) to play chess. but it performs a shallow alpha beta search with a neural network evaluation.             
also known by MCαβ (monte carlo alpha beta, see https://www.chessprogramming.org/MC%CE%B1%CE%B2)

if you want to talk with me, you can join here! https://discord.gg/cTeagCWtMA

## current workings ##
1. making the neural network lib for the engine. I know something with the neuron code is wrong, do you know? something with the weights

if you know how to do something from the above let me know on issues with the "solution" label!  

the plan is to go by this:
![alt text](https://github.com/OfekShochat/0xA2_chessengine/blob/master/threaded_chessengine.png)

### is this like allie chess? ###
I dont know. they say its a hybrid search that uses alpha beta search and monte carlo and it uses the lc0 backend. so I make my own backend (for now only with tensorflow and the  cppflow project but it is slow so maybe Ill use it too); and it doesnt specify if its the mcαβ.

### help (training wise) ###
you can train tensorflow models (value networks only for now)(you can make keras ones but I dont know how to convert them to pb file and use it) and then use it with this engine!

### special thanks ###
thc chess library (I use the retro-sargon one https://github.com/billforsternz/retro-sargon) from Bill Forster.              
cppflow for the tensorflow backend (https://github.com/serizba/cppflow) from serizba  (unsused but oh well)           
and the discord of engine-programming and leela chess zero

### why the code is messy? ###
I learn c++ on the go with this project. its the first time I program in c++ and I started this project a week ago. 
