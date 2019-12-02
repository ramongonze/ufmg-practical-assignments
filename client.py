import tkinter as tk
import numpy as np
from tkinter import messagebox
import tictactoe as ttt
import threading
import socket, select, string, sys

def createThread(func, arguments=None):
	cThread = threading.Thread(target=func, args=arguments)
	cThread.daemon = True # Program becomes able to exit even if many thread are being executed
	cThread.start()

def connect(host, port):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.settimeout(300)
	
	# connect to remote host
	try :
		sock.connect((host, port))
	except :
		print('Unable to connect to the game server!')
		sys.exit()
	
	print('Connected to the game server!')
	return sock

def getMessage(game, sock):
		"""
			Get messages from the server.
			Message format: 'abc'
				a: integer representing the message type
				b,c: coordinates, used when 'a' says that is a play

		"""

		while True:
			data = sock.recv(3).decode('utf-8')
			if not data:
				sock.close()
				break

			message, x, y = int(data[0]), int(data[1]), int(data[2])
			print('Received: |' + str(data) + '|')
			game.processMessage(message, x, y)

def main():
	if(len(sys.argv) < 3) :
		print('Usage : python client.py hostname port')
		exit()
	
	host = sys.argv[1]
	port = int(sys.argv[2])
	sock = connect(host, port)

	root = tk.Tk()
	tk.messagebox.showinfo("TicTacToe","Welcome to TicTacToe!")
	
	game = ttt.TicTacToeClient(sock)
	createThread(getMessage, (game, sock))

	game.initGame(root)
	game.pack()
	root.mainloop()

	# game.run(sock=sock)

if __name__=="__main__":
    main()

