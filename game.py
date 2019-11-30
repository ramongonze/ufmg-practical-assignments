import tkinter as tk
import numpy as np
from tkinter import messagebox

class TikTakToe(tk.Frame):
    def __init__(self, parent, rows=3, board_size=3, size=256):

        self.parent = parent
        self.rows = rows
        self.board_size = board_size
        self.size = size
        self.player = 1
        self.board = np.zeros((board_size,board_size))
        
        width = board_size * size
        height = board_size * size

        tk.Frame.__init__(self, self.parent)
        self.canvas = tk.Canvas(self, borderwidth=0, highlightthickness=0,width=width, height=height)
        self.canvas.pack()
        self.canvas.bind('<Button-1>', self.click)
        self.parent.title("TikTakToe")

        for row in range(self.board_size):
            for col in range(self.board_size):
                x1 = (col * self.size)
                y1 = (row * self.size)
                x2 = x1 + self.size
                y2 = y1 + self.size
                self.canvas.create_rectangle(x1, y1, x2, y2, outline="black", tags='square')
        
                
    def winner(self):
        won =  np.full((self.board_size),self.player)

        if(np.array_equal(np.diag(self.board), won)): return 1
        if(np.array_equal(np.diag(np.fliplr(self.board)), won)): return 1

        for i in range(self.board_size):
            if(np.array_equal(self.board[i], won)): return 1
            if(np.array_equal(self.board[:,i], won)): return 1


    def click(self, event):
        x0, y0 = int(event.x/self.size), int(event.y/self.size)
        x, y = (self.size/2) + (self.size * x0), (self.size/2) + (self.size * y0)
        fig_size = self.size/3
        self.board[x0, y0] = self.player

        if(self.player == 1):
            self.canvas.create_line(x-fig_size, y-fig_size,x+fig_size, y+fig_size,width=32, fill="red")
            self.canvas.create_line(x+fig_size, y-fig_size,x-fig_size, y+fig_size,width=32, fill="red")
        else:
            x1 = x - fig_size
            y1 = y - fig_size
            x2 = x + fig_size
            y2 = y + fig_size
            self.canvas.create_oval(x1, y1, x2, y2, outline="blue",fill="blue", width=2)

        if(self.winner()):
            if(messagebox.askyesno("We have a winner!","Player {} won! Want to play again?".format(self.player))):
                self.destroy()
                self.__init__(self.parent)
                self.pack()

            else:
                exit()
        self.player = 1 + (self.player%2)    

def main():
    root = tk.Tk()
    game = TikTakToe(root)
    game.pack()
    tk.messagebox.showinfo("TikTakToe","Welcome to tiktaktoe! Player 1 is X and Player 2 is O")
    root.mainloop()    

if __name__ == "__main__":
    main()
