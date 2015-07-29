The data server and test code are written using the nodejs language. 

The test_data_server writes randomly generated 3d points that are in spherical coordinate system to all connected clients.  
The byte stream is in the following format. (All data is Little Endian)

| float32 radius | float32 theta | float32 phi | ...

Other applications can be written in any language on any platform and can connect to the data server and begin
processing the data.

1. In order to run the code install nodejs and npm(node package manager) on your computer.
2. Using terminal change to /directory/where/code/is/ and type npm install
3. Run the data server 'node test_data_server.js'
4. In another terminal run the test client  'node test_client.js 127.0.0.1 12345'

The server should say 'client connected' and the client app should begin printing 
randomly generated data that it recieves from the server app.


