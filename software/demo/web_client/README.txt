The purpose of the webclient is to attach to the data server and display the data in a meaningful
way.  The current implementation plots the data points in 3d and allows the user to navigate the 
data set in first-person.  'w','s','a','d' move forward, backward, left, and right.  'z' and 'x' 
move up and down.  Press mouse button to use the mouse to look around the scene. 

The steps for running the web client are as follows.

1. install the webclient using 'npm install'
2. start up the data server or test data server.
3. run the web server using 'npm start'
4. start up the data stream client using 'node lidar_data_stream.js'
5. open a web browser and type '127.0.0.1:3000' into the address bar.

