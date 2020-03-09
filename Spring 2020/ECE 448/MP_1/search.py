# search.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Michael Abir (abir2@illinois.edu) on 08/28/2018

import heapq

"""
This is the main entry point for MP1. You should only modify code
within this file -- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.
"""
# Search should return the path.
# The path should be a list of tuples in the form (row, col) that correspond
# to the positions of the path taken by your search algorithm.
# maze is a Maze object based on the maze from the file specified by input filename
# searchMethod is the search method specified by --method flag (bfs,dfs,astar,astar_multi,extra)

def search(maze, searchMethod):
    return {
        "bfs": bfs,
        "astar": astar,
        "astar_corner": astar_corner,
        "astar_multi": astar_multi,
        "extra": extra,
    }.get(searchMethod)(maze)


def bfs(maze):
    """
    Runs BFS for part 1 of the assignment.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    start_pos = maze.getStart()     # Start Position
    end_pos = maze.getObjectives()[0]  # End Position

    if start_pos == None or end_pos == None:
        return []
    
    end_explored = 0    # Flag to check if end is explored
    frontier = [start_pos]  # List to store unexplored nodes
    explored = []   # List to store explored nodes
    parent_set = {} # Dictionary to save parent-child paths

    # Loop until end explored
    while end_explored == 0:
        
        curr_pos = frontier.pop(0)  # Current position
        
        # If not in explored: add to explored, else: ignore
        if curr_pos not in explored:
            explored.append(curr_pos)
        else:
            continue

        # Check if end is reached
        if end_pos[0] == curr_pos[0] and end_pos[1] == curr_pos[1]:
            end_explored = 1
            continue

        valid_neighbors = maze.getNeighbors(curr_pos[0], curr_pos[1])   # Find valid neighbors from current position

        # Check if each neighbor is explored, if not: add to frontier and set parent to current positon
        for neighbor in valid_neighbors:
            if neighbor not in explored:
                frontier.append(neighbor)
                parent_set[neighbor] = curr_pos

    temp_pos = end_pos    
    explored_path = [temp_pos]  # Add end position to final path

    # Loop until reached start position
    while temp_pos != start_pos:
        explored_path.append(parent_set[temp_pos])
        temp_pos = parent_set[temp_pos]

    explored_path.reverse() # Reverse path to get final path
    
    return explored_path

def manhattan_dist(start_row, start_col, end_row, end_col):
    """
    Finds the manhattan distance between start and end positions.

    @param start_row: The row coordinate of the start position.
           start_col: The col coordinate of the start position.
           end_row: The row coordinate of the end position.
           end_col: The col coordinate of the end position.

    @return: The manhattan distance between the start and end positions.
    """
    return (abs(start_row-end_row) + abs(start_col-end_col))

def astar(maze):
    """
    Runs A star for part 1 of the assignment.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    start_pos = maze.getStart() # Start Position
    end_pos = maze.getObjectives()[0]   # End Position

    if start_pos == None or end_pos == None:
        return []
    
    end_explored = 0    # Flag to check if end is explored
    frontier = []   # List to store unexplored nodes
    parent_set = {} # Dictionary to store parent-child paths
    travelled_cost = {} # Dictionary to store the cost of travelling to certain position

    parent_set[start_pos] = None    # Start position doesn't have a parent
    travelled_cost[start_pos] = 0   # Distance travelled to reach start position is 0

    heapq.heappush(frontier, (0 + manhattan_dist(start_pos[0], start_pos[1], end_pos[0], end_pos[1]), start_pos))   # Push start position onto PQ with given heuristic

    # Loop until end is explored
    while end_explored == 0:

        curr_pos = heapq.heappop(frontier)[1]   # Current position
        
        # Check if end is reached
        if end_pos[0] == curr_pos[0] and end_pos[1] == curr_pos[1]:
            end_explored = 1
            continue

        valid_neighbors = maze.getNeighbors(curr_pos[0], curr_pos[1])   # Find valid neighbors of current position

        # Check if neighbor is explored or has a lower travel cost, if not: update travelled_cost, put into frontier and set parent to current position
        for neighbor in valid_neighbors:
            new_travelled_cost = travelled_cost[curr_pos] + 1
            if neighbor not in travelled_cost or new_travelled_cost < travelled_cost[neighbor]:
                travelled_cost[neighbor] = new_travelled_cost
                heapq.heappush(frontier, (travelled_cost[neighbor] + manhattan_dist(neighbor[0], neighbor[1], end_pos[0], end_pos[1]), neighbor))
                parent_set[neighbor] = curr_pos
              
    temp_pos = end_pos   
    explored_path = [end_pos] # Add end position to final path

    # Loop until reached start position
    while temp_pos != start_pos:
        explored_path.append(parent_set[temp_pos])
        temp_pos = parent_set[temp_pos]

    explored_path.reverse() # Reverse path to get final path

    return explored_path

