# clientForkServTCP4
Simple Client (with DNS resolution) / ForkServer TCPv4<br>

ps1: client_old.c using gethostbyname(), deprecated. client.c using getaddrinfo().<br>
ps2: The best compatibility practice is to define ai_family (client and server) always as AF_UNSPEC (for getaddrinfo() and socket()).
