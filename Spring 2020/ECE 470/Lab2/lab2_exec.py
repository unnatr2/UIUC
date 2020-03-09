#!/usr/bin/env python

import copy
import time
import rospy
import numpy as np
from lab2_header import *
import sys

# 20Hz
SPIN_RATE = 20 

# UR3 home location
home = np.radians([120, -90, 90, -90, -90, 0])

# UR3 current position, using home position for initialization
current_position = copy.deepcopy(home)


############## Your Code Starts Here ##############


Q11 = np.radians([149.84, -53.64, 116.38, -153.81, -90.09, 10.62])
Q12 = np.radians([149.84, -60.69, 115.49, -145.85, -90.09, 10.66])
Q13 = np.radians([149.80, -67.22, 113.29, -137.13, -90.10, 10.70])
Q14 = np.radians([149.78, -72.77, 109.82, -128.11, -90.10, 10.63])

Q21 = np.radians([157.94, -54.70, 118.30, -154.68, -90.24, 18.73])
Q22 = np.radians([157.92, -62.04, 117.31, -146.36, -90.24, 18.77])
Q23 = np.radians([157.91, -68.48, 115.11, -137.72, -90.24, 18.81])
Q24 = np.radians([157.89, -74.02, 111.69, -128.76, -90.24, 18.84])

Q31 = np.radians([166.54, -54.61, 117.68, -154.17, -90.41, 27.33])
Q32 = np.radians([166.53, -61.84, 116.68, -145.94, -90.41, 27.37])
Q33 = np.radians([166.51, -68.16, 114.50, -137.44, -90.41, 27.41])
Q34 = np.radians([166.49, -73.53, 111.17, -128.74, -90.41, 27.44])

### Hint: How can you map this array to the towers?
Q = [ [home, home, home, home], \
	  [home, Q11, Q21, Q31], \
      [home, Q12, Q22, Q32], \
      [home, Q13, Q23, Q33], \
      [home, Q14, Q24, Q34] ]

















############### Your Code Ends Here ###############


thetas = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

digital_in_0 = 0
analog_in_0 = 0

suction_on = True
suction_off = False

current_io_0 = False
current_position_set = False


############## Your Code Starts Here ##############

"""
TODO: define a ROS topic callback funtion for getting the state of suction cup
Whenever ur3/gripper_input publishes info this callback function is called.
"""

def suction_callback(msg):

	global suction_feedback

	suction_feedback = msg.DIGIN














############### Your Code Ends Here ###############


"""
Whenever ur3/position publishes info, this callback function is called.
"""
def position_callback(msg):

	global thetas
	global current_position
	global current_position_set

	thetas[0] = msg.position[0]
	thetas[1] = msg.position[1]
	thetas[2] = msg.position[2]
	thetas[3] = msg.position[3]
	thetas[4] = msg.position[4]
	thetas[5] = msg.position[5]

	current_position[0] = thetas[0]
	current_position[1] = thetas[1]
	current_position[2] = thetas[2]
	current_position[3] = thetas[3]
	current_position[4] = thetas[4]
	current_position[5] = thetas[5]

	current_position_set = True


def gripper(pub_cmd, loop_rate, io_0):

	global SPIN_RATE
	global thetas
	global current_io_0
	global current_position

	error = 0
	spin_count = 0
	at_goal = 0

	current_io_0 = io_0

	driver_msg = command()
	driver_msg.destination = current_position
	driver_msg.v = 1.0
	driver_msg.a = 1.0
	driver_msg.io_0 = io_0   
	pub_cmd.publish(driver_msg)

	while(at_goal == 0):

		if( abs(thetas[0]-driver_msg.destination[0]) < 0.0005 and \
			abs(thetas[1]-driver_msg.destination[1]) < 0.0005 and \
			abs(thetas[2]-driver_msg.destination[2]) < 0.0005 and \
			abs(thetas[3]-driver_msg.destination[3]) < 0.0005 and \
			abs(thetas[4]-driver_msg.destination[4]) < 0.0005 and \
			abs(thetas[5]-driver_msg.destination[5]) < 0.0005 ):

			at_goal = 1
		
		loop_rate.sleep()

		if(spin_count >  SPIN_RATE*5):

			pub_cmd.publish(driver_msg)
			rospy.loginfo("Just published again driver_msg")
			spin_count = 0

		spin_count = spin_count + 1

	return error


def move_arm(pub_cmd, loop_rate, dest, vel, accel):

	global thetas
	global SPIN_RATE

	error = 0
	spin_count = 0
	at_goal = 0

	driver_msg = command()
	driver_msg.destination = dest
	driver_msg.v = vel
	driver_msg.a = accel
	driver_msg.io_0 = current_io_0
	pub_cmd.publish(driver_msg)

	loop_rate.sleep()

	while(at_goal == 0):

		if( abs(thetas[0]-driver_msg.destination[0]) < 0.0005 and \
			abs(thetas[1]-driver_msg.destination[1]) < 0.0005 and \
			abs(thetas[2]-driver_msg.destination[2]) < 0.0005 and \
			abs(thetas[3]-driver_msg.destination[3]) < 0.0005 and \
			abs(thetas[4]-driver_msg.destination[4]) < 0.0005 and \
			abs(thetas[5]-driver_msg.destination[5]) < 0.0005 ):

			at_goal = 1
			#rospy.loginfo("Goal is reached!")
		
		loop_rate.sleep()

		if(spin_count >  SPIN_RATE*5):

			pub_cmd.publish(driver_msg)
			rospy.loginfo("Just published again driver_msg")
			spin_count = 0

		spin_count = spin_count + 1

	return error