def astar_corner(maze):
    """
    Runs A star for part 2 of the assignment in the case where there are four corner objectives.
        
    @param maze: The maze to execute the search on.
        
    @return path: a list of tuples containing the coordinates of each state in the computed path
        """
    
    start_pos = maze.getStart() # Start Position
    objective_list = maze.getObjectives() # List of objectives
    objective_list.insert(0, start_pos) # Add start position to list of objectives
    
    path_dict = {}  # Dictionary to save paths between every two objectives

    # Loop through every possible combination of two objectives and save path in dictionary
    for objective_a in range(0, 4):
        for objective_b in range(objective_a + 1, 5):
            path_dict[(objective_a, objective_b)] = astar_helper(maze, objective_list[objective_a], objective_list[objective_b])
            temp_path = [step for step in path_dict[(objective_a, objective_b)]]
            temp_path.reverse()
            path_dict[(objective_b, objective_a)] = [step for step in temp_path]

    final_path = [step for step in path_dict[(0, 1)]]   # Save first sub-path from start to first objective

    # Add sub-paths between next 3 objectives
    for obj in range(1, 4):
        temp_path = [step for step in path_dict[(obj, obj + 1)]]
        temp_path.pop(0)
        final_path = final_path + temp_path

    # Find every possible combination of paths
    for a in range(1, 5):
        for b in range(1, 5):
            if a != b:
                for c in range(1, 5):
                    if c != a and c != b:
                        for d in range(1, 5):
                            if d != a and d != b and d != c:
                                path_taken = [0, a, b, c, d]    # Save path combination taken
                                temp_final_path = [step for step in path_dict[(0, a)]]  # Save first sub-path from start to first objective
                                
                                # Add sub-paths between next 3 objectives
                                for idx in range(1, 4):
                                    new_temp_path = [step for step in path_dict[(path_taken[idx], path_taken[idx + 1])]]
                                    new_temp_path.pop(0)
                                    for step in new_temp_path:
                                        temp_final_path.append(step)
                                if len(temp_final_path) < len(final_path):
                                    final_path = temp_final_path

    return final_path

