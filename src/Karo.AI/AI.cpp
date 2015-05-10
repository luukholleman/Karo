#include "stdafx.h"
#include "AI.h"

namespace Karo {
	namespace AI {
		AI::AI()
		{
			BoardState = gcnew Karo::Core::Karo();
		}

		Karo::Common::Move^ AI::ChooseBestMove(Karo::Core::Karo^ board, int depth, Karo::Core::Player player) {

			IEnumerable<Karo::Common::Move^>^ moves = board->GetAvailableMoves(player);

			// if we're not going any deeper, return best move
			if (depth <= 0)
			{
				Karo::Common::Move^ bestMove;
				int bestScore = -9001; // ITS OVER 9000!!!!!!!!!!!11111one1111

				for (int i = 0; i < System::Linq::Enumerable::Count(moves); i++)
				{
					int score = Evaluate(board->WithMoveApplied(System::Linq::Enumerable::ElementAt(moves, i), player), player);

					if (score > bestScore)
					{
						bestMove = System::Linq::Enumerable::ElementAt(moves, i);
						bestScore = score;
					}
				}

				return bestMove;
			}
			
			Karo::Common::Move^ bestMove;
			int bestScore = -9001; // ITS OVER 9000!!!!!!!!!!!11111one1111

			for (int i = 0; i < System::Linq::Enumerable::Count(moves); i++)
			{
				Core::Karo ^ innerBoard = board->WithMoveApplied(System::Linq::Enumerable::ElementAt(moves, i), player);

				Common::Move^ bestInnerMove = ChooseBestMove(innerBoard, --depth, Karo::Core::PlayerHelper::Opposite(player));

				int score = Evaluate(innerBoard->WithMoveApplied(bestInnerMove, player), player);

				if (score > bestScore)
				{
					bestMove = bestInnerMove;
					bestScore = score;
				}
			}

			return bestMove;
		}

		int AI::Evaluate(Karo::Core::Karo^ board, Karo::Core::Player player)
		{
			//array < Core::Piece^>^ _piece = board->Pieces;
			//for each(Core::Piece^ p in _piece) {
			//	if (p->Player == player) {
			//		/*for each(Tuple<int, int>^ tup in neighbours) {

			//		}*/
			//	}
			//}
			return rand->Next(0, 100);
		}

		void AI::DoMove(Karo::Common::Move^ previousMove, int timeLimit, Action<Karo::Common::Move^>^ done)
		{
			// add previous move
			if (previousMove != nullptr) BoardState = BoardState->WithMoveApplied(previousMove, Karo::Core::Player::Player2);
			// if you get a move to apply, it's always from player2 
			Karo::Common::Move^ newMove = ChooseBestMove(BoardState, timeLimit, Karo::Core::Player::Player1); 
			
			// apply our move 
			BoardState = BoardState->WithMoveApplied(newMove, Karo::Core::Player::Player1); 
			
			array<Karo::Common::Move^>^ moves = gcnew array< Karo::Common::Move^ > {newMove };
			
			done->Invoke(newMove);
		}
	}
}