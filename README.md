# clientForkServTCP4
Simple Client (with DNS resolution) / ForkServer TCPv4

ps1: client_old.c using gethostbyname(), deprecated. client.c using getaddrinfo().
ps2: the best thing to do is specify AF_UNSPEC at hints.ai_family.
