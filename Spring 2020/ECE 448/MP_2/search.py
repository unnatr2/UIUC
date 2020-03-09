# search.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Jongdeog Lee (jlee700@illinois.edu) on 09/12/2018

"""
This file contains search functions.
"""
# Search should return the path and the number of states explored.
# The path should be a list of tuples in the form (alpha, beta, gamma) that correspond
# to the positions of the path taken by your search algorithm.
# Number of states explored should be a number.
# maze is a Maze object based on the maze from the file specified by input filename
# searchMethod is the search method specified by --method flag (bfs,astar)
# You may need to slight change your previous search functions in MP1 since this is 3-d maze

from collections import deque
from heapq import heappop, heappush

def search(maze, searchMethod):
    return {
        "bfs": bfs,
    }.get(searchMethod, [])(maze)

def bfs(maze):
    # Write your code here
    """
    This function returns optimal path in a list, which contains start and objective.
    If no path found, return None. 
    """
    start_pos = maze.getStart()
    objectives = maze.getObjectives()

    frontier = [start_pos]
    explored = []
    parent_set = {}

    end_pos = (-1, -1, -1)

    while frontier:

        curr_pos = frontier.pop(0)

        if curr_pos not in explored:
            explored.append(curr_pos)
        else:
            continue

        if curr_pos in objectives:
            end_pos = curr_pos
            break
    
        valid_neighbors = maze.getNeighbors(curr_pos[0], curr_pos[1], curr_pos[2])

        for neighbor in valid_neighbors:
            if neighbor not in explored:
                frontier.append(neighbor)
                parent_set[neighbor] = curr_pos

    temp_pos = end_pos

    explored_path = []

    while temp_pos != start_pos and temp_pos in parent_set:
        explored_path.append(temp_pos)
        temp_pos = parent_set[temp_pos]
    
    if not explored_path:
        return None
    
    explored_path.append(start_pos)

    explored_path.reverse()

    return explored_path
