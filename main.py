# Import necessary modules
from kaggle_environments import make
from Chessnut import Game
import random
from IPython.display import display, HTML

# Create the Kaggle chess environment
env = make('chess', debug=True)

# Define a simple random-move chess bot
def chess_bot(obs):
    """
    A simple bot that picks a random valid move.
    Args:
        obs: Observation from the Kaggle environment
    Returns:
        A random valid move in algebraic notation
    """
    game = Game(obs.board)  # Create a game instance from the FEN string
    moves = list(game.get_moves())  # Get a list of all valid moves
    return random.choice(moves)  # Choose a random move

# Run a match between two random-move bots

#here we have our chess bot function that we made

result = env.run([chess_bot, chess_bot])

# Display results


print("Agent exit status/reward/time left:")
for agent in result[-1]:
    print("\t", agent['status'], "/", agent['reward'], "/", agent['observation']['remainingOverageTime'])
print("\n")

# Render the match
#isnt pygame gay tho

env.render(mode="human", width=1000, height=1000)
