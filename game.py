import tkinter as tk
import numpy as np
from tkinter import messagebox

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
		self.board = np.zeros((board_size,board_size))
		self.sock = sock
		self.turn = turn

	def get_message(self):
		data = self.sock.recv(64).decode('utf-8')
		print('message ' + data)
		player, x, y, turn = int(data.split(',')[0]), int(data.split(',')[1]), int(data.split(',')[2]), int(data.split(',')[3])

		if(player==4):
			self.player = 1
			self.turn = 1
			print('# if 4')
			tk.messagebox.showinfo("TikTakToe","Connected! You're the Player 1 - X")
			self.parent.title("TikTakToe - Player "+str(self.player))
		elif(player==5):
			print('# if 5')
			self.player = 2
			tk.messagebox.showinfo("TikTakToe","Connected! You're the Player 2 - O")
			self.parent.title("TikTakToe - Player "+str(self.player))
		elif(player==1 or player == 2):
			print('# if 1 2')
			self.draw(x,y,player,turn)

		elif(player==6 or player == 7):
			if(messagebox.askyesno("We have a winner!","Player {} won! Want to play again?".format(turn))):
				print('yes')
				self.restart()
			else:
				exit()
		elif(player==8):
			if(messagebox.askyesno("Draw!","Draw! Want to play again?")):
				print('yes')
				self.restart()
			else:
				exit()

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

		for row in range(self.board_size):
			for col in range(self.board_size):
				x1 = (col * self.size)
				y1 = (row * self.size)
				x2 = x1 + self.size
				y2 = y1 + self.size
				self.canvas.create_rectangle(x1, y1, x2, y2, outline="black", tags='square')

		self.get_message()		
		
	def restart(self):
		"""
		generate a new instance of the game.
		"""
		self.destroy()
		self.__init__()
		self.initGame(self.parent)
		self.pack()	  

	def draw(self, x0, y0, player, turn):
		fig_size = self.size/3
		x, y = (self.size/2) + (self.size * x0), (self.size/2) + (self.size * y0)
		self.board[x0, y0] = self.player
		if(player == 1):
			self.canvas.create_line(x-fig_size, y-fig_size,x+fig_size, y+fig_size,width=32, fill="red")
			self.canvas.create_line(x+fig_size, y-fig_size,x-fig_size, y+fig_size,width=32, fill="red")
		elif(player==2):
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
		print('click')
		print(self.player, self.turn)
		x0, y0 = int(event.x/self.size), int(event.y/self.size)
		if((self.board[x0][y0] == 0) and (self.turn == 1)):
			print('player  turn')
			print(self.player, self.turn)
			self.draw(x0,y0, self.player, 0)
			self.sock.send(bytes(str(x0)+","+str(y0), encoding='utf-8'))
		self.get_message()
		
					 
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
