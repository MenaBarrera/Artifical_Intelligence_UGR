/*
 * MenaBarrBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: MenaBarr
 */

#include "MenaBarrBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <limits>

using namespace std;

MenaBarrBot::MenaBarrBot() {
	// Inicializar las variables necesarias para ejecutar la partida
	srand(time(0));

}

MenaBarrBot::~MenaBarrBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void MenaBarrBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string MenaBarrBot::getName() {
	return "MenaBarrBot"; // Sustituir por el nombre del bot
}



string mostrarJugador(const Player & p) {
	if(p == J1) {
		return "J1";
	}
	else {
		return "J2";
	}
}


int MenaBarrBot::evaluoTablero(const GameState &st, const Player & jug){
	int misSemillas , semillasContrario;
	Player contrario = J1;

	if(jug == J1)
		contrario = J2;
	
	misSemillas = semillasContrario = 0;

	for(int i=0; i<=6; i++){
		misSemillas += st.getSeedsAt(jug,(Position)i);
		semillasContrario += st.getSeedsAt(contrario, (Position)i);
	}
	return misSemillas -  semillasContrario;
}

int MenaBarrBot::peso3(const GameState& st){
	int h1,h2 = 0;
	int misSemillas , semillasContrario;
	Player contrario = J1;
	Player yo = this->getPlayer();


	if(yo == J1)
		contrario = J2;
	
	misSemillas = semillasContrario = 0;

	for(int i=0; i<=6; i++){
		misSemillas += st.getSeedsAt(yo,(Position)i);
		semillasContrario += st.getSeedsAt(contrario, (Position)i);
	}

	misSemillas*=10;
	semillasContrario*=10;
	
	if(st.getWinner()==yo){
		misSemillas+=500;
	}
	if(st.getWinner()==contrario){
		semillasContrario+=500;
	}
	
	h2 = misSemillas - semillasContrario;

	return h2;
}

int MenaBarrBot::peso1(const GameState &st){
	/*
		H4: Maximize the amount of counters in a players own store.
	 	This heuristic aims to pick a move that will maximize the amount of counters captured. 
	 	It has a look ahead of one.
	 */

	int misSemillas , semillasContrario;
	Player yo = (*this).getPlayer();
	Player contrario = J1;

	if(this->getPlayer() == J1)
		contrario = J2;
	
	misSemillas = semillasContrario = 0;

	misSemillas = st.getScore(yo);
	semillasContrario = st.getScore(contrario);

	return misSemillas - semillasContrario;

}


int MenaBarrBot::peso(const GameState &st){
	Player player = this->getPlayer();
	int score = st.getScore(player);
	int opponent_score;

	if (player == Player::J1)
		opponent_score = st.getScore(Player::J2);
	else
		opponent_score = st.getScore(Player::J1);

	int opponent_win_short_by = 24 - opponent_score;

	int value = score - opponent_score + opponent_win_short_by;

	return value;
}

string desplazar(int nivel) {
	string r;
	for(int i = 0; i < nivel; i++) {
		r.push_back(' ');
		r.push_back(' ');
		r.push_back(' ');
		r.push_back(' ');
	}
	return r;
}

int MenaBarrBot::podaAlfaBeta(const GameState &estado, int limite, const Player & j, Move & mov, int alpha, int beta, int contador){

	int valor;
	Move aux;

	// cerr << desplazar(2-limite) << "A&B (" << contador << ") = " << mostrarJugador(j) << ", " << mostrarJugador(getPlayer()) << "(getPlayer)" << endl;

	if ( (limite > 0 || limite <= -1) && !estado.isFinalState()){

		if(j == getPlayer() ){   // estamos en un nodo max
			// cerr << desplazar(2-limite) << "MAX || >>" << endl;
			for(int i = 1; i < 7; i++){
				// cerr << desplazar(2-limite) << " Posicion=" << i << " tiene en granero " << (int)estado.getSeedsAt(j,(Position)i) << endl;
				if(estado.getSeedsAt(j,(Position)i) > 0){
					// cerr << desplazar(2-limite) << " ACEPTADO" << endl;
					GameState sigEstado = estado.simulateMove((Move)i);
					valor = podaAlfaBeta(sigEstado,limite-1,sigEstado.getCurrentPlayer(),aux,alpha,beta, contador+1);

					if(alpha < valor){
						alpha = valor;
						mov = (Move)i;
						// cerr << desplazar(2-limite) << " |||| SELECCIONADO " << i << " |||" << endl;
					}
					
					if(beta <= alpha) {
						// cerr << desplazar(2-limite) << "MAX || << (PODADO) " << mov << endl;
						return beta;
					}
				}
			}
			// cerr << desplazar(2-limite) << "MAX || << " << mov << endl;
			return alpha;
		}
		else{
			// cerr << desplazar(2-limite) << "MIN || <<" << endl;
			for(int i = 1; i < 7; i++){
				// cerr << desplazar(2-limite) << " Posicion=" << i << " tiene en granero " << (int)estado.getSeedsAt(j,(Position)i) << endl;

					// cerr << desplazar(2-limite) << " ACEPTADO" << endl;
					GameState sigEstado = estado.simulateMove((Move)i);
					valor = podaAlfaBeta(sigEstado,limite-1,sigEstado.getCurrentPlayer(),aux,alpha,beta, contador+1);

					if(beta > valor){
						beta = valor;
						mov  = (Move)i;
					}
					if ( beta <= alpha){
						// cerr << desplazar(2-limite) << "MIN || << (PODADO) " << mov << endl;
						return alpha;
					}
				//}
			}
			// cerr << desplazar(2-limite) << "MIN || << " << mov << endl;
			return beta;
		}
	}
	else{
		//return evaluoTablero(estado,this->getPlayer());
		return peso3(estado);
	}
}


Move MenaBarrBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Player turno= state.getCurrentPlayer() ;
	//long timeout= this->getTimeOut();



	Move movimiento= M_NONE;

	int v;
	int a = numeric_limits<int>::min();
	int b = numeric_limits<int>::max();
	assert(getPlayer() == state.getCurrentPlayer());

	v = podaAlfaBeta(state,16,turno,movimiento,a,b, 0);


	// cerr << "MOV=" << movimiento << endl;
	assert(state.getSeedsAt(getPlayer(), (Position)movimiento) > 0);


	return movimiento;
	





}
/*
Move MenaBarrBot::Minimax(const GameState &estado, int limite, const Player & j, int & valor ){
	Move mov;


	if ( (limite > 0 || limite <= -1) && !estado.isFinalState()){
		int valorSigmov;
		Move sigMov;

		if(estado.getCurrentPlayer() == j){   // estamos en un nodo max
			valor = numeric_limits<int>::min();
			
			
			for(int i = 1; i < 7; i++){
				GameState sigEstado = estado.simulateMove((Move)i);
				sigMov = Minimax(sigEstado,limite-1,j,valorSigmov);
				
				if(valor < valorSigmov){
					// cerr << " eeeeee " <<endl;
					valor = valorSigmov;
					mov = (Move)i;
				}
			}
		}
		else{
			valor = numeric_limits<int>::max();
			
			for(int i = 1; i < 7; i++){
				GameState sigEstado = estado.simulateMove((Move)i);
				sigMov = Minimax(sigEstado,limite-1,j,valorSigmov);
				if(valor > valorSigmov){
					valor = valorSigmov;
					mov = (Move)i;
				}
			}
		}
	}
	else{
		valor = evaluoTablero(estado,j);
	}
	return mov;
}	

*/