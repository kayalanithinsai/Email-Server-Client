# Email client-server architecture

Use PORT number as **2567**

Instructions to Compile in same machines:

1) Open the terminal and set the path accordingly.

2) Run the Make command in the terminal (emailserver, emailclient executables will get created ).
```
make
```
3) Run the below command to run the emailserver with PORT number - 2567 as argument (./emailserver <PORT>)
```
./emailserver 2567
```
4) Run the below command to run the emailclient with PORT number - 2567 and Server IP as an argument (./emailclient <serverIp> <PORT>)
```
./emailclient 127.0.0.1 2567
```
Instructions to Compile in different machines:

Follow the same instructions as above but choose the PORT and Server IP accordingly on their respective machines.
