/**
Réalisé par : LAHLOU Khadija et GIANNUZZI Anaïs
Fichié : morpion.c
Résumé : jeu du morpion de taille WIDTH
Date de création : 11 février 2020
Date de dernière modification : 14 février 2020
**/

//#include <SDL/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//							choix de la taille
#define WIDTH 15
#define PLAYER 1
#define AI 0

struct state{
    char grid[WIDTH][WIDTH];
    int x,y;
    int turn;
    int depth;

    struct state *next;
};

typedef struct state state;
 
void print_grid(char grid[WIDTH][WIDTH]){
    
    int i,j;

    for( i=1; i < WIDTH ; i++){
        for( j=1; j < WIDTH ; j++){
            if( j == WIDTH - 1)
                printf("%c",grid[i][j]);
            else 
                printf("%c",grid[i][j]);
        }
        printf("\n");
        //for( j=1; j < 2*WIDTH - 2 ; j++)
            //printf("-");
        //printf("\n");
    }
}

int valid_move(int x, int y, char grid[WIDTH][WIDTH]){

    return x > 0 && x < WIDTH && y > 0 && y < WIDTH && grid[x][y] == ' ';

}

state * add_to_list(state *new,state *head){

    if( head == NULL){
        return new;
    }
    new->next=head;
    return new;

}

state * generate_children(state *s)
{
    int i,j;
    state *head,*new;

    head = NULL;
    for(i=1; i<WIDTH; i++){
        for(j=1; j<WIDTH; j++){
            if( s->grid[i][j] == ' '){
                new = malloc(sizeof(state));
                new->x = i;
                new->y = j;
                new->turn = s->turn==PLAYER? AI : PLAYER;
                new->depth = s->depth + 1;

                memcpy(new->grid, s->grid,WIDTH*WIDTH*sizeof(char));
                new->grid[i][j] = s->turn == PLAYER ? '1' : '0';

                head = add_to_list(new,head);
  //              printf("I found a move!!!\n");
				//free(head);
            }
        }
    }
/*
    for( new = head; new!=NULL ; new = new->next){
        print_grid(new->grid);
        printf("\n\n");
    } 
*/
    return head;
}

int win(int x, int y, char grid[WIDTH][WIDTH])
{
    int i,j;
    int same_column, same_row, same_main_diagonal, same_sec_diagonal;

    same_column = same_row = same_main_diagonal = same_sec_diagonal = 1;


//							check column
    for( i=1; i < WIDTH ; i++){
        if( grid[i][y] != grid[x][y]){
            same_column = 0;
            break;
        }
    }

//							check row
    for( j=1; j < WIDTH ; j++){
        if( grid[x][j] != grid[x][y]){
            same_row = 0;
            break;
        }
    }

//							check main diagonal
    if( x==y ){
        for( i=1 ; i< WIDTH ; i++){
            if( grid[i][i] != grid[x][y]){
                same_main_diagonal = 0;
                break;
            }
        }
    }else{
        same_main_diagonal = 0;
    }
    
//							check secondary diagonal
    if( x+y == WIDTH ){
        for( i=1; i<WIDTH ; i++){
            if( grid[i][WIDTH-i] != grid[x][y]){
                same_sec_diagonal = 0;
                break;
            }
        }
    }else{
        same_sec_diagonal = 0;
    }

    return same_column || same_row || same_main_diagonal || same_sec_diagonal;
}

int minimax(state *s,int *movex,int *movey)
{

    if( win(s->x, s->y, s->grid) ){
        if( s->turn == AI)
            return -1;
        else
            return 1;
    }
    

//							produce all the childs
//							I could avoid doing that and produce them one by one and check the value
    
    state *head,*temp,*bestMove;

    head = generate_children(s);
    if(head == NULL){
	//printf("No moves found.Draw.\n");
        return 0;
    }
    int score = s->turn == PLAYER? 2 : -2;
    int newScore;
	//while(temp!=NULL){
       temp = head;
		//for(temp = head ; temp!=NULL ; temp = temp->next){
        newScore = minimax(temp,NULL,NULL);
//							max layer
        if( s->turn == AI){
            if( newScore > score){
                score = newScore;
//							record the move only in the upper level
                if( s->depth == 0){
                    *movex = temp->x;
                    *movey = temp->y;
                }
            }
//							min layer
        }else{
            if(newScore < score){
                bestMove = temp;
                score = newScore;
            }

        } temp = temp -> next;
    //}
    //}

    return score;

}

void computer_move(int *x, int *y, char grid[WIDTH][WIDTH])
{
    state *s = malloc(1*sizeof(state));
    memcpy(s->grid,grid,WIDTH*WIDTH*sizeof(char));
    s->x = *x;
    s->y = *y;
    s->turn = AI;
    s->depth = 0;
    
    printf("miniman:%d\n",minimax(s,x,y));

    printf("\nComputer move is %d %d\n",*x,*y); 

    grid[*x][*y]='1';
	free(s);
    return ;
}

void player_move(int *x, int *y, char grid[WIDTH][WIDTH])
{
    int c;

    print_grid(grid);
    do{
        printf("Make a move.\n");
        scanf("%d %d",x,y);
//							clear input
        while ( (c = getchar()) != '\n' && c != EOF ) ;
    }while( !valid_move(*x,*y,grid) );
    grid[*x][*y] = '0';

    return ;
}
int verifier_position(int plateau[WIDTH-1][WIDTH-1],int ligne,int colonne){
// 							retourne 1 si la position n'est pas possible 0 sinon
  if ((ligne<WIDTH-1)&&(ligne>=0)){
    if ((colonne<WIDTH-1)&&(colonne>=0))
      if (plateau[ligne][colonne]==0)
	return(1);   
	  } 
  return(0);
}

