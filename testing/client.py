import socket
import threading
import socketserver

def client(ip, port, message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ip, port))
    try:
        sock.sendall(message)
        response = sock.recv(1024)
    finally:
        sock.close()

import chess
import chess.engine as ce
import random
from time import time
import chess.pgn

class engine:
    def __init__(self, name, path):
        self.name = name
        self.path = path
        self.rating = 0 
        self.games = 0
        self.en = ce.SimpleEngine.popen_uci(path)

reference_stockfish = engine("stockfish", "D:\\stockfish_12_win_x64_bmi2\\stockfish_20090216_x64_bmi2.exe")
lc0_new = engine("lc0_new", "lc0_new.bat")
lc0_run2_730357 = engine("lc0_run2_730357", "lc0_run2_730357.bat")
lc0_703350 = engine("lc0_703350", "lc0_703350.bat")
lc0_730688 = engine("weights_run2_730688", "weights_run2_730688.bat")

engines = [reference_stockfish, lc0_new, lc0_run2_730357, lc0_703350, lc0_730688]

book = open("book.txt", "r").readlines()
st = time()
for i in range(10000):
    moves = []
    engines_sort = sorted(engines, key=lambda x: x.games + random.random()*i/10)
    engine1 = engines_sort[0]
    engine2 = engines_sort[1]
    wc = 1.5 * 60
    bc = 1.5 * 60
    dd = False
    d = False
    print("\n\nstarting a match (gameno {}): {} {} vs. {} {}".format(i, str(engine1.name), engine1.rating, str(engine2.name), engine2.rating))
    book_choice = random.choice(book)
    board = chess.Board(book_choice)

    while not board.is_game_over():
        e1s = time()
        m = engine1.en.play(board, ce.Limit(white_clock=wc, black_clock=bc))
        board.push(m.move)
        moves.append(str(m.move))
        """if m.draw_offered:
            if d:
                dd = True
                break
            d = True"""
            
        wc -= (time() - e1s)
        wc += 1 
        if board.is_game_over(): break
        e2s = time()
        m = engine1.en.play(board, ce.Limit(white_clock=wc, black_clock=bc))
        """if m.draw_offered:
            if d:
                dd = True
                break
            d = True"""
        board.push(m.move)
        moves.append(str(m.move))
        bc -= (time() - e2s)
        bc += 1
        ##print(board)
        print("{} {} | {} {}                              ".format(engine1.name, wc, engine2.name, bc), end="\r")
        if wc < 0 or bc < 0:
            break
    r = board.result()
    
    if wc < 0:
        open("game.pgn", "w+").write("[Result \"0-1\"]\n")
    elif bc < 0:
        open("game.pgn", "w+").write("[Result \"1-0\"]\n")
    else:
        open("game.pgn", "w+").write("[Result \"{}\"]\n".format(r))   
    open("game.pgn", "a+").write("[FEN \"{}\"]\n".format(book_choice[:book_choice.find("\n")]))
    open("game.pgn", "a+").write("[White \"{}\"]\n".format(engine1.name))
    open("game.pgn", "a+").write("[Black \"{}\"]\n".format(engine2.name))
    b = chess.Board(book_choice)
    bb = b.variation_san([chess.Move.from_uci(m) for m in moves]) 
    open("game.pgn", "a+").write(str(bb) + "\n\n")
    client("127.0.0.1", 41378, open("game.pgn", "rb").read())
