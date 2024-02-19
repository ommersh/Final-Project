from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D

def CreateGraph(readfile):
    df = pd.read_csv(readfile)
    # Example data from DataFrame
    x1 = df['r1x']
    y1 = df['r1y']
    z1 = df['r1z']
    t = df['t']

    # Create a figure and axis
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Set initial scatter plot
    scat = ax.scatter([], [], [], c='red', s=50)

    # Set axis labels
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Update function for animation

    # Show the animation
    plt.show()

def CreateAnimation(readfile):
    df = pd.read_csv(readfile)
    # Example data from DataFrame
    x1 = df['r1x']
    y1 = df['r1y']
    z1 = df['r1z']
    x2 = df['r2x']
    y2 = df['r2y']
    z2 = df['r2z']
    t = df['t']

    # Create a figure and axis
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Set axis labels
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Update function for animation
    def update(frame):
        ax.cla()  # Clear the previous frame
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.set_xlim(-5000, 5000)
        ax.set_ylim(-5000, 5000)
        ax.set_zlim(-5000, 5000)

        # Plot all the points up to the current frame
        current_x1 = x1[:frame]
        current_y1 = y1[:frame]
        current_z1 = z1[:frame]
        ax.scatter(current_x1, current_y1, current_z1, c='gray', s=50)

        current_x2 = x2[:frame]
        current_y2 = y2[:frame]
        current_z2 = z2[:frame]
        ax.scatter(current_x2, current_y2, current_z2, c='purple', s=50,)

        # Plot the moving point as a separate scatter plot
        current_x1_point = x1[frame]
        current_y1_point = y1[frame]
        current_z1_point = z1[frame]
        ax.scatter(current_x1_point, current_y1_point, current_z1_point, c='green', s=100)

        current_x2_point = x2[frame]
        current_y2_point = y2[frame]
        current_z2_point = z2[frame]
        ax.scatter(current_x2_point, current_y2_point, current_z2_point, c='red', s=100)

        ax.scatter(0, 0, 0, c='#3b5bB4', s=700)
        return ax

    # Create animation
    ani = FuncAnimation(fig, update, frames=len(t), interval=1000, blit=False)

    # Show the animation
    plt.show()

CreateAnimation('data/LEMUR2_COSMOS_GraphData_120.csv')


