import socket, sys
import threading

data = None

def get_data():
    while True:
        try:
            print("server")
            data, addr = s.recvfrom(1024)
        
        except Exception:
            pass

port = 1234

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

host = socket.gethostbyname('0.0.0.0')

s.bind((host, port))

t = threading.Thread(target=get_data)
t.daemon = True
t.start()

if(data != None):
    print(data)
else: 
    print("data")

    

s.close()
sys.exit()