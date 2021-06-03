IRC SERVER "IRISHA"
Introduction
IRC (Internet Relay Chat) - is an application layer protocol that facilitates communication in the form of text. Was very popular in 90-s. People used IRC to communicate long before facebook, telegram, whatsApp e.t.c. Read about it on Wiki.

This project is about:
Sockets
Client-server interaction
Server-server interaction
Main part
Irisha work according to the IRC RFCs: 1459, 2810, 2811, 2812, 2813.
It means that users can connect to the network using any client works according to the RFC (HexChat for example).
Irisha can connect to other IRC servers and create the IRC network (tested with ngIRCd).

How to launch
Configuration
There is "irisha.conf". You can configure server name, welcome message (for clients), ping timeout, connection timeout, admin info, operator password.

Execution
Run "make" command to compile project in the root.
Execute "ircserv" (./ircserv in terminal) with arguments
./ircserv "port" "password" - if you want to start server and create new network
./ircserv "hostname:network_port:network_password port password" - if you want to connect to other server
Other possibilities:

make clean - delete all .o file
make fclean - delete all .o file and execution file
make re - recompile project
