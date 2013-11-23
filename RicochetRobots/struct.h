//
//  struct.h
//  RicochetRobots
//
//  Created by Baptiste on 23/11/13.
//  Copyright (c) 2013 outadev. All rights reserved.
//

#ifndef RicochetRobots_struct_h
#define RicochetRobots_struct_h

//version du jeu
#define VERSION "0.1"

//touches de déplacement des robots
#define KEY_UP 'z'
#define KEY_LEFT 'q'
#define KEY_DOWN 's'
#define KEY_RIGHT 'd'

//couleurs standard ANSI
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"

#define ANSI_BG_COLOR_RED     "\x1b[41m"
#define ANSI_BG_COLOR_GREEN   "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW  "\x1b[43m"
#define ANSI_BG_COLOR_BLUE    "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA "\x1b[45m"

//couleurs des robots
#define ROBOT_RED 0
#define ROBOT_GREEN 1
#define ROBOT_BLUE 2
#define ROBOT_GREY 3

//directions de déplacement des robots
#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3

//taille d'un plateau de jeu
#define BOARD_SIZE 16

//nombre de grilles prédéfinies
#define BUILTIN_BOARDS_COUNT 2

//valeur des cases
#define CELL_EMPTY 'E'       //case vide
#define CELL_OBJECTIVE 'O'   //case objectif

#define CELL_WALL_LEFT 'L'   //mur à gauche
#define CELL_WALL_TOP 'U'    //mur en haut
#define CELL_WALL_RIGHT 'R'  //mur à droite
#define CELL_WALL_BOTTOM 'B' //mur en base

//structure pour les coordonnées des robots
typedef struct {
    int x;
    int y;
} Coords;

//structure pour décrire les joueurs
typedef struct {
    int robotColor;
    char username[15];
    int score;
    Coords position;
} Player;

//structure du plateau de jeu
typedef struct {
    char obstacles[BOARD_SIZE][BOARD_SIZE];
    Coords robotsPosition[4];
} GameBoard;

//état du jeu en cours
typedef struct {
    //nombre de tours
    int turnCount;
    
    //joueur courant
    Player* currentPlayer;
    Player* players;
    
    GameBoard* gameBoard;
    
    //position actuelle des robots
    char currRobotsPos[BOARD_SIZE][BOARD_SIZE];
} GameState;

#endif