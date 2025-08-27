We are going to create a simplified DHCP-like client and server.
You will write a server in C.
1. The server will take two arguments, the port on which it is running, and the ip
address of the machine it is running on.
2. The server will create a DGRAM socket and bind to the port the user enters as
an argument.
3. The server will set the socket to allow broadcast (see code below).
4. The server will wait for a request from the client.
5. The server will validate the op code is a 1
6. The server will validate the fromIPAddr is 0.0.0.0
7. The server will validate the toIPAddr is 255.255.255.255
8. If all these conditions are met, the server will
a. broadcast back an offer to the client.
b. Set the fromIPAddr to the server’s IP address
c. Set the opcode to 2
You will write a client in C.
1. The client will take one argument, the port on which the server is running
2. The client will create a DGRAM socket
3. The client will set the socket to allow broadcast (see code below).
4. The client will
a. Set the opcode to 1
b. Set the fromIPAddr to 0.0.0.0
c. Set the toIPAddr to 255.255.255.255
d. Set the ciaddr and yiaddr to 0.0.0.0
e. Set the transID to a randoom number
f. Broadcast the request to the server
5. The client will then wait for a response, and:
a. The client will validate the op code is a 2
b. The server will validate the toIPAddr is 255.255.255.255
6. If all these conditions are met, the server will
a. Print all the fields in the packet
