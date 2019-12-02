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

class TicTacToeServer():
	"""docstring for TicTacToeServer"""
	def __init__(self, boardSize=3):
		# The board starts cleaned
		self.boardSize = boardSize # Number of rows and columns
		self.board = None

	def resetBoard(self):
		"""
			Fill all the board with zeros.
		"""
		self.board = np.zeros((self.boardSize,self.boardSize))		

	def winner(self):
		"""
		Check if there is a draw, winner, or none of them.

		Return:
			0 -- No win or draw
			1 -- If player 1 has won
			2 -- If player 2 has won
			3 -- If there was a draw
		"""

		for player in [1,2]:
			won = np.full((self.boardSize), player)

			# Check diagonals
			if(np.array_equal(np.diag(self.board), won)): return player
			if(np.array_equal(np.diag(np.fliplr(self.board)), won)): return player

			# Check lines and columns
			for i in range(self.boardSize):
				if(np.array_equal(self.board[i], won)): return player
				if(np.array_equal(self.board[:,i], won)): return player

		# Draw
		if(not(0 in self.board)): return 3

		# No win or draw
		return 0		

class TicTacToeClient(tk.Frame):
	"""docstring for TicTacToeClient"""
	def __init__(self, sock, boardSize=3, squareSize=128):
		"""
			Constructor.

			@Parameters:
				sock: socket
				boardSize: Number of rows and columns
				squareSize: Square size of the board game (in pixels)
		"""

		super(TicTacToeClient, self).__init__()
		self.boardSize = boardSize # Number of rows and columns
		self.squareSize = squareSize # Square board size	
		self.sock = sock
		self.board = None
		self.player = None # Player number (can assume 1 or 2)
		self.parent = None # Tkinter parent window that will be used.

	def resetBoard(self):
		"""
			Fill all the board with zeros.
		"""
		self.board = np.zeros((self.boardSize,self.boardSize))	

	def draw(self, x0, y0, message):
		fig_size = self.squareSize/3
		x, y = (self.squareSize/2) + (self.squareSize * x0), (self.squareSize/2) + (self.squareSize * y0)
		
		if(message == P1_MUST_PLAY):
			self.canvas.create_line(x-fig_size, y-fig_size, x+fig_size, y+fig_size, width=25, fill="red")
			self.canvas.create_line(x+fig_size, y-fig_size, x-fig_size, y+fig_size, width=25, fill="red")
		
		elif(message == P2_MUST_PLAY):
			x1 = x - fig_size
			y1 = y - fig_size
			x2 = x + fig_size
			y2 = y + fig_size
			self.canvas.create_oval(x1, y1, x2, y2, outline="blue",fill="blue", width=2)

	def click(self, event):
		"""
			Makes a move for the current player.
			If the clicked square is empty, makes a move (send it to the server).

			@Parameters:
				event: Tkinter event
		"""
		
		# Get play (mouse click)
		x, y = int(event.x/self.squareSize), int(event.y/self.squareSize)
		# If the square clicked is empty, does a movement
		if(self.board[x][y] == 0):
			self.board[x][y] = self.player
			self.draw(x,y, self.player)
			self.sock.send(bytes('%d%d'%(x,y), encoding='utf-8'))

	def initGame(self, parent):
		"""
			Resets the board and creates an interface.
			
			@Parameters:
				parent: tkinter parent window that will be used.
		"""

		self.resetBoard()

		self.parent = parent
		width = self.boardSize * self.squareSize
		height = self.boardSize * self.squareSize
		tk.Frame.__init__(self, self.parent)

		self.canvas = tk.Canvas(self, borderwidth=0, highlightthickness=0, width=width, height=height)
		self.canvas.pack()
		self.canvas.bind('<Button-1>', self.click)
		self.parent.title("TicTacToe")

		for row in range(self.boardSize):
			for col in range(self.boardSize):
				x1 = (col * self.squareSize)
				y1 = (row * self.squareSize)
				x2 = x1 + self.squareSize
				y2 = y1 + self.squareSize
				self.canvas.create_rectangle(x1, y1, x2, y2, outline="black", tags='square')

	def processMessage(self, message, x, y):
		"""
			Given a message from server, changes the game state.

			@Parameters:
				message: integer
				x: x coordinate
				y: y coordinate
		"""

		if(message == YOU_ARE_P1):
			self.player = 1
			tk.messagebox.showinfo("TicTacToe", "You're the Player 1 - X")
			self.parent.title("TicTacToe - Player %d"%(self.player))

		elif(message == YOU_ARE_P2):
			self.player = 2
			tk.messagebox.showinfo("TicTacToe","Connected! You're the Player 2 - O")
			self.parent.title("TicTacToe - Player %d"%(self.player))

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
			# Someone has won or there was a draw

			if(message == P1_WON and self.player == 1) or (message == P2_WON and self.player == 2):
				text = "You win! Do you want to play again?"

			elif((message == P1_WON and self.player == 2) or (message == P2_WON and self.player == 1)):
				text = "You lose! Do you want to play again?"
				self.draw(x, y, 1 + (self.player%2))
				
			elif(message == DRAW):
				text = "Draw! Do you want to play again?"
				if x != 4:
					self.draw(x, y, 1 + (self.player%2))

			if(messagebox.askyesno("Match is over!", text)):
				self.i_play_again = 1
				self.sock.send(bytes('%d0'%(PLAY_AGAIN), encoding='utf-8'))
				tk.messagebox.showinfo("Restart match", "Waiting for the adversary...")
			else:
				self.sock.send(bytes('%d0'%(EXIT_GAME), encoding='utf-8'))
				
				# Closes the window and connection
				self.parent.destroy()
				exit()
