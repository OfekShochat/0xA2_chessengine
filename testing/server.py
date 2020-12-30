import socket
import threading
import socketserver
from time import sleep

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        data = self.request.recv(1024)
        for line in data.decode('utf-8').split('\n'):
            open("games5.pgn", "a+").write(line)
        open("games5.pgn", "a+").write("\n\n")

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

if __name__ == "__main__":
    # Port 0 means to select an arbitrary unused port
    HOST, PORT = "127.0.0.1", 41378

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address

    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever)
    # Exit the server thread when the main thread terminates
    server_thread.daemon = True
    server_thread.start()
    while True:
        sleep(3)
    server.shutdown()
    server.server_close()