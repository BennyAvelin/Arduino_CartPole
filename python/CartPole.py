import numpy as np
import gym

from keras.models import Sequential
from keras.layers import Dense, Activation, Flatten
from keras.optimizers import Adam

from rl.agents.dqn import DQNAgent
from rl.policy import EpsGreedyQPolicy
from rl.memory import SequentialMemory

ENV_NAME = 'CartPole-v0'

env = gym.make(ENV_NAME)
np.random.seed(123)
env.seed(123)
nb_actions = env.action_space.n

model = Sequential()
model.add(Flatten(input_shape=(1,) + env.observation_space.shape))
model.add(Dense(16))
model.add(Activation("relu"))
model.add(Dense(nb_actions))
model.add(Activation("linear"))
print(model.summary())

policy = EpsGreedyQPolicy()
memory = SequentialMemory(limit = 50000, window_length = 1)
dqn = DQNAgent(model=model, nb_actions = nb_actions, memory = memory, nb_steps_warmup=100,
target_model_update=1e-2, policy=policy)
dqn.compile(Adam(lr=1e-3), metrics=['mae'])

dqn.fit(env, nb_steps=1000, visualize=True, verbose=2)

import serial
import struct

def setupSerial(baudrate, timeout):
    ser = serial.Serial('/dev/cu.usbmodem1421',baudrate,timeout=timeout)
    return ser

def npArrayToByteArray(array):
    bytearr = []
    for x in array:
        bytearr += struct.pack('f',x)
    return bytearr
    
def sendArrayToSerial(ser,array):
    bytearr = npArrayToByteArray(array.astype(float).flatten())
    rows = array.shape[0]
    columns = 1
    if (len(array.shape) > 1):
        columns = array.shape[1]
    ser.write(struct.pack('h',rows)+struct.pack('h',columns))
    ser.write(bytes(bytearr))

def recieveArrayFromSerial(ser):
    sizeOfInt = 2
    sizeOfFloat = 4
    rows = struct.unpack('h',ser.read(sizeOfInt))[0]
    cols = struct.unpack('h',ser.read(sizeOfInt))[0]
    buffer = ser.read(sizeOfFloat*rows*cols)
    array = []
    for index in range(0,len(buffer),4):
        array += [struct.unpack_from('f',bytes(buffer),index)[0]]
    return np.reshape(np.array(array),(rows,cols))

weights = dqn.layers[1].get_weights()[0]
bias = dqn.layers[1].get_weights()[1]
weights1 = dqn.layers[3].get_weights()[0]
bias1 = dqn.layers[3].get_weights()[1]
inputs = np.reshape(np.array([1.0,1.0,1.0,1.0]),(1,4))

ser = setupSerial(9600,10)
sendArrayToSerial(ser,weights)
ser.readline()
sendArrayToSerial(ser,bias)
ser.readline()
sendArrayToSerial(ser,weights1)
ser.readline()
sendArrayToSerial(ser,bias1)
ser.readline()
sendArrayToSerial(ser,inputs)
ser.readline()
ser.readline()
ser.readline()

recieveArrayFromSerial(ser)
ser.read_all()