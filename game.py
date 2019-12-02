import tkinter as tk
import numpy as np
from tkinter import messagebox
import threading
from time import sleep

# Message variables
P1_MUST_PLAY    = 1
P2_MUST_PLAY    = 2
YOU_ARE_P1      = 3
YOU_ARE_P2      = 4
P1_WON          = 5
P2_WON          = 6
DRAW            = 7
PLAY_AGAIN      = 8
EXIT_GAME       = 9  

class TicTacToe(tk.Frame):
	def __init__(self, sock=None, parent=None, board_size=3, size=256):
		"""
		initialize the game class
		
		Arguments:
		board size -- dimension of the board in number of cells in a row.
		size = size of the each row in pixels.
		"""
		self.board_size = board_size
		self.size = size
		self.player = 0
		self.sock = sock
		self.board = np.zeros((self.board_size,self.board_size))
		self.i_play_again = 0
		self.adversary_play_again = 0

		# Create a new thread only if a client is running this file
		if self.sock != None:
			cThread = threading.Thread(target=self.getMessage)
			cThread.daemon = True # Program becomes able to exit even if many thread are being executed
			cThread.start()

	def processMessage(self, message, x, y):
		if(message == YOU_ARE_P1):
			self.player = 1
			tk.messagebox.showinfo("TicTacToe","Connected! You're the Player 1 - X")
			self.parent.title("TicTacToe - Player " + str(self.player))

		elif(message == YOU_ARE_P2):
			self.player = 2
			tk.messagebox.showinfo("TicTacToe","Connected! You're the Player 2 - O")
			self.parent.title("TicTacToe - Player " + str(self.player))

		elif(message == P1_MUST_PLAY or message == P2_MUST_PLAY):
			self.draw(x, y, message)

		elif(message == PLAY_AGAIN):
			self.adversary_play_again = 1

		elif(message == EXIT_GAME):
			# Closes the window and connection
			tk.messagebox.showinfo("TicTacToe","Your adversary don't want to play more =(")
			sleep(3)
			self.parent.destroy()
			exit()

		else:
			if(message == P1_WON and self.player == 1) or (message == P2_WON and self.player == 2):
				text = "You win! Do you want to play again?"

			elif((message == P1_WON and self.player == 2) or (message == P2_WON and self.player == 1)):
				text = "You lose! Do you want to play again?"
				
			elif(message == DRAW):
				text = "Draw! Do you want to play again?"

			if(messagebox.askyesno("Match is over!", text)):
				self.i_play_again = 1
				self.sock.send(bytes('%d0'%(PLAY_AGAIN), encoding='utf-8'))
				print('Player[%d] sent: |'%(self.player) + 'PLAY_AGAIN' + '|')
				tk.messagebox.showinfo("TicTacToe","Waiting for the other player...")
			else:
				self.sock.send(bytes('%d0'%(EXIT_GAME), encoding='utf-8'))
				print('Player[%d] sent: |'%(self.player) + 'EXIT_GAME' + '|')
				
				# Closes the window and connection
				self.parent.destroy()
				exit()

	def getMessage(self):
		while True:
			if self.i_play_again and self.adversary_play_again:
				print('Player[%d] entrou'%(self.player))
				# Reset match
				self.restart()
				continue

			data = self.sock.recv(2048).decode('utf-8')
			print('Player[%d] received: |'%(self.player) + str(data) + '|')
			if not data:
				self.sock.close()
				break

			message, x, y = int(data[0]), int(data[1]), int(data[2])

			self.processMessage(message, x, y)

	def initGame(self, parent):
		"""
		Create an interface.
		parent -- tkinter parent window that will be used.
		"""

		self.parent = parent
		width = self.board_size * self.size
		height = self.board_size * self.size
		tk.Frame.__init__(self, self.parent)

		self.canvas = tk.Canvas(self, borderwidth=0, highlightthickness=0, width=width, height=height)
		self.canvas.pack()
		self.canvas.bind('<Button-1>', self.click)
		self.parent.title("TicTacToe"+str(self.player))
		self.board = np.zeros((self.board_size,self.board_size))

		for row in range(self.board_size):
			for col in range(self.board_size):
				x1 = (col * self.size)
				y1 = (row * self.size)
				x2 = x1 + self.size
				y2 = y1 + self.size
				self.canvas.create_rectangle(x1, y1, x2, y2, outline="black", tags='square')	
		
	def restart(self):
		"""
		generate a new instance of the game.
		"""
		self.destroy()

		# Change player's turn (p1 becomes p2 and vice-versa)
		self.player = 1 + (self.player%2)
		
		self.board = np.zeros((self.board_size,self.board_size))
		self.i_play_again = 0
		self.adversary_play_again = 0

		self.initGame(self.parent)

		# Update the screen
		self.pack()

	def draw(self, x0, y0, message):
		fig_size = self.size/3
		x, y = (self.size/2) + (self.size * x0), (self.size/2) + (self.size * y0)
		self.board[x0, y0] = self.player
		
		if(message == P1_MUST_PLAY):
			self.canvas.create_line(x-fig_size, y-fig_size,x+fig_size, y+fig_size,width=32, fill="red")
			self.canvas.create_line(x+fig_size, y-fig_size,x-fig_size, y+fig_size,width=32, fill="red")
		
		elif(message == P2_MUST_PLAY):
			x1 = x - fig_size
			y1 = y - fig_size
			x2 = x + fig_size
			y2 = y + fig_size
			self.canvas.create_oval(x1, y1, x2, y2, outline="blue",fill="blue", width=2)
		
	def click(self, event):
		"""
		Makes a move for the current player.
		If the clicked square is empty, makes a move.

		Arguments:
		event -- a tkinter event
		"""
		
		x0, y0 = int(event.x/self.size), int(event.y/self.size)
		if(self.board[x0][y0] == 0):
			self.draw(x0,y0, self.player)
			self.sock.send(bytes('%d%d'%(x0,y0), encoding='utf-8'))
					 
def run(sock):
	root = tk.Tk()
	tk.messagebox.showinfo("TicTacToe","Welcome to TicTacToe! Waiting for the other player...")
	game = TicTacToe(sock=sock)

	game.initGame(root)
	game.pack()
	root.mainloop()

def main():
	run()

if __name__ == "__main__":
	main()
