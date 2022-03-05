import socket, sys

port = 1234

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

host = socket.gethostbyname('0.0.0.0')

s.bind((host, port))

while True:
    data, addr = s.recvfrom(1024)
    print(data)

    if not data:
        break

s.close()
sys.exit()