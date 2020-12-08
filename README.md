## 0xA2 chess engine ##

0xA2 is a chess engine that uses a0 methods (or lc0) to play chess. but it performs a shallow alpha beta search with a neural network evaluation.
also known by MCαβ (monte carlo alpha beta, see https://www.chessprogramming.org/MC%CE%B1%CE%B2)

# is this like allie chess? #
I dont know. they say its a hybrid search that uses alpha beta search and monte carlo and it uses the lc0 backend. so I make my own backend (for now only with tensorflow with cppflow project but it is slow so maybe Ill use it too); and it doesnt specify if its the mcαβ.

## current workings ##
the problem I have now is to convert my hdh5 keras files to pb tensorflow files. if you know how to do this on tensorflow 2.3 let me know on issues!

### special thanks ###
thc chess library (I use the retro-sargon one https://github.com/billforsternz/retro-sargon) from Bill Forster.              
cppflow for the tensorflow backend (https://github.com/serizba/cppflow) from serizba             
and the discord of engine-programming and leela chess zero
