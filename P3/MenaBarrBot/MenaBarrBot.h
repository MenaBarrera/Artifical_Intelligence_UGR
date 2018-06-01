/*
 * ManuPCBot.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class MenaBarrBot:Bot {
public:
	MenaBarrBot();
	~MenaBarrBot();

	int evaluoTablero(const GameState & st, const Player & j);
	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
	Move Minimax(const GameState &estado, int limite, const Player & j, int & valor );
	int podaAlfaBeta(int limit,const GameState &status, Move & mov, const Player & j,  int alpha, int beta, int contador);	
	int peso(const GameState &st);
	int peso1(const GameState &st);
	int peso3(const GameState &st);
	int pesoMaxmov(GameState);



	/*
		Minimax: vamos a representar en un arbol nuestros movs
		y los movs contrarios

		- evaluamos el arbol cuando es nuestro turno
			- si nos toca mover intentamos maximizar
			- si le toca mover al contrario intenta maximizar su jugada, es decir, minimizar nuestro beneficio
			  (aquel qu me joda menos)
	*/
};

#endif /* MANUPCBOT_H_ */
