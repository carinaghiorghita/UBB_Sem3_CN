import socket

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print("Socket created successfully\n")

port=1500

s.bind(('',port))
print("Socket binded to port\n")

s.listen(50)
print("Socket is listening\n")

cs,addr=s.accept()
print("Got connection from"+str(addr)+"\n")

b=cs.recv(50)
print(b.decode())
arr=[1,2,3,4]
for i in (0,len(arr)):
    byt=arr[i].to_bytes(1,'big')
    cs.send(byt)
cs.close()