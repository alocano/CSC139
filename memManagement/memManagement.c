/*****************************************************/
/*Name: Althaea Locano CSC 139-01 Memory Management  */
/*Date: 26 Apr 2026                                  */
/*Implements FIFO, Least Recently Used (LRU),        */
/*and Optimal Page Replacement (OPT) algorithms.     */
/*Test the following reference string:               */
/*2,6,9,2,4,2,1,7,3,0,5,2,1,2,9,5,7,3,8,5            */
/*0,6,3,0,2,6,3,5,2,4,1,3,0,6,1,4,2,3,5,7            */
/*3,1,4,2,5,4,1,3,5,2,0,1,1,0,2,3,4,5,0,1            */
/*4,2,1,7,9,8,3,5,2,6,8,1,0,7,2,4,1,3,5,8            */
/*0,1,2,3,4,4,3,2,1,0,0,1,2,3,4,4,3,2,1,0            */
/*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_PAGES 20
#define MAX_FRAMES 3 //can start with 3 frames then increase to 5 and 10
