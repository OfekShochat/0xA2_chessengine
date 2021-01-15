import random
import chess
import chess.engine
from time import sleep, time
from numpy import tanh
from io import StringIO
import chess.pgn

def random_fen(nnue=False):
    b = chess.Board()
    for i in range(random.randint(1, random.randint(2,100))):
        if b.is_game_over(): break  
        previous = b.fen()
        b.push(random.choice(list(b.legal_moves)))
    if nnue:
        return b.fen(), previous
    return b
engine = chess.engine.SimpleEngine.popen_uci(r"C:\Users\User\Downloads\stockfish_12_win_x64_bmi2\stockfish_20090216_x64_bmi2.exe")
def ev(n):
    dat = open("data.dat", "a+")
    tar = open("targets.dat", "a+")
    st = time()
    sleep(0.000000001)
    for i in range(n):
        print(i, end="\r")
        r = random_fen()
        e = engine.analyse(r, chess.engine.Limit(depth=3))
        if str(e["score"].white()).__contains__("#"):
            dat.write(r.fen() + "\n")
            if str(e["score"]).__contains__("+"):
                tar.write("1\n")
            elif str(e["score"]).__contains__("-"):
                tar.write("-1\n")
            else:
                print(str(e["score"]))
        else:
            tar.write(str(tanh(int(str(e["score"].white()))/500)) + "\n")
            dat.write(r.fen() + "\n")
        del r
        del e
    del tar
    del dat
        
def ev_from():
    dat = open("data.dat", "a+")
    tar = open("targets.dat", "a+")
    st = time()
    sleep(0.000000001)
    positions = open(r"C:\Users\User\Downloads\CCRL-4040.[1190874].pgn", "r").read().split("\n\n")
    for i in positions[1::2]:
        board = chess.Board()
        game = chess.pgn.read_game(StringIO(i))
        for move in game.mainline_moves():
            board.push(move)
            e = engine.analyse(board, chess.engine.Limit(depth=1))
            if str(e["score"].white()).__contains__("#"):
                dat.write(board.fen() + "\n")
                if str(e["score"]).__contains__("+"):
                    tar.write("1\n")
                elif str(e["score"]).__contains__("-"):
                    tar.write("-1\n")
                else:
                    print(str(e["score"]))
            else:
                tar.write(str(tanh(int(str(e["score"].white()))/500)) + "\n")
                dat.write(board.fen() + "\n")
                del e
        del board
    del tar
    del dat

"""m = False
for i in range(200):
    st = time()
    print("starting next pulse ({})".format(i))
    #ev(10000)
    print("finished pulse {}".format(i))
    if not m:
        print("info eps {}".format(10000/(time() - st)))
        d = input("do you want to proceed?[y/n] ")
        if d == "y":
            continue
        elif d == "more":
            m = True
        else:
            break
    else:
        sleep(2)"""

ev(100_000_000)