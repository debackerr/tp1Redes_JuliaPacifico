#pragma once

#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>


int checksLocation( int x, int y, int mat[50][2]);

int addLocation( int x, int y, int mat[50][2]);

void removeLocation(int x, int y, int mat[50][2]);

int  queryLocation(int x, int y, int mat[50][2]);

void listLocations( char msg[], int mat[50][2]);

void handle( char buf[], int MAIN_DATA[50][2]);
