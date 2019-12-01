import tkinter as tk
import numpy as np
from tkinter import messagebox
import threading

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

class TikTakToe(tk.Frame):
	def __init__(self, sock=None, parent=None, board_size=3, size=256, turn=0):
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
		self.turn = turn
		self.board = np.zeros((self.board_size,self.board_size))
		self.play_again = 0
		# Create a new thread
		cThread = threading.Thread(target=self.getMessage)
		cThread.daemon = True # Program becomes able to exit even if many thread are being executed
		cThread.start()

	def getMessage(self):
		while True:
			data = self.sock.recv(2048).decode('utf-8')
			if not data:
				self.sock.close()
				break

			message, x, y, turn = int(data[0]), int(data[1]), int(data[2]), int(data[3])

			if(message == YOU_ARE_P1):
				self.player = 1
				self.turn = 1
				tk.messagebox.showinfo("TikTakToe","Connected! You're the Player 1 - X")
				self.parent.title("TikTakToe - Player " + str(self.player))
			elif(message == YOU_ARE_P2):
				self.player = 2
				tk.messagebox.showinfo("TikTakToe","Connected! You're the Player 2 - O")
				self.parent.title("TikTakToe - Player " + str(self.player))
			elif(message == P1_MUST_PLAY or message == P2_MUST_PLAY):
				self.draw(x, y, message, turn)
			elif(message == PLAY_AGAIN):
				print('play again t')
				if(self.play_again == 1):
					print('pg 1')
					self.restart()
			elif(message == EXIT_GAME):
				break		
			else:
				if(message == P1_WON and self.player == 1) or (message == P2_WON and self.player == 2):
					text = "You win! Do you want to play again?"
				elif((message == P1_WON and self.player == 2) or (message == P2_WON and self.player == 1)):
					text = "You lose! Do you want to play again?"
				elif(message == DRAW):
					text = "Draw! Want to play again?"

				if(messagebox.askyesno("Match is over!", text)):
					self.sock.send(bytes('%d%d'%(PLAY_AGAIN, self.player), encoding='utf-8'))
				else:
					self.sock.send(bytes('%d%d'%(EXIT_GAME, self.player), encoding='utf-8'))
					break


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
		self.parent.title("TikTakToe"+str(self.player))
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
		print('restart')
		self.destroy()
		self.run(self.parent) 

	def draw(self, x0, y0, message, turn):
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
		
		self.turn = turn
		
	def click(self, event):
		"""
		Makes a move for the current player.
		If the clicked square is empty, makes a move.

		Arguments:
		event -- a tkinter event
		"""
		
		x0, y0 = int(event.x/self.size), int(event.y/self.size)
		if((self.board[x0][y0] == 0) and (self.turn == 1)):
			self.draw(x0,y0, self.player, 0)
			self.sock.send(bytes('%d%d'%(x0,y0), encoding='utf-8'))		
					 
def run(sock):
	root = tk.Tk()
	tk.messagebox.showinfo("TikTakToe","Welcome to tiktaktoe! Waiting for the other player...")
	game = TikTakToe(sock=sock)

	game.initGame(root)
	game.pack()
	root.mainloop()

def main():
	run()

if __name__ == "__main__":
	main()
