## 0xA2 chess engine ##

0xA2 is a chess engine that uses a0 methods (or lc0) to play chess. but it performs a shallow alpha beta search with a neural network evaluation.             
also known by MCαβ (monte carlo alpha beta, see https://www.chessprogramming.org/MC%CE%B1%CE%B2)

## current workings ##
1. convert my hdh5 keras files to pb tensorflow files with tensorflow 2.x. (tried https://medium.com/@sebastingarcaacosta/how-to-export-a-tensorflow-2-x-keras-model-to-a-frozen-and-optimized-graph-39740846d9eb but then: "Exception thrown at 0x00007FFAF05B1F70 (tensorflow.dll) in a0xa.exe: 0xC0000005: Access violation reading location 0x0000000000000090.")           
2. when writing "position " and then "fen" or something else it finds the first, "position" and then in the next iteration it makes cmd to "fen" for some reason.

if you know how to do something from the above let me know on issues with the "solution" label!  

### is this like allie chess? ###
I dont know. they say its a hybrid search that uses alpha beta search and monte carlo and it uses the lc0 backend. so I make my own backend (for now only with tensorflow and the  cppflow project but it is slow so maybe Ill use it too); and it doesnt specify if its the mcαβ.

### help (training wise) ###
you can train tensorflow models (value networks only for now)(you can make keras ones but I dont know how to convert them to pb file and use it) and then use it with this engine!

### special thanks ###
thc chess library (I use the retro-sargon one https://github.com/billforsternz/retro-sargon) from Bill Forster.              
cppflow for the tensorflow backend (https://github.com/serizba/cppflow) from serizba             
and the discord of engine-programming and leela chess zero

### why the code is messy? ###
I learn c++ on the go with this project. its the first time I program in c++ and I started this project a week ago. 
