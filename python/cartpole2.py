"""
Based on the cart-pole from OpenAI gym. This version simulates a segway.
Author:Benny Avelin
"""

import logging
import math
import gym
from gym import spaces
from gym.utils import seeding
import numpy as np
import serial
import numpy as np
import struct
import time

logger = logging.getLogger(__name__)

def setupSerial(baudrate, timeout):
    #ser = serial.Serial('/dev/cu.HC-06-DevB',baudrate,timeout=timeout)
    ser = serial.Serial('/dev/cu.usbmodem1421',baudrate,timeout=timeout)
    return ser
    
def recieveStateFromSerial(ser):
    sizeOfFloat = 4
    buffer = ser.read(sizeOfFloat*4)
    array = []
    for index in range(0,len(buffer),sizeOfFloat):
        array += [struct.unpack_from('f',bytes(buffer),index)[0]]
    return array


class MyCartPoleEnv(gym.Env):
    metadata = {
        'render.modes': ['human', 'rgb_array'],
        'video.frames_per_second' : 50
    }

    def __init__(self):
        self.gravity = 9.8
        self.masscart = 1.0
        self.masspole = 1.0
        self.total_mass = (self.masspole + self.masscart)
        self.length = 0.5 # actually half the pole's length
        self.polemass_length = (self.masspole * self.length)
        self.force_mag = 5.0
        self.tau = 0.02  # seconds between state updates
        self.wheelradii = 0.2

        # Angle at which to fail the episode
        self.ampfac = 50
        
        #self.theta_threshold_radians = 12 * 2 * math.pi / 360
        self.theta_threshold_radians = 0.3
        self.topValue = 378.0
        self.deg90 = 304.0
        #self.minTheta = 355.0 / self.ampfac
        #self.maxTheta = 400.0 / self.ampfac
        self.x_threshold = 2.4

        # Angle limit set to 2 * theta_threshold_radians so failing observation is still within bounds
        high = np.array([ 
            self.x_threshold * 2,
            np.finfo(np.float32).max,
            self.theta_threshold_radians,
            np.finfo(np.float32).max])

        self.action_space = spaces.Discrete(2)
        self.observation_space = spaces.Box(-high, high)

        self._seed()
        self.viewer = None
        self.state = None

        self.steps_beyond_done = None
        self.ser = setupSerial(115200,10)

    def _toAngle(self,z_acc):
        return (z_acc-self.topValue)/(self.topValue-self.deg90)
    
    def _seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]
        
    def _getstate(self,x,theta):
        newx, n_x_acc, n_y_acc, n_z_acc = recieveStateFromSerial(self.ser)
        n_z_acc = self._toAngle(n_z_acc)
        x_dot = newx-x
        theta_dot = n_z_acc-theta
        theta = n_z_acc
        x = newx
        return [x,x_dot,theta,theta_dot]

    def _step(self, action):
        assert self.action_space.contains(action), "%r (%s) invalid"%(action, type(action))
        #prev_state = self.state
        x, x_dot, theta, theta_dot = self.state
        self.ser.write(bytes(str(action)+"\n",'utf-8'))
        x, x_dot, theta, theta_dot = self._getstate(x,theta)
        self.state = [x,x_dot,theta,theta_dot]
        
        done =  x < -self.x_threshold \
                or x > self.x_threshold \
                or theta < -self.theta_threshold_radians \
                or theta > self.theta_threshold_radians
        done = bool(done)

        if not done:
            reward = 1.0
        elif self.steps_beyond_done is None:
            # Pole just fell!
            self.steps_beyond_done = 0
            reward=1.0
        else:
            if self.steps_beyond_done == 0:
                logger.warning("You are calling 'step()' even though this environment has already returned done = True. You should always call 'reset()' once you receive 'done = True' -- any further steps are undefined behavior.")
            self.steps_beyond_done += 1
            reward = 0.0

        return np.array(self.state), reward, done, {}

    def _reset(self):
        self.ser.write(bytes(str(-1)+"\n",'utf-8'))
        input("Reset the bot to standing and press enter when done...")
        self.ser.read_all()
        self.ser.write(bytes(str(0)+"\n",'utf-8'))
        self.state = self._getstate(0,0)
        self.steps_beyond_done = None
        return np.array(self.state)

    def _render(self, mode='human', close=False):
        if close:
            if self.viewer is not None:
                self.viewer.close()
                self.viewer = None
            return

        screen_width = 600
        screen_height = 400

        world_width = self.x_threshold*2
        scale = screen_width/world_width
        carty = 100 # TOP OF CART
        polewidth = 10.0
        polelen = scale * 1.0
        cartwidth = 50.0
        cartheight = 30.0

        if self.viewer is None:
            from gym.envs.classic_control import rendering
            self.viewer = rendering.Viewer(screen_width, screen_height)
            l,r,t,b = -cartwidth/2, cartwidth/2, cartheight/2, -cartheight/2
            axleoffset =cartheight/4.0
            cart = rendering.FilledPolygon([(l,b), (l,t), (r,t), (r,b)])
            self.carttrans = rendering.Transform()
            cart.add_attr(self.carttrans)
            self.viewer.add_geom(cart)
            l,r,t,b = -polewidth/2,polewidth/2,polelen-polewidth/2,-polewidth/2
            pole = rendering.FilledPolygon([(l,b), (l,t), (r,t), (r,b)])
            pole.set_color(.8,.6,.4)
            self.poletrans = rendering.Transform(translation=(0, axleoffset))
            pole.add_attr(self.poletrans)
            pole.add_attr(self.carttrans)
            self.viewer.add_geom(pole)
            self.axle = rendering.make_circle(polewidth/2)
            self.axle.add_attr(self.poletrans)
            self.axle.add_attr(self.carttrans)
            self.axle.set_color(.5,.5,.8)
            self.viewer.add_geom(self.axle)
            self.track = rendering.Line((0,carty), (screen_width,carty))
            self.track.set_color(0,0,0)
            self.viewer.add_geom(self.track)

        if self.state is None: return None

        x = self.state
        cartx = x[0]*scale+screen_width/2.0 # MIDDLE OF CART
        self.carttrans.set_translation(cartx, carty)
        self.poletrans.set_rotation(-x[2])

        return self.viewer.render(return_rgb_array = mode=='rgb_array')