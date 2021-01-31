# Extended Kalman Filter Project

Self-Driving Car Engineer Nanodegree Program ([Udacity Starter Code](https://github.com/udacity/CarND-Extended-Kalman-Filter-Project))

In this project we utilize a kalman filter to estimate the state of a moving object of interest with noisy lidar and radar measurements. Passing the project requires obtaining RMSE values that are lower than the tolerance outlined in the project rubric. 

This project involves the Term 2 Simulator which can be downloaded [here](https://github.com/udacity/self-driving-car-sim/releases).

This repository includes two files that can be used to set up and install [uWebSocketIO](https://github.com/uWebSockets/uWebSockets) for either Linux or Mac systems. For windows you can use either Docker, VMware, or even [Windows 10 Bash on Ubuntu](https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10/) to install uWebSocketIO. Please see the uWebSocketIO Starter Guide page in the classroom within the EKF Project lesson for the required version and installation scripts.

Once the install for uWebSocketIO is complete, the main program can be built and run by doing the following from the project top directory.

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./ExtendedKF


**INPUT**: values provided by the simulator to the c++ program

["sensor_measurement"] => the measurement that the simulator observed (either lidar or radar)


**OUTPUT**: values provided by the c++ program to the simulator

["estimate_x"] <= kalman filter estimated position x

["estimate_y"] <= kalman filter estimated position y

["rmse_x"]

["rmse_y"]

["rmse_vx"]

["rmse_vy"]

---

## Dependencies

* cmake >= 3.5
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make` 
   * On windows, you may need to run: `cmake .. -G "Unix Makefiles" && make`
4. Run it: `./ExtendedKF `

## My Simulator and Environment Set-up (Ubuntu 20.04.1 LTS)

### Simulator Setup

The instructions are from [here](https://medium.com/@kaigo/how-to-install-udacitys-self-driving-car-simulator-on-ubuntu-20-04-14331806d6dd).

1. Download the (.deb) package of Unity (3D) version 5.5.1f1 that the Udacity Simulator uses. 
2. Install dependencies : `sudo apt install gconf-service lib32gcc1 lib32stdc++6 libc6-i386 libgconf-2-4 npm`
3. Run the install : `sudo dpkg -i ~/Downloads/unity-editor_amd64-5.5.1xf1Linux.deb`
4. If you get error about unmet dependencies you may need to run, and retry ` sudo apt --fix-broken install` 
5. With Unity working now, download and run the latest release of the Udacity Term 2 [Simulator](https://github.com/udacity/self-driving-car-sim/releases). 

### Environment Setup

I used VSCode IDE for this project. Follow the thorough instructions provided by [yosoufe](https://gist.github.com/yosoufe/dd37284b7319c484dd77e42947fc82b7) to setup the environment. The instructions cover debugging as well which maybe useful.

##   Results

I ran the simulation four times:

1. Dataset1, using lidar and radar
2. Dataset1, using lidar only
3. Dataset1, using radar only
4. Dataset2, using lidar and radar

The output files and plotting script are in the ./output_files folder.

### Dataset1, using both the lidar and radar (lidar = true, radar = true)

The conde works well, with RMSE = [0.0964425, 0.0852905, 0.415426, 0.431636]. Check output [file.](https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/tree/master/output_files/dataset1_11.out)

The simulator output is shown below : 

| Position     | RMSE         |
|:------------:|:------------:|
|<img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset1_11_position.jpg" width="480" height="270"/> | <img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset1_11_rmse.jpg" width="480" height="270"/> |


### Dataset1, using lidar alone (lidar = true, radar = false)

The predictions are slightly worse, with RMSE = [0.146952, 0.115285, 0.609732, 0.537763]. Check output [file.](https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/tree/master/output_files/dataset1_10.out)

The simulator output is shown below : 

| Position     | RMSE         |
|:------------:|:------------:|
|<img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset1_10_position.jpg" width="480" height="270"/> | <img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset1_10_rmse.jpg" width="480" height="270"/> |

### Dataset1, using radar alone (radar = true, lidar = false)

The predictions are the worst of the three options, with RMSE = [0.22789, 0.346044, 0.572788, 0.796972]. Check output [file.](https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/tree/master/output_files/dataset1_01.out)

The simulator output is shown below : 

| Position     | RMSE         |
|:------------:|:------------:|
|<img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset1_01_position.jpg" width="480" height="270"/> | <img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset1_01_rmse.jpg" width="480" height="270"/> |

### Dataset2, using both the lidar and radar (lidar = true, radar = true)

The predictions for dataset2 are about as good as those for datase1 while using lidar and radar measurements, with RMSE = [0.0725678, 0.0964738, 0.421634, 0.493199]. Check output [file.](https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/tree/master/output_files/dataset2.out)

The simulator output is shown below : 

| Position     | RMSE         |
|:------------:|:------------:|
|<img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset2_position.jpg" width="480" height="270"/> | <img src="https://github.com/prasadshingne/CarND-Extended-Kalman-Filter-Project/blob/master/output_files/dataset2_rmse.jpg" width="480" height="270"/> |


