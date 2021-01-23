#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jan 21 13:41:01 2021

@author: sprasad
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

#%% merge array
# Merge arr1[0..n1-1] and  
# arr2[0..n2-1] into  
# arr3[0..n1+n2-1] 
def mergeArrays(arr1, arr2, n1, n2): 
    arr3 = [None] * (n1 + n2) 
    i = 0
    j = 0
    k = 0
  
    # Traverse both array 
    while i < n1 and j < n2: 
      
        # Check if current element  
        # of first array is smaller  
        # than current element of  
        # second array. If yes,  
        # store first array element  
        # and increment first array 
        # index. Otherwise do same  
        # with second array 
        if arr1[i] < arr2[j]: 
            arr3[k] = arr1[i] 
            k = k + 1
            i = i + 1
        else: 
            arr3[k] = arr2[j] 
            k = k + 1
            j = j + 1
      
  
    # Store remaining elements 
    # of first array 
    while i < n1: 
        arr3[k] = arr1[i]; 
        k = k + 1
        i = i + 1
  
    # Store remaining elements  
    # of second array 
    while j < n2: 
        arr3[k] = arr2[j]; 
        k = k + 1
        j = j + 1
    
    return arr3
    #print("Array after merging") 
    #for i in range(n1 + n2): 
     #   print(str(arr3[i]), end = " ") 
#%% read data

data_column        = ["key","px","py","vx","vy"]

output_file_name   = "dataset2.out" 
data = pd.read_csv(output_file_name, sep = " ", skiprows = 3, names = data_column, index_col=False)

gt      = data[data["key"] == "ground_truth"]
est     = data[data["key"] == "estimate"]
rmse    = data[data["key"] == "RMSE"]
lidar   = data[data["key"] == "Lidar"]
radar   = data[data["key"] == "Radar"]

# %% extract timestamp and clean Lidar and Radar data
meas_time_lidar = np.asarray(lidar["vy"]/1e6)
meas_time_radar = np.asarray(radar["vy"]/1e6)

first_time_step = meas_time_lidar[0]

meas_time_lidar = meas_time_lidar - first_time_step
meas_time_radar = meas_time_radar - first_time_step

meas_time = np.asarray(mergeArrays(meas_time_lidar, meas_time_radar, len(meas_time_lidar), len(meas_time_radar)))

lidar["vy"] = 0
radar["vy"] = 0
#%% fix radar measurements
radar["rho"]     = radar["px"]
radar["theta"]   = radar["py"]
radar["rho_dot"] = radar["vx"]

radar["px"]      = radar["rho"]    *np.cos(radar["theta"])
radar["py"]      = radar["rho"]    *np.sin(radar["theta"])

radar["vx"]      = radar["rho_dot"]*np.cos(radar["theta"])
radar["vy"]      = radar["rho_dot"]*np.sin(radar["theta"])

#%% plot position

fig = plt.figure(figsize=(6.4, 5.0))
plt.scatter(lidar["px"],lidar["py"], s = 25, marker = "x", color = 'k', linewidths=1, label = "Lidar")
plt.scatter(radar["px"],radar["py"], s = 25, marker = "+", color = 'r', linewidth=1, label = "Radar")
plt.scatter(est["px"],est["py"], s = 35, marker = "1", color = 'g', linewidth=1, label = "Estimate")
plt.plot(gt["px"],gt["py"], linewidth=1.5, label = "Ground Truth")
plt.grid(color = 'k', ls = '--', lw = 0.5)
plt.legend()
plt.xlabel('X (units)')
plt.ylabel('Y (units)')
plt.title('Position [X,Y]')
fig.savefig( output_file_name[:-4]+'_position.tif', dpi = 300)

#%% plot velocity

# fig = plt.figure(figsize=(6.4, 5.0))
# # plt.scatter(lidar["px"],lidar["py"], s = 25, marker = "x", color = 'k', linewidths=1, label = "Lidar")
# plt.scatter(radar["vx"],radar["vy"], s = 25, marker = "+", color = 'r', linewidth=1, label = "Radar")
# plt.scatter(est["vx"],est["vy"], s = 35, marker = "1", color = 'g', linewidth=1, label = "Estimate")
# plt.plot(gt["vx"],gt["vy"], linewidth=1.5, label = "Ground Truth")
# plt.grid(color = 'k', ls = '--', lw = 0.5)
# plt.legend()
# plt.xlabel('VX (units)')
# plt.ylabel('VY (units)')
# plt.title('Velocity [VX,VY]')
# fig.savefig(output_file_name[:-4]+'_velocity.tif', dpi = 300)

#%% plot rmse vs time
fig = plt.figure(figsize=(6.4, 5.0))
plt.plot(meas_time,rmse["px"],  color = 'b', linewidth=1, label = "px")
plt.plot(meas_time,rmse["py"],  color = 'r', linewidth=1, label = "py")
plt.plot(meas_time,rmse["vx"],  color = 'g', linewidth=1, label = "vx")
plt.plot(meas_time,rmse["vy"],  color = 'k', linewidth=1, label = "vy")

plt.grid(color = 'k', ls = '--', lw = 0.5)
plt.legend()
plt.xlabel('time (sec)')
plt.ylabel('RMSE (units)')
plt.title('RMSE')
fig.savefig(output_file_name[:-4]+'_rmse.tif', dpi = 300)
plt.ylim(0.0, 1.0)
fig.savefig(output_file_name[:-4]+'_rmse_zoom_y.tif', dpi = 300)

