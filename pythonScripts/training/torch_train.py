import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader

from time import time
import torch.optim as optim
import chess
import numpy as np

EPOCHS = 100
BATCH_SIZE = 2048 

def get_piece_val(piece:str):
   temp = 0
   if piece == "k":
      temp = -20.0
   if piece == "q":
      temp = -9.0
   if piece == "r":
      temp = -5.0
   if piece == "b":
      temp = -3.40
   if piece == "n":
      temp = -3.20
   if piece == "p":
      temp = -1.0
   if piece == "K":
      temp = 20.0
   if piece == "Q":
      temp = 9.0
   if piece == "R":
      temp = 5.0
   if piece == "B":
      temp = 3.4
   if piece == "N":
      temp = 3.2
   if piece == "P":
      temp = 1.0
   return temp

def make(fen):
   board = chess.Board(str(fen))
   this_board = []
   row = []
   index = 0
   for piece in str(board):
      if piece != " " and piece != "\n":
         index += 1
         row.append([get_piece_val(piece)])
   
      if index == 8:
        index = 0
        this_board.append(row)
        row = []
   return np.array([this_board])

class Net(nn.Module):

    def __init__(self):
        super(Net, self).__init__()
        self.fc0 = nn.Linear(64,256)
        self.fc1 = nn.Linear(256, 512)
        self.fc2 = nn.Linear(512, 32)
        self.fc3 = nn.Linear(32, 32)
        self.fc4 = nn.Linear(32, 512)
        for i in range(5, 512-5):
            exec("self.fc{} = nn.Linear(512, 512)".format(i))
        self.fc507 = nn.Linear(512, 1)

    def forward(self, x):
        x = F.relu(self.fc0(x))
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = F.relu(self.fc3(x))
        x = F.relu(self.fc4(x))
        for i in range(5, 512-5):
            exec("x = F.relu(self.fc{}(x))".format(i))
        x = torch.tanh(self.fc507(x))
        return x

class dset(Dataset):
    def __init__(self):
        pretargets = open("targets.dat").readlines()
        predata = open("data.dat").readlines()
        self.x = []
        deletes = []
        for i in predata:
            try:
                self.x.append(make(i))
            except ValueError:
                print(predata.index(i), i)
                deletes.append(i)
        self.y = []
        for i in pretargets:
            if not i in deletes:
                self.y.append(float(i))
    def __getitem__(self, index):
        try:
            return self.x[index], self.y[index]
        except IndexError:
            print(index, len(self.x), len(self.y))
    def __len__(self):
        assert(len(self.x) == len(self.y))
        return len(self.x)

net = Net()
net = net.cuda()
optimizer = optim.Adam(net.parameters(), lr=0.001)
criterion = nn.MSELoss()

dataset = dset()
datasetloader = DataLoader(dataset, BATCH_SIZE, True)
"""pretargets = open("targets.dat").readlines()
predata = open("data.dat").readlines()
data = []
for i in predata:
    data.append(make(i))

targets = []
for i in pretargets:
    targets.append(float(i))"""

#net.load_state_dict(torch.load("model.pt"))

running_loss = 0.0
step = 0

for epoch in range(EPOCHS):
    print("starting epoch {}".format(epoch+1))
    st = time()
    for i, (x, t) in enumerate(datasetloader):
        optimizer.zero_grad()
        #x = torch.from_numpy(d)
        x = x.view(-1, 64)
        x = x.cuda()
        out = net(x.float())
        
        loss = criterion(out, torch.from_numpy(np.array(t)).float().view(-1, 1).cuda())
        loss.backward()
        optimizer.step()
        running_loss += loss.item()
        step += 1
    print("epoch {}: loss {} steps {} time {}".format(epoch+1, running_loss/i, step, time() - st))
    running_loss = 0.0
        

"""for epoch in range(10):
    for d, t in zip(data, np.array(targets)):
        optimizer.zero_grad()
        x = torch.from_numpy(d)
        x = x.view(-1, 64)
        #x = x.cuda()
        out = net(x.float())
        loss = criterion(out, torch.from_numpy(np.array(t)).float().view(-1, 1))
        loss.backward()
        optimizer.step()
        running_loss += loss.item()
        if step % 2000 == 1999:    # print every 2000 mini-batches
            print('[%d, %5d] loss: %.3f' %
                  (epoch + 1, step + 1, running_loss / 2000))
            running_loss = 0.0
        step += 1"""
    
torch.save(net.state_dict(), "model512.pt")
"""print("test:")
while True:
    i = input("% ")
    x = make(i)
    x = torch.from_numpy(x)
    x = x.view(-1, 64)
    out = net(x.float())
    print(out)"""
def search(board, color):
    best_s = -2
    best_m = None
    for i in board.legal_moves:
        board.push(i)
        x = make(board.fen())
        x = torch.from_numpy(x)
        x = x.view(-1, 64)
        x = x.cuda()
        out = net(x.float()) * color
        board.pop()
        if out > best_s:
            best_s = out
            best_m = i
    return best_m
board = chess.Board()
color = 1
moves = ""
i = 0
while not board.is_game_over():
    i += 1
    move = search(board, color)
    moves += str(i) + ". " + str(board.san(move)) + " "
    board.push(move)
    color *= -1
    move = search(board, color)
    moves += str(board.san(move)) + " "
    board.push(move)
    color *= -1
print(moves)