def astar_helper(maze, start_pos, end_pos):
    
    if start_pos == None or end_pos == None:
        return []
    
    end_explored = 0    # Flag to check if end is explored
    frontier = []   # List to store unexplored nodes
    parent_set = {} # Dictionary to store parent-child paths
    travelled_cost = {} # Dictionary to store the cost of travelling to certain position

    parent_set[start_pos] = None    # Start position doesn't have a parent
    travelled_cost[start_pos] = 0   # Distance travelled to reach start position is 0
    
    heapq.heappush(frontier, (0 + manhattan_dist(start_pos[0], start_pos[1], end_pos[0], end_pos[1]), start_pos))   # Push start position onto PQ with given heuristic

    # Loop until end is explored
    while end_explored == 0:

        curr_pos = heapq.heappop(frontier)[1]   # Current position
        
        # Check if end is reached
        if end_pos[0] == curr_pos[0] and end_pos[1] == curr_pos[1]:
            end_explored = 1
            continue

        valid_neighbors = maze.getNeighbors(curr_pos[0], curr_pos[1])   # Find valid neighbors of current position

        # Check if neighbor is explored or has a lower travel cost, if not: update travelled_cost, put into frontier and set parent to current position
        for neighbor in valid_neighbors:
            new_travelled_cost = travelled_cost[curr_pos] + 1
            if neighbor not in travelled_cost or new_travelled_cost < travelled_cost[neighbor]:
                travelled_cost[neighbor] = new_travelled_cost
                heapq.heappush(frontier, (travelled_cost[neighbor] + manhattan_dist(neighbor[0], neighbor[1], end_pos[0], end_pos[1]), neighbor))
                parent_set[neighbor] = curr_pos
                    
    temp_pos = end_pos  
    explored_path = [temp_pos] # Add end position to final path
    
    # Loop until reached start position
    while temp_pos != start_pos:
        explored_path.append(parent_set[temp_pos])
        temp_pos = parent_set[temp_pos]

    explored_path.reverse() # Reverse path to get final path
    
    return explored_path

