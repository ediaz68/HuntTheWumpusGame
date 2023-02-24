/*
	Prog 2: Wumpus (bats, arrow, and all arrays are dynamic)
	CS 211, UIC, Fall 2022
	System: Replit
	Author: Erik Diaz

	Hunt the Wumpus is a classic text-based adventure game by Gregory Yob from 1972.
	The Wumpus lives in a completely dark cave of 20 rooms.  Each room has 3 tunnels leading to other rooms.
	This program implements this game, without superbats or arrows, using a fixed size array.
*/

#include <stdio.h>
#include <stdbool.h>	// for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

// Room connection values are set as global constants, and don't need to be passed.
// The rooms array is indexed from 0..22, but we don't use the 0th entry and 
// only use 1..20.  This approach helps prevent off-by-one indexing errors.


// Used to more conveniently pass all game information between functions.
struct GameInfo 
{
	int moveNumber;  // Counts up from 1, incrementing for each move
	int personRoom;  // Room 1..20 the person currently is in
	int wumpusRoom;  // Room 1..20 the Wumpus is in
	int pitRoom1;    // Room 1..20 the first pit is in
	int pitRoom2;    // Room 1..20 the second pit is in
  int batRoom1;    // Room 1..20 the first bat  is in
  int batRoom2;    // Room 1..20 the second bat is in
  int arrowRoom;    // Room 1..20 the arrow is in
};

// Function prototype needed to allow calls in any order between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *personIsAlive, int **roomHelp);

//--------------------------------------------------------------------------------
void displayCave()
{
    printf( "\n"
			"       ______18______             \n"
    		"      /      |       \\           \n"
    		"     /      _9__      \\          \n"
    		"    /      /    \\      \\        \n"
    		"   /      /      \\      \\       \n"
    		"  17     8        10     19       \n"
    		"  | \\   / \\      /  \\   / |    \n"
    		"  |  \\ /   \\    /    \\ /  |    \n"
    		"  |   7     1---2     11  |       \n"
    		"  |   |    /     \\    |   |      \n"
    		"  |   6----5     3---12   |       \n"
    		"  |   |     \\   /     |   |      \n"
    		"  |   \\       4      /    |      \n"
    		"  |    \\      |     /     |      \n"
    		"  \\     15---14---13     /       \n"
    		"   \\   /            \\   /       \n"
    		"    \\ /              \\ /        \n"
    		"    16---------------20           \n"
    		"\n");
}


//--------------------------------------------------------------------------------
void displayInstructions()
{
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
        	"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
        	"2. Two other rooms have super-bats.  If you go there, the bats grab you and     \n"
        	"   fly you to some random room, which could be troublesome.  Then those bats go \n"
        	"   to a new room different from where they came from and from the other bats.   \n"
    		"3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    \n"
        	"   is too heavy for bats to lift.  Usually he is asleep.  Two things wake       \n"
        	"    him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   \n"
        	"    will move into the lowest-numbered adjacent room anytime you shoot an arrow.\n"
        	"    When you move into the Wumpus' room, then he wakes and moves if he is in an \n"
        	"    odd-numbered room, but stays still otherwise.  After that, if he is in your \n"
        	"    room, he snaps your neck and you die!                                       \n"
        	"                                                                                \n"
        	"Moves:                                                                          \n"
        	"On each move you can do the following, where input can be upper or lower-case:  \n"
        	"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
        	"   then a room number.                                                          \n"
        	"2. Shoot your guided arrow through a list of up to three adjacent rooms, which  \n"
        	"   you specify.  Your arrow ends up in the final room.                          \n"
        	"   To shoot enter 'S' followed by the number of rooms (1..3), and then the      \n"
        	"   list of the desired number (up to 3) of adjacent room numbers, separated     \n"
        	"   by spaces. If an arrow can't go a direction because there is no connecting   \n"
        	"   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      \n"
        	"   continues doing this until it has traveled the designated number of rooms.   \n"
        	"   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  \n"
        	"   automatically pick up the arrow if you go through a room with the arrow in   \n"
        	"   it.                                                                          \n"
        	"3. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
        	"4. Enter 'C' to cheat and display current board positions.                      \n"
        	"5. Enter 'D' to display this set of instructions.                               \n"
        	"6. Enter 'P' to print the maze room layout.                                     \n"
        	"7. Enter 'X' to exit the game.                                                  \n"
        	"                                                                                \n"
        	"Good luck!                                                                      \n"
        	" \n\n");
}//end displayInstructions()


