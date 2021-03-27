import socket
import sys

host = '127.0.0.1'
port = 8000

clientMessage = 'Hello,kila,1842'
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((host, port))
client.sendall(bytes(clientMessage, 'utf-8'))

serverMessage = str(client.recv(1024), encoding='utf-8')
print(serverMessage)