void jouer(int plateau[WIDTH-1][WIDTH-1], int joueur){
//							place le pion sur la grille
  int posx,posy;
  int result=2;
  
  while(result!=1){
    if(result==0)
      printf("\nmauvaise position...\n");
    printf("Entrer la colonne :");
    scanf("%d",&posx);
    printf("Entrer la ligne :");
    scanf("%d",&posy);
    posy--;posx--;
    
    result=verifier_position(plateau,posx,posy);
  }
  plateau[posx][posy]=joueur;
}

void affiche_plateau(int plateau[WIDTH-1][WIDTH-1]){
  int i=0;
   int j=0;
  while(j<WIDTH-1){
    if(i<WIDTH-1){
      switch(plateau[i][j]){
      case 0:
	printf("-1");
        break;
      case 1:
        printf("0");
        break;
      case 2:
        printf("1");
        break;
      }
      i++;
    }else{
      i=0;
      j++;
      printf("\n");
    }
  }
}    
 

void vider_plateau(int plateau[WIDTH-1][WIDTH-1]){
  int i,j;
  for(j=0;j<WIDTH-1;j++)
    for(i=0;i<WIDTH-1;i++)
      plateau[i][j]=0;
}

int gagnant(int plateau[WIDTH-1][WIDTH-1],int joueur){
//							prend en paramètre le jeu de type morpion, un indice de ligne i et un indice de colonne j du jeu et le joueur.
//							Cette fonction renverra 1 si le joueur a gagné, sinon 0 et -1 si la grille est pleine.
  int i,j=0;
  int nbcase=0;
  for(i=0;i<WIDTH-1;i++)
    if(plateau[i][j]==joueur)
      if((plateau[i][j+1]==plateau[i][j+2])&&(plateau[i][j+1]==joueur))
	return(1);
 i=0;j=0;
  for(j=0;j<WIDTH-1;j++)
    if(plateau[i][j]==joueur)
    if((plateau[i+1][j]==plateau[i+2][j])&&(plateau[i+1][j]==joueur))
	return(1);    

  i=0;j=0;

    if(plateau[i][j]==joueur)
    if((plateau[i+1][j+1]==plateau[i+2][j+2])&&(plateau[i+1][j+1]==joueur))
	return(1);
 
	i=2;j=0;
   
    if(plateau[i][j]==joueur)
    if((plateau[i-1][j+1]==plateau[i-2][j+2])&&(plateau[i-2][j+2]==joueur))
	return(1);

	i=0;j=0;
    
	for(j=0;j<WIDTH-1;j++)
	 for(i=0;i<WIDTH-1;i++)
	  if (plateau[i][j]!=0)
	    nbcase++;
   
	if(nbcase==9)
          return(-1);

	return(0);
}

int main(void){

  //int T=0;
  
  /*printf("Quelle taille de plateau voulez-vous ? : ");
  scanf("%d", &T);
  printf("\n");
  printf("Ok pour la taille %d ! \n\n", T);*/
   
  int choixMenu;
  
  printf("=== Menu ===\n\n");
  printf("1. JvsJ\n");
  printf("2. JvsORDI\n");
  printf("3. Sortir\n");
  printf("4. Règle\n");
  
  printf("\nVotre choix ? ");
  scanf("%d", &choixMenu);
  
  printf("\n");
  
  switch (choixMenu)
  {
    case 1:
		printf("Vous avez choisi le mode J vs J !");
		int joueur,resultat;
		int plateau[WIDTH-1][WIDTH-1];
		int fin=0;
		
		//int WIDTH-1=4;

		vider_plateau(plateau);
  
		printf("Bienvenue dans Tic Tac Toe...\n\n");
  
		joueur=1;

		while(fin!=1){
			printf("c'est au joueur %d de jouer...\n",joueur);
			jouer(plateau,joueur);
			affiche_plateau(plateau);
			resultat=gagnant(plateau,joueur);
    
			if(resultat==1){
				fin=1;
				printf("\nLe joueur %d a gagner...\n",joueur);
			}
			if (resultat==2){
				fin=1;
				printf("\nMatch nul...\n");
			}
				joueur=joueur%2+1;
		}
		getchar();
		return(0);	
		break;
		case 2:
			printf("En cours\n");
			char grid[WIDTH][WIDTH];
			int i, j, turn;
			int moves, x, y;

			for( i=0; i < WIDTH ; i++){
				for( j=0; j < WIDTH ; j++){
					grid[i][j] = ' ';
				}
			}
    
		moves=0;
		turn = PLAYER;
		x=y=0;

		while(moves != (WIDTH-1)*(WIDTH-1)){
			if( turn == PLAYER){
				player_move(&x,&y,grid);
				turn = AI;
			}else{
				computer_move(&x,&y,grid);
				turn = PLAYER;
			}
			if( win(x,y,grid) )
				break;
				moves++;
		}

		if( moves == (WIDTH-1)*(WIDTH-1) )
			printf("It's a draw.\n");
		else{
			if( turn == AI)//the player played last
				printf("The player won.\n");
			else
				printf("The computer won.\n");
		}
		print_grid(grid);

		return 0;
			break;
		case 3:
			break;
		case 4:
			printf("Un premier joueur dessine son symbole sur une case. Puis c'est au tour de l'autre joueur");
			printf("de dessiner son symbole sur une case vide. Le but du jeu est de réussir à aligner ses trois sym");
			printf("boles, on remporte alors la partie. Pour corser le jeu, on peut aussi jouer sur des grilles plus gra");
			printf("ndes : 4x4 cases, il faudra donc aligner 4 symboles. 5x5, aligner 5 symboles.\n");
			break;
		default:
			printf("Vous n'avez pas rentre un nombre correct. Vous ne jouer pas !");
       break;
	}
	return 0;
}

