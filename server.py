import socket
import threading
import tictactoe as ttt
import numpy as np

def createThread(func, arguments=None):
	cThread = threading.Thread(target=func, args=arguments)
	cThread.daemon = True # Program becomes able to exit even if many thread are being executed
	cThread.start()

class Match():
	def __init__(self):
		self.currPlayer = None # Keeps the number of the current player who must play.
		self.game = None # TicTacToe object

	def startMatch(self, player1, player2):
		"""
			Set a new game; Reset the board game; 
			Sendo messages of YOU_ARE_P{1,2}

			Warning: It requires that the variable 'self.currPlayer' has been set before with value 1 or 2

			@Parameters:
				player1: socket connection
				player2: socket connection
		"""

		self.game = ttt.TicTacToeServer()
		self.game.resetBoard()
		
		if self.currPlayer == 1:
			player1.send(bytes('%d00'%(ttt.YOU_ARE_P1), encoding='utf-8'))
			player2.send(bytes('%d00'%(ttt.YOU_ARE_P2), encoding='utf-8'))
		else:
			player1.send(bytes('%d00'%(ttt.YOU_ARE_P2), encoding='utf-8'))
			player2.send(bytes('%d00'%(ttt.YOU_ARE_P1), encoding='utf-8'))

class Server:
	def __init__(self, port, nClients, dataSize, ip='0.0.0.0'):
		"""
			
			@Parameters:
				port: Port used to listen.
				nClients: Maximum number of simultaneous connections.
				ip: address to listen. It has to be '0.0.0.0' if the server is listening all addresses.
				dataSize: Maximum number of bytes the server receives in each message.
		"""

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # SOCK_STREAM because it is a TCP connection
		self.ip = ip
		self.port = port
		self.dataSize = dataSize
		self.player1 = None
		self.player2 = None
		self.resetMatch = 0 # It keeps the number of players who answered 'ttt.PLAY_AGAIN'
		self.matchNumber = 1 # Used to swap which player starts playing

		self.sock.bind((ip, port))
		self.sock.listen(nClients)
		self.match = Match()

	def manageMatch(self):
		while True:
			if self.resetMatch == 2:
				
				# Decides which player is going to play at first
				self.match.currPlayer = 1 + (self.matchNumber%2)
				
				# Start a new match
				self.resetBoard()
				
				self.matchNumber += 1
				self.resetMatch = 0

				aux = self.player1
				self.player1 = self.player2
				self.player2 = self.player1

	def sendResult(self, data, result, playerID):
		if result == 0:
			if self.match.currPlayer == 1:
				self.match.currPlayer = 2
				#(1,2,3) 1 message, 2,3 coord
				self.player1.send(bytes('%d%s'%(playerID, data), encoding='utf-8'))
				self.player2.send(bytes('%d%s'%(playerID, data), encoding='utf-8'))
			else:
				self.match.currPlayer = 1
				self.player1.send(bytes('%d%s'%(playerID, data), encoding='utf-8'))
				self.player2.send(bytes('%d%s'%(playerID, data), encoding='utf-8'))
			
			return None
		
		if result == 1:
			self.player1.send(bytes("%d44"%(ttt.P1_WON), encoding='utf-8'))
			self.player2.send(bytes("%d%s"%(ttt.P1_WON, data), encoding='utf-8'))
		elif result == 2:
			self.player1.send(bytes("%d%s"%(ttt.P2_WON, data), encoding='utf-8'))
			self.player2.send(bytes("%d44"%(ttt.P2_WON), encoding='utf-8'))
		else:
			if playerID == 1:
				self.player1.send(bytes("%d44"%(ttt.DRAW), encoding='utf-8'))
				self.player2.send(bytes("%d%s"%(ttt.DRAW, data), encoding='utf-8'))
			else:
				self.player1.send(bytes("%d%s"%(ttt.DRAW, data), encoding='utf-8'))
				self.player2.send(bytes("%d44"%(ttt.DRAW), encoding='utf-8'))

	def manageClient(self, connection, playerID):
		"""
			A handler used by a new thread created due to a new connection between server and client.

			@Parameters:
				connection: client's connection object
				playerID: Player's number
		"""
		
		while True:
			# if playerID == 1:
			# 	connection = self.player1
			# else:
			# 	connection = self.player2

			# Decide which player the server will listen to
			data = connection.recv(self.dataSize).decode('utf-8')
			
			# if len(data) > 1 and int(data[0]) == ttt.PLAY_AGAIN:
			# 	self.resetMatch += 1 # Increase the number of players who accepted playing again

			# 	if(playerID == 1):
			# 		self.player2.send(bytes('%d00'%(ttt.PLAY_AGAIN), encoding='utf-8'))
			# 	else:
			# 		self.player1.send(bytes('%d00'%(ttt.PLAY_AGAIN), encoding='utf-8'))
			
			# elif len(data) > 1 and int(data[0]) == ttt.EXIT_GAME:
			# 	if(playerID == 1):
			# 		self.player2.send(bytes('%d00'%(ttt.EXIT_GAME), encoding='utf-8'))
			# 	else:
			# 		self.player1.send(bytes('%d00'%(ttt.EXIT_GAME), encoding='utf-8'))

			# 	connection.close()

			if self.match.currPlayer == playerID:
				if not data:
					connection.close()
					break

				x,y = int(data[0]), int(data[1]) # Get the play
				self.match.game.board[x][y] = self.match.currPlayer # Fill the board with the play
				
				# Check if there is a winner or a draw
				result = self.match.game.winner()
				self.sendResult(data, result, playerID)

	def run(self):
		while True:
			connection, _ = self.sock.accept()

			# Add the new player's connection to the list of players
			if self.player1 == None:
				print('Player 1 connected.')
				
				self.player1 = connection
				createThread(self.manageClient, (connection, 1))
			else:
				print('Player 2 connected.')
				
				self.player2 = connection
				createThread(self.manageClient, (connection, 2))

				# The first player starts playing
				self.match.currPlayer = 1
				self.resetMatch = 0
				
				# Start a new match
				self.match.startMatch(self.player1, self.player2)
				print('Match has started!')
				
				cThread = threading.Thread(target=self.manageMatch)
				cThread.daemon = True # Program becomes able to exit even if many thread are being executed
				cThread.start()

def main():
	port = 65001
	nPlayers = 2
	dataSize = 2048

	# Create a new server
	S = Server(port, nPlayers, dataSize)
	S.run()

main()