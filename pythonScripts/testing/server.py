
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

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        data = self.request.recv(1024)
        d = json.loads(data.decode('utf-8'))
    
        if d["type"] == "register":
            key = hashlib.md5(bytes(str(time()), "utf-8")).hexdigest()
            self.request.sendall(b"""{"type":"registerResponse", "key":%s}""" % bytes(key, "utf-8"))

        if d["type"] == "jobGet":
            self.request.sendall(b"""{"type:"GetResponse", "job":%s}""" % jobs[0])
            inprogress.append({"job":job[0], "time":time()})
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
        if i.lower() == "addjob":
            jobs.append(json.loads(input("addJob: ")))
        if i.lower() == "peek":
            t = [["job", "status"]]
            print(t)
            table = AsciiTable(t)
            print(table.table)
    server.shutdown()
    server.server_close()