
"""
0xA2_chess-engine
Copyright (C) 2020-2021  Ofek Shochat

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import socket
import threading
import requests
import hashlib 

key = input("api_key: ")
result = hashlib.sha256(key.encode()) 
print(result.hexdigest())
apikeys = requests.get("https://raw.githubusercontent.com/OfekShochat/0xA2_chessengine/master/testing/passwords.dat", allow_redirects=True).content.decode("utf-8").split("\n")
if not result.hexdigest() in apikeys:
    exit(0)

r = requests.get("https://raw.githubusercontent.com/OfekShochat/0xA2_chessengine/master/testing/book.txt", allow_redirects=True)
open('book.txt', 'wb').write(r.content)

"""
def client(ip, port, message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ip, port))
    try:
        sock.sendall(message)
        response = sock.recv(1024)
    finally:
        sock.close()
"""
import chess
import chess.engine as ce
import random
from time import time
import chess.pgn
from discord_webhook import DiscordWebhook
from glob import glob
import sys
import os 

class engine:
    def __init__(self, name, path):
        self.name = name
        self.path = path
        self.rating = 0 
        self.games = 0
        self.en = ce.SimpleEngine.popen_uci(path)

engines = []
for i in sys.argv[1:]:
    engines.append(engine(i.split("\\")[-1], i))

book = open("book.txt", "r").readlines()
st = time()
for i in range(10000):
    moves = []
    engines_sort = sorted(engines, key=lambda x: x.games + random.random()*100)
    engine1 = engines_sort[0]
    engine2 = engines_sort[1]
    wc = 3 * 60
    bc = 3 * 60
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
        if m.draw_offered:
            if d:
                dd = True
                break
            d = True
        else:
            d = False
            
        wc -= (time() - e1s)
        #wc += 1 
        if board.is_game_over(): break
        e2s = time()
        m = engine1.en.play(board, ce.Limit(white_clock=wc, black_clock=bc))
        if m.draw_offered:
            if d:
                dd = True
                break
            d = True
        else:
            d = False
        board.push(m.move)
        moves.append(str(m.move))
        bc -= (time() - e2s)
        #bc += 1
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
    #client("127.0.0.1", 41378, open("game.pgn", "rb").read())
    webhook = DiscordWebhook(url='https://discord.com/api/webhooks/794818234238763040/ZrlRJx-tEC3hF8Z4pLURa_xqOoyqSR51DNjTvWVdGjtKKDGLVdUl74S83IyE8QJB9vYh', content=open("game.pgn").read())
    response = webhook.execute()
