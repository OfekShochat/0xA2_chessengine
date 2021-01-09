
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
import socketserver
from glob import glob
import os
import json
import requests

VERSION = 0.1

def client(ip, port, mtype):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ip, port))
    try:
        if mtype == "register":
            message = b"""{"type":"register"}"""

        sock.sendall(message)
        response = sock.recv(4096)
    finally:
        sock.close()
    return response

if __name__=="__main__":
    try:
        if not open("login.txt").read() == "":
            apikey = json.loads(open("login.txt"))["key"]
        raise FileNotFoundError
    except FileNotFoundError:
        print("no login information. getting api key")
        open("login.txt", "w+").write(client("127.0.0.1", 41378, "register").decode("utf-8"))

    if not ".\\cutechess" in glob(".\\**"):
        import zipfile
        with zipfile.ZipFile(".\\cutechess.zip", 'r') as zip_ref:
            os.mkdir(".\\cutechess")
            zip_ref.extractall(".\\cutechess")
    
    r = requests.get("https://raw.githubusercontent.com/OfekShochat/0xA2_chessengine/master/testing/book.txt", allow_redirects=True)
    open('book.txt', 'wb').write(r.content)

    os.system(r""".\cutechess\cutechess-cli.exe -engine name=lc0 cmd=D:\lc0-v0.26.1-windows-gpu-nvidia-cuda\lc0.exe"""
    + r""" -engine name=stockfish cmd=D:\stockfish_12_win_x64_bmi2\stockfish_20090216_x64_bmi2.exe -each proto=uci tc=100/90+1 """
    + r"""-rounds 1 -pgnout file.pgn min fi -bookmode disk -openings file=book_3moves_cp25-49_13580pos.pgn format=pgn""")