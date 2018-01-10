import numpy as np
import gym

from keras.models import Sequential
from keras.layers import Dense, Activation, Flatten
from keras.optimizers import Adam

from rl.agents.dqn import DQNAgent
from rl.policy import BoltzmannQPolicy, EpsGreedyQPolicy
from rl.memory import SequentialMemory, EpisodeParameterMemory

from gym.envs.registration import registry, register, make, spec

import cartpole

ENV_NAME = 'CartPole-v2'
ENV_NAME2 = 'CartPole-v3'

register(
   	id='CartPole-v2',
   	entry_point='cartpole:MyCartPoleEnv',
)
register(
   	id='CartPole-v3',
   	entry_point='cartpole2:MyCartPoleEnv',
)

env = gym.make(ENV_NAME)
#env2 = gym.make(ENV_NAME2)
np.random.seed(123)
env.seed(123)
nb_actions = env.action_space.n

model = Sequential()
model.add(Flatten(input_shape=(1,) + env.observation_space.shape))
model.add(Dense(16))
model.add(Activation("relu"))
model.add(Dense(16))
model.add(Activation("relu"))
model.add(Dense(nb_actions))
model.add(Activation("linear"))
print(model.summary())

policy = EpsGreedyQPolicy()
memory = SequentialMemory(limit = 100000, window_length = 1)
dqn = DQNAgent(model=model, nb_actions = nb_actions, memory = memory, nb_steps_warmup=1000,
target_model_update=1e-2, policy=policy)
dqn.compile(Adam(lr=1e-4), metrics=['mae'])

dqn.fit(env, nb_steps=200000, visualize=False, verbose=1)