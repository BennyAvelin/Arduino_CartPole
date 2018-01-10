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

logger = logging.getLogger(__name__)

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
        self.force_mag = 2.0
        self.tau = 0.02  # seconds between state updates
        self.wheelradii = 0.2

        # Angle at which to fail the episode
        self.theta_threshold_radians = 12 * 2 * math.pi / 360
        self.x_threshold = 2.4

        # Angle limit set to 2 * theta_threshold_radians so failing observation is still within bounds
        high = np.array([
            self.x_threshold * 2,
            np.finfo(np.float32).max,
            self.theta_threshold_radians * 2,
            np.finfo(np.float32).max])
        high2 = np.array([
            np.finfo(np.float32).max,
            self.gravity*math.cos(2*self.theta_threshold_radians),
            np.finfo(np.float32).max,
            np.finfo(np.float32).max
        ])
        low2 = np.array([
            -np.finfo(np.float32).max,
            -1,
            -np.finfo(np.float32).max,
            -np.finfo(np.float32).max
        ])

        self.action_space = spaces.Discrete(2)
        self.observation_space = spaces.Box(low2, high2)

        self._seed()
        self.viewer = None
        self.states = None
        self.state = None

        self.steps_beyond_done = None

    def _seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def _step(self, action):
        assert self.action_space.contains(action), "%r (%s) invalid"%(action, type(action))
        states = self.states
        x, x_dot, theta, theta_dot = states

        torque = self.force_mag if action==1 else -self.force_mag
        costheta = math.cos(theta)
        sintheta = math.sin(theta)
        thetaacc = (torque + self.masspole*self.gravity*self.polemass_length*sintheta)/(self.masspole*self.polemass_length*self.polemass_length)
        xacc = -torque/(self.wheelradii*(self.total_mass))
        #Observed acceleration
        oacc_x_prev , oacc_y_prev, tmp1, tmp2 = self.state
        oacc_x = torque/self.polemass_length + self.gravity*sintheta
        oacc_y = self.gravity*costheta
        oacc_x_dot = oacc_x_prev - oacc_x
        oacc_y_dot = oacc_y_prev - oacc_y
        
        x  = x + self.tau * x_dot
        x_dot = x_dot + self.tau * xacc
        theta = theta + self.tau * theta_dot
        theta_dot = theta_dot + self.tau * thetaacc
        self.states = (x,x_dot,theta,theta_dot)
        self.state = (oacc_x,oacc_y,oacc_x_dot,oacc_y_dot)
        
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
        self.states = self.np_random.uniform(low=-0.05, high=0.05, size=(4,))
        x, x_dot, theta, theta_dot = self.states
        costheta = math.cos(theta)
        sintheta = math.sin(theta)
        oacc_x = 0
        oacc_y = 0
        oacc_x_dot = 0
        oacc_y_dot = 0
        self.state = (oacc_x,oacc_y,oacc_x_dot,oacc_y_dot)
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

        if self.states is None: return None

        x = self.states
        cartx = x[0]*scale+screen_width/2.0 # MIDDLE OF CART
        self.carttrans.set_translation(cartx, carty)
        self.poletrans.set_rotation(-x[2])

        return self.viewer.render(return_rgb_array = mode=='rgb_array')