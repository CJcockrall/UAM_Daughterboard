# UAM_Connect.py
# This script sets up a server for the ESP32 to connect to

import socket

SERVER_IP = "0.0.0.0" # Set this as your PC's IPv4 address 
SERVER_PORT = 8080 #Must match ESP32's server port

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((SERVER_IP,SERVER_PORT)) # Bind to server IP and port
    s.listen(1) # Listen for max of 1 connection
    print(f"Listening on {SERVER_IP}:{SERVER_PORT}...")
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            data = conn.recv(1024)
            if not data: # If connection closed
                break
            print(f"Received from ESP32: {data.decode(errors='ignore')}")
            reply = "Hello from PC!"
            conn.sendall(reply.encode())
        print("Connection closed.")