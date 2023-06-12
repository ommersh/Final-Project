from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def CreateGraph(readfile):
    df=pd.read_csv(readfile)

    x1 = df['r1x']
    y1= df['r1y']
    z1 = df['r1z']
    x2 = df['r2x']
    y2 = df['r2y']
    z2 = df['r2z']

    f = df['f']
    t = df['t']
    df = df['df']
    fig = plt.figure()
    #ax = plt.axes(projection='3d')
    #ax.plot3D(x1, y1, z1, 'gray')
    #ax.plot3D(x2, y2, z2, 'blue')
    ax = plt.axes()
    ax.plot(t, f, 'blue')
    ax.plot(t,df, 'red')

    plt.show()

CreateGraph('data/LEMUR2_COSMOS_GraphData.csv')



