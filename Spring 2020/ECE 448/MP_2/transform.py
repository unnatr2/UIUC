
# transform.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
# 
# Created by Jongdeog Lee (jlee700@illinois.edu) on 09/12/2018

"""
This file contains the transform function that converts the robot arm map
to the maze.
"""
import copy
from arm import Arm
from maze import Maze
from search import *
from geometry import *
from const import *
from util import *

def transformToMaze(arm, goals, obstacles, window, granularity):
    """This function transforms the given 2D map to the maze in MP1.
    
        Args:
            arm (Arm): arm instance
            goals (list): [(x, y, r)] of goals
            obstacles (list): [(x, y, r)] of obstacles
            window (tuple): (width, height) of the window
            granularity (int): unit of increasing/decreasing degree for angles

        Return:
            Maze: the maze instance generated based on input arguments.

    """
    numLinks = arm.getNumArmLinks()
    arm_limits = arm.getArmLimit()
    arm_start_angles = arm.getArmAngle()
    num_x = 1
    num_y = 1
    num_z = 1
    if numLinks == 1:
        alpha_limits = arm_limits[0]
        offsets = [alpha_limits[0], 0, 0]
        num_x = int((alpha_limits[1] - alpha_limits[0]) / granularity) + 1
        initial_map = np.full((num_x, 1, 1), SPACE_CHAR)
        arm_start_angles.append(0)
        arm_start_angles.append(0)
    elif numLinks == 2:
        alpha_limits = arm_limits[0]
        beta_limits = arm_limits[1]
        offsets = [alpha_limits[0], beta_limits[0], 0]
        num_x = int((alpha_limits[1] - alpha_limits[0]) / granularity) + 1
        num_y = int((beta_limits[1] - beta_limits[0]) / granularity) + 1
        initial_map = np.full((num_x, num_y, 1), SPACE_CHAR)
        arm_start_angles.append(0)
    else:
        alpha_limits = arm_limits[0]
        beta_limits = arm_limits[1]
        gamma_limits = arm_limits[2]
        offsets = [alpha_limits[0], beta_limits[0], gamma_limits[0]]
        num_x = int((alpha_limits[1] - alpha_limits[0]) / granularity) + 1
        num_y = int((beta_limits[1] - beta_limits[0]) / granularity) + 1
        num_z = int((gamma_limits[1] - gamma_limits[0]) / granularity) + 1
        initial_map = np.full((num_x, num_y, num_z), SPACE_CHAR)
    
    arm_start_pos = angleToIdx(arm_start_angles, offsets, granularity)

    for x in range (0, num_x):
        for y in range (0, num_y):
            for z in range (0, num_z):
        
                angles = idxToAngle((x, y, z), offsets, granularity)

                arm.setArmAngle(angles)

                armPosDist = arm.getArmPosDist()
                armEnd = arm.getEnd()
                armPos = arm.getArmPos()

                if (x, y, z) == arm_start_pos:
                    initial_map[x][y][z] = START_CHAR
                
                if doesArmTouchObjects(armPosDist, obstacles, isGoal=False):
                    initial_map[x][y][z] = WALL_CHAR
                
                elif not isArmWithinWindow(armPos, window):
                    initial_map[x][y][z] = WALL_CHAR

                elif doesArmTipTouchGoals(armEnd, goals) and initial_map[x][y][z] != WALL_CHAR:
                    initial_map[x][y][z] = OBJECTIVE_CHAR
                
                elif doesArmTouchObjects(armPosDist, goals, isGoal=True):
                    initial_map[x][y][z] = WALL_CHAR


    return Maze(initial_map, offsets, granularity)