//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue,      // New number we're checking
                 int *randomNumbers,  // Set of numbers previously found
                 int limit)            // How many numbers previously found
{
    int returnValue = false;
    
    // compare random value against all previously found values
    for( int i = 0; i < limit; i++) {
        if( randomValue == randomNumbers[i]) 
        {
            returnValue = true;   // It is already there
            break;
        }
    }
    
    return returnValue;
} 


//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int *randomNumbers,   // Array of random numbers
                     int size)              // Size of random numbers array
{
    int randomValue = -1;
    
    for( int i = 0; i < size; i++) 
    {
        do 
        {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
} 


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(struct GameInfo *gameInfo)   // All game settings variables
{
    // Array of 6 unique values 1..20, to be used in initializing cave hazards locations
    int *randomNumbers;
    randomNumbers = (int*)malloc(7 * sizeof(int));
    
    // Initialize cave room connections
    //       ______18______
    //      /      |       \
    //     /      _9__      \
    //    /      /    \      \
    //   /      /      \      \
    //  17     8        10    19
    // |  \   / \      /  \   / |
    // |   \ /   \    /    \ /  |
    // |    7     1---2     11  |
    // |    |    /     \    |   |
    // |    6----5     3---12   |
    // |    |     \   /     |   |
    // |    \       4      /    |
    // |     \      |     /     |
    //  \     15---14---13     /
    //   \   /            \   /
    //    \ /              \ /
    //    16---------------20
    
      
    // Select some unique random values 1..20 to be used for 2 bats rooms, 2
    // pits rooms, Wumpus room, and initial player room
    setUniqueValues(randomNumbers, 7);
    // Use the unique random numbers to set initial locations of hazards, which
    //    should be non-overlapping.

    gameInfo->personRoom = randomNumbers[0];
    gameInfo->wumpusRoom = randomNumbers[1]; 
    gameInfo->pitRoom1 = randomNumbers[2];
    gameInfo->pitRoom2 = randomNumbers[3];
    
    gameInfo->batRoom1 = randomNumbers[4];
    gameInfo->batRoom2 = randomNumbers[5];
    gameInfo->arrowRoom = randomNumbers[6];
    
    gameInfo->moveNumber = 1;

  //frees memory for randomNumbers array
  free(randomNumbers);
  randomNumbers = NULL;
  
}// end initializeBoard(...)


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int *tunnels,     // Array of adjacent tunnels
                    int nextRoom)        // Desired room to move to
{
    return( tunnels[0] == nextRoom ||
            tunnels[1] == nextRoom ||
            tunnels[2] == nextRoom
          );
}


//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(struct GameInfo gameInfo)
{

  printf( "Cheating! Game elements are in the following rooms: \n"
    		"Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
    		"%4d %5d %6d %5d %5d %5d %5d \n\n", 
	    gameInfo.personRoom,
			gameInfo.wumpusRoom,
			gameInfo.pitRoom1,
			gameInfo.pitRoom2,
      gameInfo.batRoom1,
      gameInfo.batRoom2,
      gameInfo.arrowRoom
		);
    
}// end displayCheatInfo(...)



//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo(struct GameInfo gameInfo,
                     int **passRooms)//Passes the dynamic array full of rooms from main
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.personRoom;
    printf("You are in room %d. ", currentRoom);
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = passRooms[currentRoom][0];
    int room2 = passRooms[currentRoom][1];
    int room3 = passRooms[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if(room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) 
    {
        printf("You smell a stench. ");
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if( room1 == pit1Room || room1 == pit2Room ||
        room2 == pit1Room || room2 == pit2Room ||
        room3 == pit1Room || room3 == pit2Room
      ) 
    {
        printf("You feel a draft. ");
    }


  int bat1Room = gameInfo.batRoom1;
  int bat2Room = gameInfo.batRoom2;

  //Display message if a bat is in an adjacent room
  if(room1 == bat1Room || room1 == bat2Room ||
        room2 == bat1Room || room2 == bat2Room ||
        room3 == bat1Room || room3 == bat2Room
      ) 
    {
        printf("You hear rustling of bat wings. ");
    }
    printf("\n\n");
}//end displayRoomInfo(...)


//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the person dies.
// If the person just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
void checkForHazards(
         struct GameInfo *gameInfo, // Hazards location and game info
         bool *personIsAlive, // Person is alive, but could die depending on the hazards
         int **passRooms) //Passes the dynamic array full of rooms from main
{
  // retrieve the room the person is in
  int personRoom = gameInfo->personRoom;
    
  // Check for the Wumpus
  if(personRoom == gameInfo->wumpusRoom) 
  {    
    if(gameInfo->wumpusRoom % 2 == 1) 
    {
      // You got lucky and the Wumpus moves away
      printf( "You hear a slithering sound, as the Wumpus slips away. \n"
					"Whew, that was close! \n");
      gameInfo->wumpusRoom = passRooms[personRoom][0];  // Choose the lowest-numbered adjacent room
    }
    else 
    {
      // Wumpus kills you
      printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
					"It is over.\n");
    *personIsAlive = false;
      return;
    }
  }

  // Check whether there is a pit
  if(personRoom == gameInfo->pitRoom1 || personRoom == gameInfo->pitRoom2) 
  {
    // Person falls into pit
    printf("Aaaaaaaaahhhhhh....   \n");
    printf("    You fall into a pit and die. \n");
    *personIsAlive = false;
    return;
  }

  //Checks if player enters room with either of the bats
  if(personRoom == gameInfo->batRoom1 || personRoom == gameInfo->batRoom2) 
    {
      //initialized to -1 because number cant be between 1-20
      int randomNewRoom = -1;

      //saves players initial room location
      int personInital = gameInfo->personRoom; 
      
      //Player gets assigned into a new random room
      gameInfo->personRoom = rand() % NUMBER_OF_ROOMS + 1; 
      
      printf("Woah... you're flying!\n");
      printf("You've just been transported by bats to room %d.\n", gameInfo->personRoom);

      //Loops till randomNewRoom does not equal the persons room or either of the bat rooms
      do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS + 1;
    } while (randomNewRoom == gameInfo->personRoom ||
             randomNewRoom == gameInfo->batRoom1  ||
             randomNewRoom == gameInfo->batRoom2 );

    //if the players initial room is equal to batRoom1 it
    //assigns batRoom1 into a new room 
    if(personInital == gameInfo->batRoom1) 
    {
        gameInfo->batRoom1 = randomNewRoom;
    }
    //If the players initial room does not equal batRoom1 then
    //batRoom2 gets the newRandomRoom
    else 
    {
      gameInfo->batRoom2 = randomNewRoom;
    }
      
    return;
    }
  
  //print message if player walks into room and the arrow is in it
  if(gameInfo->arrowRoom  == personRoom)
      {
        // Player picks up arrow
	      printf("Congratulations, you found the arrow and can once again shoot.\n");
        gameInfo->arrowRoom = -1;
        return;
      }
  
}//end checkForHazards(...)


