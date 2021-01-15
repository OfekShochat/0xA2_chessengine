
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
from time import sleep
from time import time
import json 
import hashlib
from terminaltables import AsciiTable
import at_exit

def atexit():
    print("poop")
    server.shutdown()
    server.server_close()
at_exit.register(atexit)

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        data = self.request.recv(1024)
        d = json.loads(data.decode('utf-8'))
    
        if d["type"] == "register":
            key = hashlib.md5(bytes(str(time()), "utf-8")).hexdigest()
            self.request.sendall(b"""{"type":"registerResponse", "key":"%s"}""" % bytes(key, "utf-8"))
            open("keys.txt", "a+").write(key + "\n")

        if d["type"] == "jobGet":
            if len(jobs) < 1:
                self.request.sendall(b"""{"type":"GetResponse", "code":"3"}""")
            else:
                self.request.sendall(b"""{"type":"GetResponse", "job":%s, "code":"0"}""" % bytes(json.dumps(jobs[0]), "utf-8"))
                inprogress.append({"job":jobs[0], "time":time()})
                jobs.pop(0)
            
        if d["type"] == "finishJob":
            self.request.sendall(b"""{"type":"finishResponse"}""")
            for line in d["game"].split('\n'):
                open("games5.pgn", "a+").write(line)
            open("games5.pgn", "a+").write("\n\n")

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

def checker():
    while True:
        for i in inprogress:
            if time() - i["time"] > 180*2+20:
                jobs.append(i["job"])
                inprogress.pop(inprogress.index(i))

if __name__ == "__main__":
    # Port 0 means to select an arbitrary unused port
    HOST, PORT = "127.0.0.1", 41378

    jobs = []
    inprogress = []
    r = open("jobs.txt", "r").readlines()
    for i in r:
        jobs.append(json.loads(i))

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address

    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever)
    # Exit the server thread when the main thread terminates
    server_thread.daemon = True
    server_thread.start()

    checker_Thread = threading.Thread(target=checker)
    checker_Thread.deamon = True
    checker_Thread.start()
    
    while True:
        i = input("% ")
        

        if i.lower() == "templates":
            print("poop")
            print("1. games template")
            choice = input(" - ")
            if choice == "1":
                n = int(input("number of games: "))
                tc = input("tc: ")
                engines = input("engines: ").split("-")
            es = []
            for engine in engines:
                e = {}
                this = json.loads(open(engine).read())
                es.append(this)
                
            for _ in range(n):
                
                jobs.append({"tc":tc, "engines":es}) 
                # here we need to loop on the engines and with priority (or without it) assign games that we want to, with priority, 
                # make the priority engines play all low priority engines. without priority, try to make it that every engine plays the same amount of games. 
                # a way to do that, can be like we did in the games.py, so adding game to an engine that is the least tested. but that would be highly inefficient.

        if i.lower() == "addjob":
            jobs.append(json.loads(input("addJob: ")))
        if i.lower() == "peek":
            t = [["job", "status"]]
            for i in jobs:
                t.append([i, "TOGET"])
            for i in inprogress:
                t.append([i, "INPROGRESS"])
            table = AsciiTable(t)
            print(table.table)
    server.shutdown()
    server.server_close()