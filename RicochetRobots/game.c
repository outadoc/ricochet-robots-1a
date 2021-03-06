//
//  game.c
//  RicochetRobots
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Baptiste Candellier. All rights reserved.
//
//  Fonctions relatives à la boucle globale
//

#include "game.h"

//
// Boucle principale du jeu.
// Affiche le menu principal et gère les options principales.
//
int start() {
    //boucle principale : on en sort avec un return
    while(1) {
        bool retry = false;

        //tant qu'on n'a pas choisi une option correcte du menu
        do {
            //on affiche les options du menu principal
            int choice = displayMainMenu();
            retry = false;

            switch (choice) {
                case -1:
                case 3:
                    //si on veut quitter le jeu
                    return 0;
                case 0:
                    if(startSinglePlayer() != 0) {
                        retry = true;
                    }
                    break;
                case 1: {
                    if(startMultiPlayer() != 0) {
                        retry = true;
                    }
                    break;
                }
                case 2:
                    displayLeaderboard();
                    break;
                default:
                    //si le choix est incorrect, on réessaye
                    retry = true;
                    break;
            }
        } while(retry);
    }
}

//
// Boucle de partie solo.
// Demande à l'utilisateur les infos nécessaires au lancement de la partie, puis exécute la boucle jusqu'à la fin du jeu.
//
int startSinglePlayer() {
    GameBoard board;
    
    //si askForGameBoard renvoie 0, on est prêts à continuer
    if(askForGameBoard(&board) == 0) {
        Player players[] = {{.score = 0}};
        Robot robots[ROBOTS_COUNT];
        
        //initialisation des robots
        initRobots(robots, &board);
        
        //on demande le pseudo du joueur
        askForSinglePlayerUsername(players);
        
        //instanciation du jeu
        GameState state = {
            .turnCount = 0,
            .playersCount = 1,
            .currentPlayer = &players[0],
            .players = players,
            .currentRobot = &robots[0],
            .robots = robots,
            .gameBoard = &board
        };
        
        //on met à jour l'affichage une première fois
        refreshGameDisplay(&state);
        
        //on fait disparaitre le curseur
        curs_set(0);
        
        //tant que tous les robots n'ont pas été sur leurs objectifs respectifs
        while(!wereAllRobotsOnTheirObjectives(state.robots, state.gameBoard)) {
            //on demande à l'utilisateur dans quelle direction il veut aller
            Direction direction = waitForDirection(&state);
            
            //si on a appuyé sur "echap" ou un truc du genre
            if(direction == -1) return 1;
            
            //on déplace le robot dans cette direction
            moveCurrentRobotWhilePossible(&state, direction);
            
            //on met à jour l'affichage après chaque tour
            refreshGameDisplay(&state);
        }
        
        //fin du jeu !
        //hop, on fait réapparaitre le curseur
        curs_set(1);
        
        //on enregistre le meilleur score
        Score bestScore;
        
        bestScore.score = state.currentPlayer->score;
        strcpy(bestScore.username, state.currentPlayer->username);
        strcpy(bestScore.boardName, state.gameBoard->name);
        
        registerScore(bestScore);
        
        //un joueur est arrivé sur l'objectif, fin du jeu
        displaySoloGameEnding(&state);
    } else {
        refresh();
        return 1;
    }
    
    return 0;
}

//
// Boucle de partie multijoueur.
// Demande aux utilisateurs les infos nécessaires au lancement de la partie, puis exécute la boucle jusqu'à la fin du jeu.
//
int startMultiPlayer() {
    GameBoard board;
    
    //si askForGameBoard renvoie 0, on est prêts à continuer
    if(askForGameBoard(&board) == 0) {
        int i;
        int playersCount = askForPlayersCount();
        
        Color usedColors[ROBOTS_COUNT]; //cet array va contenir les couleurs de robots pour lesquelles on a déjà joué
        Player players[MAX_PLAYERS_COUNT];
        Robot robots[ROBOTS_COUNT];
        
        //initialisation des robots
        initRobots(robots, &board);

        for(i = 0; i < playersCount; i++) {
            players[i].score = 0;
            players[i].victoryCount = 0;
        }
        
        askForPlayersInfo(players, playersCount);
        
        //instanciation du jeu
        GameState state = {
            .turnCount = 0,
            .playersCount = playersCount,
            .currentPlayer = &players[0],
            .players = players,
            .currentRobot = &robots[0],
            .robots = robots,
            .gameBoard = &board
        };
        
        //une partie se joue en 4 tours
        for(i = 0; i < ROBOTS_COUNT; i++) {
            int j;
            bool isInArray = false;
            
            //on tire une couleur de robot au hasard
            //mais on vérifie à chaque fois qu'elle n'ait pas déjà été tirée
            do {
                int k;
                
                isInArray = false;
                state.robotColorToMove = rand_between(0, ROBOTS_COUNT);
                
                for(k = 0; k < i; k++) {
                    printw("%d vs %d\n", state.robotColorToMove, usedColors[k]);refresh();
                    if(state.robotColorToMove == usedColors[k]) {
                        isInArray = true;
                    }
                }
            } while(isInArray);
            
            usedColors[i] = state.robotColorToMove;
            
            //on affiche le plateau pour que les joueurs puissent faire leurs prévisions
            clear();
            displayGameBoard(&state);
            
            //on leur demande leur prévision
            askForScoreGoals(&state);
            
            //on fait disparaitre le curseur
            curs_set(0);
            
            //on trie les joueurs par ordre croissant du nombre de coups prévus
            qsort(state.players, state.playersCount, sizeof(Player), (compfn) sortByGoal);
            
            for(j = 0; j < playersCount; j++) {
                bool playerWon = false;
                
                //c'est au tour du prochain joueur
                state.currentPlayer = &players[j];
                
                //on remet à zéro le plateau
                resetMap(&state);
                
                //on met à jour l'affichage après chaque tour
                refreshGameDisplay(&state);
                
                do {
                    //on demande à l'utilisateur dans quelle direction il veut aller
                    Direction direction = waitForDirection(&state);
                    
                    //si on a appuyé sur "echap" ou un truc du genre
                    if(direction == -1) return 1;
                    
                    //on déplace le robot dans cette direction
                    moveCurrentRobotWhilePossible(&state, direction);
                    
                    //on met à jour l'affichage après chaque tour
                    refreshGameDisplay(&state);
                    
                    if(isRobotOnObjective(&state.robots[state.robotColorToMove], state.gameBoard)) {
                        //yaaay, le joueur a gagné. on incrémente son compteur de victoires
                        state.players[j].victoryCount++;
                        playerWon = true;
                        break;
                    }
                } while(state.players[j].score < state.players[j].goal);
                
                //si le joueur a gagné, pas la peine de faire jouer les joueurs suivants
                if(playerWon) break;
            }
            
            //on remet à zéro le plateau
            resetMap(&state);
            
            //hop, on fait réapparaitre le curseur
            curs_set(1);
        }
        
        //fin du jeu
        //on trie les joueurs par score
        qsort(state.players, state.playersCount, sizeof(Player), (compfn) sortPlayersByScore);
        
        //on n'inscrit pas les parties multi dans le fichier des high-scores, il faudrait un système totalement différent.
        
        //on affiche l'écran de fin
        displayMultiGameEnding(&state);
    } else {
        refresh();
        return 1;
    }
    
    return 0;
}