//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the person's location,
// useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo)
{
    printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n");
	// In the scanf below note that we put the space at the beginning of the scanf so  
    // that any newline left over from a previous input is not read in and used as 
	// the next move. Another option is having getchar() after the scanf() statement.
	scanf(" %d %d %d %d %d %d %d", &theGameInfo->personRoom, &theGameInfo->wumpusRoom, 
                          &theGameInfo->pitRoom1, &theGameInfo->pitRoom2, &theGameInfo->batRoom1, &theGameInfo->batRoom2, &theGameInfo->arrowRoom);
    printf("\n");
}


//All functions related to shooting the arrow are here
void arrowFunctions(struct GameInfo *theGameInfo, // Hazards location and game info
                    bool *playerAlive, // Person is alive, but could die depending on the hazards
                    bool *wumpusAlive, // Wumpus is alive, but could die depending on the hazards
                    int **passRooms) //Passes the dynamic array full of rooms from main
{

  //Saves the number of rooms the player wants to shoot
  int shootCount;
  //Saves the room that the player wants to send the arrow
  int arrowInput1, arrowInput2, arrowInput3;
  
  // Prompt user to enter input
	printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
  scanf(" %d", &shootCount);

  //Saves the rooms based on how many rooms the player wants to shoot
  if(shootCount == 1)
  {
     scanf(" %d", &arrowInput1);
  }
  else if (shootCount == 2)
  {
    scanf(" %d %d", &arrowInput1, &arrowInput2);
  }
  else if(shootCount == 3)
  {
    scanf(" %d %d %d", &arrowInput1, &arrowInput2, &arrowInput3);  
  }

  //Put all the arrow locations into a dynamic array
  int *arrowInputArr = (int*) malloc(3 * sizeof(int));
  arrowInputArr[0] = arrowInput1;
  arrowInputArr[1] = arrowInput2;
  arrowInputArr[2] = arrowInput3;
  int i;

  // User enters more than 3 rooms to shoot arrow through
  if(shootCount > 3)
  {
	  printf("Sorry, the max number of rooms is 3.  Setting that value to 3.\n");
    shootCount = 3; // gets defaulted to 3
  }

  //arrowRoom equals the players room because they picked it up
  theGameInfo->arrowRoom =  theGameInfo->personRoom;

  //for loop to loop through how many rooms the user wants the arrow to go through
  for(i = 0; i < shootCount; i++)
  {
    //checks if the arrow input is adjacent to the room that the player is currently in
    if(roomIsAdjacent(passRooms[theGameInfo->arrowRoom], arrowInputArr[i]))
    {
      //changes the arrows room to the room the user inputted
      theGameInfo->arrowRoom = arrowInputArr[i];

      //checks if the wumpus is in the room
      if(theGameInfo->arrowRoom  == theGameInfo->wumpusRoom)
      {
        // Arrow killed Wumpus
	      printf("Wumpus has just been pierced by your deadly arrow! \n"
            "Congratulations on your victory, you awesome hunter you.\n");
        *wumpusAlive = false;
        return;
      }

      //checks if the player gets shot by their own arrow
      if(theGameInfo->arrowRoom == theGameInfo->personRoom)
      {
      	// Arrow killed player
	      printf( "You just shot yourself.  \n"
            "Maybe Darwin was right.  You're dead.\n");
        *playerAlive = false;
        return;
      }
    } //end if() for checking if the room is adjacent

    //else for if the room is not adjacent  
    else
    {
      // Attempt to shoot arrow through room that is not adjacent
	    printf("Room %d is not adjacent.  Arrow ricochets...\n", arrowInputArr[i]);

      //moves to lowest numbered adjoining room
      theGameInfo->arrowRoom = passRooms[theGameInfo->arrowRoom][0];

      //checks if the wumpus is in the room
      if(theGameInfo->arrowRoom  == theGameInfo->wumpusRoom)
      {
       	// Arrow ricochets, and kills Wumpus
        printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
            "Accidental victory, but still you win!\n");
        *wumpusAlive = false;
        return;
      }
      //checks if the player is in the room
      if(theGameInfo->arrowRoom == theGameInfo->personRoom)
      {
        // Arrow ricochets, and kills person
	      printf( "You just shot yourself, and are dying.\n"
            "It didn't take long, you're dead.\n");
        *playerAlive = false;
        return;
      }
    }  
  }// end for loop

  //moves wumpus to lowest adjoining room
  theGameInfo->wumpusRoom = passRooms[theGameInfo->wumpusRoom][0];
  
  //frees memory for arrowInputArr
  free(arrowInputArr);
  arrowInputArr = NULL;
}//ends arrowFunctions()


