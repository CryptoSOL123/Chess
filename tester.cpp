// This will be the C++ file that the engine will be stored in!
// We will be using a traditional search algorithm called Minimax
//          I have already tested MCTS algorithm but there is too much RAM and computtion powre needed for that algorithm


// This will be the C++ engine that will be called from the main.py
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <vector>
#include "chess.hpp"   // Will be using Disservin's Chess.hpp
TYPE YES: Fanum 
PLEASE TRY AGAIN TYPE YES: Tax
PLEASE TRY AGAIN TYPE YES: Ohio
PLEASE TRY AGAIN TYPE YES:
// Now we will use a class for the engine
using namespace chess;

class ChessEngine {
public:
    ChessEngine(int depth) : maxDepth(depth) {}
    
    // EVALUATES THE PROVIDED FEN STRING
            // STOPS AFTER THE FIRST SPLIT OF THE STRING BECAUSE I DONT CARE ABOUT CASTLING
    int evaluate(const std::string& fen, bool isMaximizingPlayer) {
        static constexpr int pieceValues[12] = {
            100, 320, 330, 510, 880, 20000,  // White pieces
           -100, -320, -330, -510, -880, -20000 // Black pieces
        };
        
        int materialScore = 0;

        for (size_t i = 0; i < fen.size(); ++i) {
            char ch = fen[i];
            if (std::isspace(ch)) break; // Stop processing pieces

            if (std::isdigit(ch)) continue; // Empty squares

            int pieceValueIndex;
            if (std::isupper(ch)) {
                pieceValueIndex = ch - 'P';
            } else {
                pieceValueIndex = ch - 'p' + 6;
            }

            if (pieceValueIndex >= 0 && pieceValueIndex < 12) {
                materialScore += pieceValues[pieceValueIndex];
            }
        }
       // std::cout<<materialScore<<std::endl;
        return materialScore;
    }
    // GLORIOUS MINIMAX WITH ALPHA_BETA 
    int minimax(Board& board, int depth, bool isMaximizingPlayer, int alpha, int beta) {
        if (depth == 0) {
            return evaluate(board.getFen(), isMaximizingPlayer);
        }

        Movelist moves;
        movegen::legalmoves(moves, board);

        if (isMaximizingPlayer) {
            int maxEval = std::numeric_limits<int>::min();
            for (const auto& move : moves) {
                Board nextBoard(board);
                nextBoard.makeMove(move);
                int eval = minimax(nextBoard, depth - 1, false, alpha, beta);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (const auto& move : moves) {
                Board nextBoard(board);
                nextBoard.makeMove(move);
                int eval = minimax(nextBoard, depth - 1, true, alpha, beta);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return minEval;
        }
    }

    std::string getBestMove(const std::string& fen, bool isMaximizingPlayer) {
        Board board(fen);
        Movelist moves;
        movegen::legalmoves(moves, board);

        int bestEval = isMaximizingPlayer ? std::numeric_limits<int>::min()
                                          : std::numeric_limits<int>::max();

        std::string bestMove;

        for (const auto& move : moves) {
            Board nextBoard(board);
            nextBoard.makeMove(move);
            int eval = minimax(nextBoard, maxDepth - 1, !isMaximizingPlayer, 
                               std::numeric_limits<int>::min(), 
                               std::numeric_limits<int>::max());

            if (isMaximizingPlayer) {
                if (eval > bestEval) {
                    bestEval = eval;
                    bestMove = uci::moveToUci(move);
                }
            } else {
                if (eval < bestEval) {
                    bestEval = eval;
                    bestMove = uci::moveToUci(move);
                }
            }
        }

        return bestMove;
    }

private:
    int maxDepth;
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./a.out <FEN> <isWhite>" << std::endl;
        return 1;
    }

    std::string fen = argv[1];
    bool isWhite = std::string(argv[2]) == "1";

    ChessEngine engine(3);
    std::cout << engine.getBestMove(fen, isWhite) << std::endl;

    return 0;
}
