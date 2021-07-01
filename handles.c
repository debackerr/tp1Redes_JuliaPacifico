#include "handles.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <arpa/inet.h>
#define BUFSZ 500

//Handle functions:

int checksLocation( int x, int y, int mat[50][2]){
    //verifies if certain location already exists 
    int cont = 0;
    for( int i = 0; i < 50; i++){
        if( mat[i][0] == x){
            if( mat[i][1] == y){
                cont = 1;
                break; 
            }
        }
    }
    //returns '1' case the given localization already exisits
    //if it doesn't, the function returns '0'
    return cont;
}

int addLocation( int x, int y, int mat[50][2]){
    int row = -1; //unused value
    //adds a new location on data matriz
    for(int i = 0; i < 50; i++){
        for(int j=0; j < 2; j++){
            if(mat[i][j] < 0){ //searches for a 'clear' position on the matrix
                if( j == 0 ){
                    mat[i][j]= x; //saves x
                    row = i; break; // breaks loop after saving X as to fullfill only one line of the data matrix
                }
            }
        }
        if(row != -1){
            mat[row][1] = y; //saves y
            break;
        }
    }
    return row; //return the row number of the new position added  
}

void removeLocation(int x, int y, int mat[50][2]){
    int row = -1; //unused value
    for(int i = 0; i< 50; i++){
        for(int j=0; j<2;j++){
           if( j != 0 ){ //searches for the second column of the matrix
                if((mat[i][j]= y) && (mat[i][j-1] == x)){ //compare cordinates
                    mat[i][j] = -1; //clears y
                    row = i; //saves X row
                    break;
                }    
           }   
            
        }
        if(row != -1){ 
            mat[row][0] = -1; //deletes specified X  
            break;
        }
    }
      
}

int queryLocation(int x, int y, int mat[50][2]){
    //this function returns a vector with the coordinates of the nearest vaccination point 
    //calculated as the euclidian distance bewtween 2 points
    int cont = 0; //counter for the number of filled positions on the matrix
    //this is necessary once removeLocation() erases coordinates 
    for(int i = 0; i < 50; i++){
        if(mat[i][0] > 0){
            cont++;
        }
    }
    double distances[cont][2]; 
    // 2d array containg the calculated distance for each coordinate from given X and Y 
    // and it's position on tha data matrix
    cont = 0; 
    for(int row = 0; row < 50; row++){
        if(mat[row][0] > 0){
            distances[cont][0] = sqrt( pow( (mat[row][0]-x) , 2) + pow( (mat[row][1]-y) , 2) );
            distances[cont][1] = row;
            cont++;
        }
    }

    int nearestRow; //saves the the row on the data matrix of the nearest coordinate
    int aux;
    for(int i=1;i<cont;i++){   
            if( i == 1){
                if(distances[i-1][0] < distances[i][0]){
                    nearestRow = distances[i-1][1];
                    aux = i-1;
                }else{
                    nearestRow = distances[i][1];
                    aux = i;
                }
                
            }else{
                if(distances[i][0] < distances[aux][0]){
                    nearestRow = distances[i][1];
                    aux = i;
                }
            } 
    }
    return nearestRow;
}

void listLocations( char msg[], int mat[50][2]){
    char aux[10]; 
    for (int i = 0; i < 50; i++){
        if( mat[i][0] > 0){ //seraches for filled positions
            sprintf(aux,"%d %d\n", mat[i][0], mat[i][1]);
            strcat(msg, aux); //concatenate string positions
        }
    }
}

void handle( char buf[],int MAIN_DATA[50][2]){

    char msg[BUFSZ];
    strcpy(msg, buf); //creates a copy of the buffer for manipulation
    memset(buf,0, BUFSZ); //clears buffer
     
    //string declaration for each command known:
    const char add[] = "add"; 
    const char rm[] = "rm"; 
    const char kill[] = "kill"; 
    const char query[] = "querry"; 
    const char list[] = "list";

    //identifies the specified command
    if (strncmp(kill, msg,4) == 0){
        exit(EXIT_SUCCESS); //exit code and ends every connection
    } else if( strncmp(list,msg,4) == 0){
        listLocations(buf, MAIN_DATA); //writes a list of the known locations on the buffer
    }else{
        int X,Y; 
        //Gets coordiantes from the received message:
        char *Xstr = NULL; 
        char *Ystr = NULL; 
        char delim[] = " "; //using space as a delimiter

        Xstr = strtok(msg,delim);   //gets both coordinates from the message sent
        Xstr = strtok(NULL,delim);  //gets X
        X = atoi(Xstr);
        Ystr = strtok(NULL, delim); //gets Y
        Xstr = strtok(NULL, delim); //avoids memory trash when getting Y
        Y = atoi(Ystr);

        if( ((X >= 0) && (X<= 9999)) && ((Y >= 0) && (Y <= 9999)) ){
            
            //Proceeds with the comparions:
            if( strcmp(add, msg) == 0){
                int addvf = checksLocation(X,Y, MAIN_DATA);
                //returns 1 case the given pair already exists
                if( addvf == 1){
                    sprintf(buf, "<< %d %d already exists",X,Y);
                }else{
                    int added = addLocation(X, Y, MAIN_DATA); //adds a new coordinate
                    sprintf(buf, "<< %d %d added", MAIN_DATA[added][0], MAIN_DATA[added][1]);
                }
            }else if( strcmp(rm, msg) == 0){
                int remove = checksLocation(X,Y, MAIN_DATA);
                if( remove == 1){
                    removeLocation(X,Y,MAIN_DATA); //removes a given coordinate
                    sprintf(buf,"<< %d %d removed",X,Y);
                }else{
                    sprintf(buf, "<< %d %d does not exist", X,Y);
                }
            }else if( strcmp(query,msg)){
                int q = queryLocation(X,Y,MAIN_DATA);
                //returns the row of the matrix containg the nearest location to a given location
                printf("deu query");
                sprintf(buf, "<< %d %d ", MAIN_DATA[q][0], MAIN_DATA[q][1]);
            }else{
                sprintf(buf,"<< invalid operation");
                //unreconized operation
            }
        }else{
            printf(buf, "invalid values");
            //coordinates sent by the client has either below 0 or above 9999 values
        }   
    }
}