//--------------------------------------------------------------------------------
int main(void) 
{
    struct GameInfo gameInfo;   // Used to more easily pass game info variables around
    bool personIsAlive = true;  // Used in checking for end of game
    bool wumpusIsAlive = true;  // Used in checking for end of game
    char typeOfMove;            // Used to handle user input letter
    int nextRoom;               // User input of destination room number, used on a 'M' type move
    
    // Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);
  
  int i;
  //allocating room array
  int** rooms = malloc(sizeof(int*) * 21); //allocates the rows in the array
  for(i = 0; i < 21; i++)
    {
      //allocating the columns in the array
      rooms[i] = malloc(sizeof(int) * 3);
    }

  //Room is ignored
  rooms[0][0] = 0;
  rooms[0][1] = 0;
  rooms[0][2] = 0;

  //Room 1
  rooms[1][0] = 2;
  rooms[1][1] = 5;
  rooms[1][2] = 8;

  //Room 2
  rooms[2][0] = 1;
  rooms[2][1] = 3;
  rooms[2][2] = 10;

  //Room 3
  rooms[3][0] = 2;
  rooms[3][1] = 4;
  rooms[3][2] = 12;

  //Room 4
  rooms[4][0] = 3;
  rooms[4][1] = 5;
  rooms[4][2] = 14;

  //Room 5
  rooms[5][0] = 1;
  rooms[5][1] = 4;
  rooms[5][2] = 6;

  //Room 6
  rooms[6][0] = 5;
  rooms[6][1] = 7;
  rooms[6][2] = 15;

  //Room 7
  rooms[7][0] = 6;
  rooms[7][1] = 8;
  rooms[7][2] = 17;

  //Room 8
  rooms[8][0] = 1;
  rooms[8][1] = 7;
  rooms[8][2] = 9;

  //Room 9
  rooms[9][0] = 8;
  rooms[9][1] = 10;
  rooms[9][2] = 18;

  //Room 10
  rooms[10][0] = 2;
  rooms[10][1] = 9;
  rooms[10][2] = 11;

  //Room 11
  rooms[11][0] = 10;
  rooms[11][1] = 12;
  rooms[11][2] = 19;

  //Room 12
  rooms[12][0] = 3;
  rooms[12][1] = 11;
  rooms[12][2] = 13;

  //Room 13
  rooms[13][0] = 12;
  rooms[13][1] = 14;
  rooms[13][2] = 20;

  //Room 14
  rooms[14][0] = 4;
  rooms[14][1] = 13;
  rooms[14][2] = 15;

  //Room 15
  rooms[15][0] = 6;
  rooms[15][1] = 14;
  rooms[15][2] = 16;

  //Room 16
  rooms[16][0] = 15;
  rooms[16][1] = 17;
  rooms[16][2] = 20;

  //Room 17
  rooms[17][0] = 7;
  rooms[17][1] = 16;
  rooms[17][2] = 18;

  //Room 18
  rooms[18][0] = 9;
  rooms[18][1] = 17;
  rooms[18][2] = 19;

  //Room 19
  rooms[19][0] = 11;
  rooms[19][1] = 18;
  rooms[19][2] = 20;

  //Room 20
  rooms[20][0] = 13;
  rooms[20][1] = 16;
  rooms[20][2] = 19;
  
    // Set random initial values for person, Wumpus, bats, arrows and pits
    initializeGame(&gameInfo);

	// Main playing loop.  Break when player dies, or player kills Wumpus
    while (personIsAlive && wumpusIsAlive) 
    {
            
		// Display current room information: Room number, hazards detected
		displayRoomInfo(gameInfo, rooms);
		
		// Prompt for and handle move
		printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
        // Note the extra space in the scanf below between the opening quote " and the %c.  
        //    This skips leading white space in the input so that the newline left over from
        //    a previous move is not read in and used as the current move.  An alternative 
        //    is using getchar() after the scanf() statement.
		scanf(" %c", &typeOfMove);	
									
		typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
		
		// Check all types of user input and handle them.  This uses if-else-if code
		//   rather than switch-case, so that we can take advantage of break and continue.
		if( typeOfMove == 'D') 
    {
			displayCave();
			displayInstructions();
			continue;       // Loop back up to reprompt for the same move
		}
		else if( typeOfMove == 'P') 
    {
			// To assist with play, display the cave layout
			displayCave();
			continue;       // Loop back up to reprompt for the same move
		}
		else if( typeOfMove == 'M') 
    {
			// Move to an adjacent room,
            // Note the extra space in the scanf below between the opening quote " and the %c.  
            //    This skips leading white space in the input so that the newline left over from
            //    a previous move is not read in and used as the current move.  An alternative 
            //    is using getchar() after the scanf() statement.
			scanf(" %d", &nextRoom);
			
			if( roomIsAdjacent(rooms[gameInfo.personRoom], nextRoom)) 
      {
				gameInfo.personRoom = nextRoom;        // move to a new room
				// Check if pit or wumpus is present in this new room
				checkForHazards(&gameInfo, &personIsAlive, rooms);
			}
			else 
      {
				printf("Invalid move.  Please retry. \n");
				continue;        // Doesn't count as a move, so retry same move.
			}
		}
		else if(typeOfMove == 'C') 
    {
			// Display Cheat information
			displayCheatInfo(gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
    else if(typeOfMove == 'S') 
    {
      //if player has the arrow go into arrowFunctions()
      if(gameInfo.arrowRoom == -1)
      {
        arrowFunctions(&gameInfo, &personIsAlive, &wumpusIsAlive, rooms);
      }
      else
      {
        // Attempt to shoot arrow you don't have
	      printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
      }
		}
		else if(typeOfMove == 'R') 
    {
			// Specify resetting the hazards and person positions, useful for testing
			resetPositions(&gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
		else if( typeOfMove == 'X') 
    {
			// Exit program
			personIsAlive = false;   // Indicate person is dead as a way to exit playing loop
			break;
		}

		// Increment the move number
		gameInfo.moveNumber = gameInfo.moveNumber + 1;
        
    }//end while(personIsAlive && wumpusIsAlive)
    
    printf("\nExiting Program ...\n");

  //Deallocates array once done with it
  for(i = 0; i < 21; i++)
    {
    free(rooms[i]);
    rooms[i] = NULL;
    }
  
  free(rooms);
  rooms = NULL;
	
	return 0;
}