def astar_multi(maze):
    """
    Runs A star for part 3 of the assignment in the case where there are
    multiple objectives.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    start_pos = maze.getStart() # Start Position
    objective_list = maze.getObjectives()   # List of objectives
    objective_list.insert(0, start_pos) # Add start to list of objectives

    path_dict = {}  # Dictionary to save paths between two objectives
    dist_dict = {}  # Dictionary to save distances between two objectives

    # Loop through all possible combination of objectives and find their paths using A*
    for obj_a in range(0, len(objective_list) - 1):
        for obj_b in range(obj_a + 1, len(objective_list)):
            obj_a_pos = objective_list[obj_a]
            obj_b_pos = objective_list[obj_b]
            path_dict[(obj_a_pos, obj_b_pos)] = astar_helper(maze, obj_a_pos, obj_b_pos)
            temp_path = [step for step in path_dict[(obj_a_pos, obj_b_pos)]]
            temp_path.reverse()
            path_dict[(obj_b_pos, obj_a_pos)] = [step for step in temp_path]
            dist_dict[(obj_a_pos, obj_b_pos)] = len(path_dict[obj_a_pos, obj_b_pos])
            dist_dict[(obj_b_pos, obj_a_pos)] = len(path_dict[obj_b_pos, obj_a_pos])


    frontier = []   # Priority Queue to find lowest heuristic

    visited = {}    # Dictionary to save visited objectives of each node
    unvisited = {}  # Dictionary to save unvisited objectives of each node
    travelled_cost = {} # Dictionary to save cost of path travelled till each node


    travelled_cost[start_pos] = 0   # No cost to travel to start position
    unvisited[start_pos] = [obj for obj in objective_list]  # Store all objectives into start position's unvisited list
    unvisited[start_pos].pop(0) # Remove start position from list
    visited[start_pos] = [start_pos]    # Add start position to its visited list
    
    heapq.heappush(frontier, [travelled_cost[start_pos] + min_manhattan_dist(start_pos, unvisited[start_pos]) + prims_mst(unvisited[start_pos], dist_dict), start_pos]) # Put start position into the PQ

    end_pos = (0, 0) # Temporary end position

    # Loop untill frontier empty
    while frontier:

        curr_pos = heapq.heappop(frontier)[1] # Current Position
        
        # if no more objectives left, set end position
        if not unvisited[curr_pos]:
            end_pos = curr_pos
            break

        # For all unvisited objectives, create their own visited and unvisited objectives, update the lists and calculate the new travelled cost. Finally, push onto PQ
        for objective in unvisited[curr_pos]:
            unvisited[objective] = [obj for obj in unvisited[curr_pos]]
            visited[objective] = [obj for obj in visited[curr_pos]]
            unvisited[objective].remove(objective)
            visited[objective].append(objective)
            travelled_cost[objective] = travelled_cost[curr_pos] + dist_dict[(curr_pos, objective)] - 1
            heapq.heappush(frontier, [travelled_cost[objective] + min_manhattan_dist(objective, unvisited[objective]) + prims_mst(unvisited[objective], dist_dict), objective])
    
    final_path = [start_pos] # Add starting position to final path

    # For all objectives visited to reach end position, copy paths and add to final path
    for position in range(len(visited[end_pos]) - 1):
        temp_path = [node for node in path_dict[(visited[end_pos][position], visited[end_pos][position + 1])]]
        temp_path.pop(0)
        final_path += temp_path

    return final_path

def min_manhattan_dist(curr_pos, objective_list):
    """
    Finds the closest objective from the given position and returns the manhattan distance
    to that objective.

    @param curr_pos: The current position in the maze.
           objective_list: The list of remaining objectives in the maze.

    @return min_dist: The manhattan distance between the current position and the closest
                      objective in the list of remaining objectives.
    """
    min_dist = -1 # Temporary min distance

    # Loop through all objectives and calculate minimum distance from current position to objective
    for obj in objective_list:
        if min_dist == -1 or min_dist > manhattan_dist(curr_pos[0], curr_pos[1], obj[0], obj[1]):
            min_dist = manhattan_dist(curr_pos[0], curr_pos[1], obj[0], obj[1])
    return min_dist

def prims_mst(objective_list, dist_dict):
    """
    Runs a Prim's MST Algorithm to create a minimum spanning tree connecting all
    remaining objectives in objective_list using distances provided in the dist_dict.

    Referenced https://bradfieldcs.com/algos/graphs/prims-spanning-tree-algorithm/ to
    brush up on Prim's MST Algorithm.

    @param objective_list: The list of remaining objectives in the maze.
           dist_dict: The dictionary that stores all precomputed A* path distances
                      between any two objectives.

    @return final_sum: The sum of all edge weights in the minimum spanning tree.
    """
    edge_list = []  # List to save edges of graph
    visited = []    # List of visited nodes
    pq = [] # Priority Queue
    mst = []    # List of edges in MST
    final_sum = 0   # Final sum of edges in MST

    if not objective_list:
        return 0

    curr_pos = objective_list[0] # Find first objective

    # For all objectives, create edges and save weights into edge list
    for obj_a in range(0, len(objective_list) - 1):
        for obj_b in range(obj_a + 1, len(objective_list)):
            obj_a_pos = objective_list[obj_a]
            obj_b_pos = objective_list[obj_b]
            edge_list.append((dist_dict[obj_a_pos, obj_b_pos] - 1, obj_a_pos, obj_b_pos))
            edge_list.append((dist_dict[obj_b_pos, obj_a_pos] - 1, obj_b_pos, obj_a_pos))

    # Add all edges going out from the current position in the PQ
    for edge in edge_list:
        if edge[1] == curr_pos:
            heapq.heappush(pq, edge)
    
    # Save current position as visited
    visited.append(curr_pos)

    # Keep going till all edges added
    while (len(mst) < len(objective_list) - 1):

        shortest_edge = heapq.heappop(pq) # Find shortest edge
        
        # Check if circles back, if not: add to visited and MST
        if shortest_edge[2] not in visited:
            visited.append(shortest_edge[2])
            mst.append(shortest_edge)

            # Check all adjacent edges and if they don't circle back, add to PQ
            for adjacent_edge in edge_list:
                if adjacent_edge[1] == shortest_edge[2] and adjacent_edge[2] not in visited:
                    heapq.heappush(pq, adjacent_edge)

    # Add all edge weights for final sum
    for edge in mst:
        final_sum += edge[0]

    return final_sum


def extra(maze):
    """
    Runs extra credit suggestion.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    return []