############## Your Code Starts Here ##############

def move_block(pub_cmd, loop_rate, start_loc, start_height, \
	           end_loc, end_height):
	global Q
	
	move_arm(pub_cmd, loop_rate, Q[4][start_loc], 4.0, 4.0)
	move_arm(pub_cmd, loop_rate, Q[start_height][start_loc], 4.0, 4.0)

	gripper(pub_cmd, loop_rate, suction_on)
	time.sleep(1.0)

	if(suction_feedback == 0):
		error = 1
		return error

	move_arm(pub_cmd, loop_rate, Q[4][start_loc], 4.0, 4.0)
	move_arm(pub_cmd, loop_rate, Q[4][end_loc], 4.0, 4.0)
	move_arm(pub_cmd, loop_rate, Q[end_height][end_loc], 4.0, 4.0)
	
	gripper(pub_cmd, loop_rate, suction_off)
	time.sleep(1.0)
	
	move_arm(pub_cmd, loop_rate, Q[4][end_loc], 4.0, 4.0)
	
	error = 0

	return error



















############### Your Code Ends Here ###############


def main():

	global home
	global Q
	global SPIN_RATE

	# Initialize ROS node
	rospy.init_node('lab2node')

    # Initialize publisher for ur3/command with buffer size of 10
	pub_command = rospy.Publisher('ur3/command', command, queue_size=10)

	# Initialize subscriber to ur3/position and callback fuction
	# each time data is published
	sub_position = rospy.Subscriber('ur3/position', position, position_callback)

	############## Your Code Starts Here ##############
	# TODO: define a ROS subscriber for ur3/gripper_input message and corresponding callback function

	sub_suction = rospy.Subscriber('ur3/gripper_input', gripper_input, suction_callback)


	############### Your Code Ends Here ###############


	############## Your Code Starts Here ##############
	# TODO: modify the code below so that program can get user input

	input_done = 0
	loop_count = 0

	while(not input_done):
		input_string = raw_input("Enter number of loops <Either 1 2 3 or 0 to quit> ")
		print("You entered " + input_string + "\n")

		if(int(input_string) == 1):
			input_done = 1
			loop_count = 1
		elif (int(input_string) == 2):
			input_done = 1
			loop_count = 2
		elif (int(input_string) == 3):
			input_done = 1
			loop_count = 3
		elif (int(input_string) == 0):
			print("Quitting... ")
			sys.exit()
		else:
			print("Please just enter the character 1 2 3 or 0 to quit \n\n")














	############### Your Code Ends Here ###############

	# Check if ROS is ready for operation
	while(rospy.is_shutdown()):
		print("ROS is shutdown!")

	rospy.loginfo("Sending Goals ...")

	loop_rate = rospy.Rate(SPIN_RATE)

	############## Your Code Starts Here ##############
	# TODO: modify the code so that UR3 can move a tower to a new location according to user input

	start_input_done = 0
	end_input_done = 0
	start_loc = 0
	end_loc = 0

	while(not start_input_done):
		input_string = raw_input("Enter start location: ")
		print("You entered " + input_string + "\n")

		if(int(input_string) == 1):
			start_input_done = 1
			start_loc = 1
		elif (int(input_string) == 2):
			start_input_done = 1
			start_loc = 2
		elif (int(input_string) == 3):
			start_input_done = 1
			start_loc = 3
		else:
			print("Please just enter the character 1 2 or 3 to quit \n\n")

	while(not end_input_done):
		input_string = raw_input("Enter end location: ")
		print("You entered " + input_string + "\n")

		if(int(input_string) == 1 and int(input_string) != start_loc):
			end_input_done = 1
			end_loc = 1
		elif (int(input_string) == 2 and int(input_string) != start_loc):
			end_input_done = 1
			end_loc = 2
		elif (int(input_string) == 3 and int(input_string) != start_loc):
			end_input_done = 1
			end_loc = 3
		else:
			print("Please just enter the character 1 2 or 3 to quit \n\n")

	int_loc = start_loc ^ end_loc

	while(loop_count > 0):

		move_arm(pub_command, loop_rate, home, 4.0, 4.0)

		rospy.loginfo("Sending start location ...")
		move_arm(pub_command, loop_rate, Q[4][start_loc], 4.0, 4.0)

		rospy.loginfo("Sending end location ...")
		move_arm(pub_command, loop_rate, Q[4][end_loc], 4.0, 4.0)

		err = move_block(pub_command, loop_rate, start_loc, 3, end_loc, 1)
		if(err != 0):
			break
		err = move_block(pub_command, loop_rate, start_loc, 2, int_loc, 1)
		if(err != 0):
			break
		err = move_block(pub_command, loop_rate, end_loc, 1, int_loc, 2)
		if(err != 0):
			break
		err = move_block(pub_command, loop_rate, start_loc, 1, end_loc, 1)
		if(err != 0):
			break
		err = move_block(pub_command, loop_rate, int_loc, 2, start_loc, 1)
		if(err != 0):
			break
		err = move_block(pub_command, loop_rate, int_loc, 1, end_loc, 2)
		if(err != 0):
			break
		err = move_block(pub_command, loop_rate, start_loc, 1, end_loc, 3)
		if(err != 0):
			break

		

		loop_count = loop_count - 1

	gripper(pub_command, loop_rate, suction_off)

	move_arm(pub_command, loop_rate, home, 4.0, 4.0)








	############### Your Code Ends Here ###############



if __name__ == '__main__':
	
	try:
		main()
    # When Ctrl+C is executed, it catches the exception
	except rospy.ROSInterruptException:
		pass


	






