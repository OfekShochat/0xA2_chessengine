import chess
import tensorflow as tf
import numpy as np
import ast
import os
#import numba as nb
from time import sleep
import json
import argparse

d = open("data.dat")
t = open("targets.dat")

def shuffle(x, y):
   for _ in range(len(x)):
      rand = np.random.randint(0, len(x))
      rand2 = np.random.randint(0, len(x))
      x[rand], x[rand2] = x[rand2], x[rand]
      y[rand], y[rand2] = y[rand2], y[rand]
   return x, y

data, targets = shuffle(d, t)

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
print("making data...")

boards_data = []
i = 0
for i in data:
   board = chess.Board(str(i))
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
   boards_data.append(np.array(this_board))

new_targets = []
for i in t:
   #print(bytes(i, "utf-8"))
   new_targets.append(float(i))

from keras.engine.topology import Input
from keras.engine.training import Model
from keras.layers.convolutional import Conv2D
from keras.layers.core import Activation, Dense, Flatten
from keras.layers.merge import Add
from keras.layers.normalization import BatchNormalization
from keras.optimizers import SGD
from keras.regularizers import l2
from keras.layers import Concatenate

def nn():
    in_x = x = Input((8,8,1))
    x = Flatten()(x)
    x = Dense(256, "relu")(x)
    x = Dense(512, "relu")(x)
    x = Dense(32)(x)
    x = Dense(1, "tanh")(x)
    model = Model(in_x, x, name="chess_model")
    model.compile(loss="mse", optimizer="adam", metrics=["accuracy"])
    return model

model = nn()
model.fit(np.array(boards_data), np.array(new_targets), 10, 2048)