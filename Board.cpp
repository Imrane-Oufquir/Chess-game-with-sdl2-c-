#include "../include/Board.hpp"

	Board::Board(){
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("Chess_GAME" 	,SDL_WINDOWPOS_UNDEFINED
												,SDL_WINDOWPOS_UNDEFINED
												,600,600
												,SDL_WINDOW_SHOWN);	

		render = SDL_CreateRenderer(window,-1,0);
	}
	

	int * Board::getPiece(int indexPiece){

		int *pivot 	= (int*)malloc(sizeof(int)*2);
		pivot[0] 	= abs(indexPiece)-1;
		pivot[1]	= (indexPiece > 0)?0:1;

		return pivot;
	}

	void Board::drawCurrentBoard(){
		
		
		board = NULL;    
		while(board == NULL)
			board = IMG_Load("./image/board.png");

		texture = SDL_CreateTextureFromSurface(render,board);
		SDL_RenderCopy(render,texture,NULL,NULL);

		int *pivot = (int*)malloc(sizeof(int)*2);

		for(int i=0 ; i<8 ; i++){
			for(int j=0 ; j<8 ; j++){
				if(currentBoard[i][j] != 0){

					rectPiece.x = 33 + j*67;
					rectPiece.y = 33 + i*67;
					pivot = getPiece(currentBoard[i][j]);
					texturePiece = SDL_CreateTextureFromSurface(render,piece[pivot[0]][pivot[1]]);
					SDL_RenderCopy(render,texturePiece,NULL,&rectPiece);
				}
			}
		}
		SDL_RenderPresent(render);

	}

	void Board::Move(){

		bool continuer = 1;
		SDL_Event event;

		drawCurrentBoard();

		while(continuer){
			SDL_WaitEvent(&event);
			switch(event.type){
				case SDL_QUIT:
					continuer=0;
					break;

				case SDL_MOUSEBUTTONDOWN:
					int x_from, y_from,x_to,y_to;
					bool wait = 1;
					x_from = (event.motion.y-35)/67;
					y_from = (event.motion.x-35)/67;

					selectSquare(x_from, y_from, 1);

					if( currentBoard[x_from][y_from] !=0 ){
						printf(" From : %d  %d\n",x_from,y_from);
						while(wait){
							SDL_WaitEvent(&event);
							if(event.type == SDL_MOUSEBUTTONDOWN){
								x_to=(event.motion.y-35)/67;
								y_to=(event.motion.x-35)/67;
								printf(" To : %d  %d\n",x_to,y_to);

								if(isMovmentPossible(x_from,y_from,x_to,y_to)){
									currentBoard[x_to][y_to] = currentBoard[x_from][y_from];
									currentBoard[x_from][y_from] = 0;
									print();
									drawCurrentBoard();
									selectSquare(x_to, y_to, 1);
									selectSquare(x_from, y_from, 2);
									printf("\n\n");
								}
								else{
									std::cout<<"Movment Impossible"<<std::endl;
									drawCurrentBoard();
								}
								wait = 0;
							}
							else if(event.type == SDL_QUIT){
								wait = 0;
								continuer = 0;
							}
						}
					}

					break;
			}
		}

	}

	bool Board::isMovmentPossible(int xFrom, int yFrom, int xTo, int yTo){

		if(abs(currentBoard[xFrom][yFrom]) == 1){
			if (abs(xFrom - xTo) == 2 && yFrom == yTo){
				int sgn = (xFrom-xTo>0)?1:-1;
				if((currentBoard[xTo+sgn][yTo]==0) && (currentBoard[xTo][yTo]==0))
					if( (xFrom==6 && currentBoard[xFrom][yFrom]>0) || (xFrom==1 && currentBoard[xFrom][yFrom]<0))
						return true;
			}
			else if(abs(xFrom - xTo) == 1){	
				if ( yFrom == yTo  && currentBoard[xTo][yTo] == 0){
					if( (xFrom>xTo && currentBoard[xFrom][yFrom]>0) || (xFrom<xTo && currentBoard[xFrom][yFrom]<0) ){
						if(xTo == 0 && currentBoard[xFrom][yFrom]>0)
							currentBoard[xFrom][yFrom] = 5;
						else if(xTo == 7 && currentBoard[xFrom][yFrom]<0)
							currentBoard[xFrom][yFrom] = -5;
						return true;
					}
				}
				else if(abs(yFrom - yTo)==1 && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] < 0){
					if( (xFrom>xTo && currentBoard[xFrom][yFrom]>0) || (xFrom<xTo && currentBoard[xFrom][yFrom]<0) ){
						if(xTo == 0 && currentBoard[xFrom][yFrom]>0)
							currentBoard[xFrom][yFrom] = 5;
						else if(xTo == 7 && currentBoard[xFrom][yFrom]<0)
							currentBoard[xFrom][yFrom] = -5;
						return true;
					}
				}
			}
		}

		else if(abs(currentBoard[xFrom][yFrom]) == 2){
			if((xTo == xFrom) && (yTo != yFrom) && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0){
				int from = (yTo>yFrom)?yFrom:yTo, to = (yTo<yFrom)?yFrom:yTo;
				from++;
				while(from<to && currentBoard[xTo][from] == 0)
					from++;
				if(from == to)
					return true;
			}

			else if((xTo != xFrom) && (yTo == yFrom) && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0){
				int from = (xTo>xFrom)?xFrom:xTo, to   = (xTo<xFrom)?xFrom:xTo;
				from++;
				while(from<to && currentBoard[from][yTo] == 0)
					from++;
				if(from == to)
					return true;
			}
		}

		else if(abs(currentBoard[xFrom][yFrom]) == 3){
			if( abs(xFrom - xTo) == 2 && abs(yFrom - yTo) == 1 && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0)
				return true;
			else if( abs(xFrom - xTo) == 1 && abs(yFrom - yTo) == 2 && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0)
				return true;
		}

		else if (abs(currentBoard[xFrom][yFrom]) == 4){
			if((abs(xFrom - xTo) == abs(yFrom - yTo)) && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0){
				if((xFrom<xTo && yFrom<yTo) || (xFrom>xTo && yFrom>yTo)){
					int ifrom = (xTo>xFrom)?xFrom:xTo, jfrom = (yTo>yFrom)?yFrom:yTo;
					int ito   = (xTo<xFrom)?xFrom:xTo, jto   = (yTo<yFrom)?yFrom:yTo;
					ifrom++; jfrom++;
					while( ifrom<ito && jfrom<jto && currentBoard[ifrom][jfrom] == 0){
						ifrom++; jfrom++;
					}
					if(ifrom == ito && jfrom == jto)
						return true;
				}
				else if((xFrom>xTo && yFrom<yTo) || (xFrom<xTo && yFrom>yTo)){
					int ifrom = (xTo<xFrom)?xFrom:xTo, jfrom = (yTo>yFrom)?yFrom:yTo;
					int ito   = (xTo>xFrom)?xFrom:xTo, jto   = (yTo<yFrom)?yFrom:yTo;
					ifrom--; jfrom++;
					while( ifrom>ito && jfrom<jto && currentBoard[ifrom][jfrom] == 0){
						ifrom--; jfrom++;
					}
					if(ifrom == ito && jfrom == jto)
						return true;
				}
			}
		}

		else if (abs(currentBoard[xFrom][yFrom]) == 5){
			if((xTo == xFrom) && (yTo != yFrom) && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0){
				int from = (yTo>yFrom)?yFrom:yTo, to = (yTo<yFrom)?yFrom:yTo;
				from++;
				while(from<to && currentBoard[xTo][from] == 0)
					from++;
				if(from == to)
					return true;
			}

			else if((xTo != xFrom) && (yTo == yFrom) && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0){
				int from = (xTo>xFrom)?xFrom:xTo, to   = (xTo<xFrom)?xFrom:xTo;
				from++;
				while(from<to && currentBoard[from][yTo] == 0)
					from++;
				if(from == to)
					return true;
			}
			else if((abs(xFrom - xTo) == abs(yFrom - yTo)) && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0){
				if((xFrom<xTo && yFrom<yTo) || (xFrom>xTo && yFrom>yTo)){
					int ifrom = (xTo>xFrom)?xFrom:xTo, jfrom = (yTo>yFrom)?yFrom:yTo;
					int ito   = (xTo<xFrom)?xFrom:xTo, jto   = (yTo<yFrom)?yFrom:yTo;
					ifrom++; jfrom++;
					while( ifrom<ito && jfrom<jto && currentBoard[ifrom][jfrom] == 0){
						ifrom++; jfrom++;
					}
					if(ifrom == ito && jfrom == jto)
						return true;
				}
				else if((xFrom>xTo && yFrom<yTo) || (xFrom<xTo && yFrom>yTo)){
					int ifrom = (xTo<xFrom)?xFrom:xTo, jfrom = (yTo>yFrom)?yFrom:yTo;
					int ito   = (xTo>xFrom)?xFrom:xTo, jto   = (yTo<yFrom)?yFrom:yTo;
					ifrom--; jfrom++;
					while( ifrom>ito && jfrom<jto && currentBoard[ifrom][jfrom] == 0){
						ifrom--; jfrom++;
					}
					if(ifrom == ito && jfrom == jto)
						return true;
				}
			}
		}

		else if (abs(currentBoard[xFrom][yFrom]) == 6){
			if((abs(xFrom-xTo) <= 1 && abs(yFrom-yTo) <= 1) && currentBoard[xTo][yTo]*currentBoard[xFrom][yFrom] <= 0)
				return true;
		}

		return false;
	}


	void Board::print(){
		for(int i=0; i<8 ; i++){
			for(int j=0 ; j<8 ; j++){
				printf("  %2d",currentBoard[i][j]);
			}
			printf("\n");
		}
	}

	void Board::selectSquare(int x_from, int y_from, int type){
		if(type == 1){
			int *pivot = (int*)malloc(sizeof(int)*2);
			if(currentBoard[x_from][y_from]!=0){
				rectPiece.x = 33 + y_from*67;
				rectPiece.y = 33 + x_from*67;
				select = NULL;    
				while(select == NULL)
					select = IMG_Load("./image/pieceSelected.png");

				texturePiece = SDL_CreateTextureFromSurface(render,select);
				SDL_RenderCopy(render,texturePiece,NULL,&rectPiece);

				pivot = getPiece(currentBoard[x_from][y_from]);
				texturePiece = SDL_CreateTextureFromSurface(render,piece[pivot[0]][pivot[1]]);
				SDL_RenderCopy(render,texturePiece,NULL,&rectPiece);
				SDL_RenderPresent(render);
			}
		}
		else if (type ==2){
			select = NULL;    
			while(select == NULL)
				select = IMG_Load("./image/pieceSelected_.png");
			
			rectPiece.x = 33 + y_from*67;
			rectPiece.y = 33 + x_from*67;

			texturePiece = SDL_CreateTextureFromSurface(render,select);
			SDL_RenderCopy(render,texturePiece,NULL,&rectPiece);
			SDL_RenderPresent(render);
		}
	}

