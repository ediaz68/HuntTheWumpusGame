#include <stdio.h>
#include <stdlib.h>		// for srand


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
int main(void) {

	// Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);
    // How to get a random number
    int randomValue = rand() % 20 + 1;   // Using +1 gives random number 1..20 rather than 0..19

	// Below are all the messages used throughout the program. You may need to change them,
	// or move them to a different place in the code.
	printf("1. Enter your move (or 'D' for directions): ");	// prompt for user to enter their move
	printf("Invalid move.  Please retry. \n");	// message for invalid move

	// Message for menu option C (cheating). Note that the printf() statement below always prints
	// 1, 2, 3, 4, 5, 6, 7
	printf( "Cheating! Game elements are in the following rooms: \n"
    		"Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
    		"%4d %5d %6d %5d %5d %5d %5d \n\n", 
			1,
			2,
			3,
			4,
			5,
			6,
			7
		);

	// All messages possible when player is in a given room
	printf("You are in room 1. ");
	printf("You smell a stench. ");
	printf("You feel a draft. ");
	printf("You hear rustling of bat wings. ");
	printf("\n\n");

	// Message when the player dies by falling into a pit
	printf("Aaaaaaaaahhhhhh....   \n");
	printf("    You fall into a pit and die. \n");

	// Message when bats transport player
	printf("Woah... you're flying! \n");
    printf("You've just been transported by bats to room %d.\n", 1); // always prints they are going to room 1

	/* ---- All messages related to the arrow --- */

	// Attempt to shoot arrow you don't have
	printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
	
	// Prompt user to enter input
	printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
	
	// User enters more than 3 rooms to shoot arrow through
	printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
	
	// Arrow killed Wumpus
	printf( "Wumpus has just been pierced by your deadly arrow! \n"
            "Congratulations on your victory, you awesome hunter you.\n");
	
	// Arrow killed player
	printf( "You just shot yourself.  \n"
            "Maybe Darwin was right.  You're dead.\n");
	
	// Attempt to shoot arrow through room that is not adjacent
	printf("Room %d is not adjacent.  Arrow ricochets...\n", 1); // always prints room 1
	
	// Arrow ricochets, and kills person
	printf( "You just shot yourself, and are dying.\n"
            "It didn't take long, you're dead.\n");
	
	// Arrow ricochets, and kills Wumpus
	printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
            "Accidental victory, but still you win!\n");

	// Player picks up arrow
	printf("Congratulations, you found the arrow and can once again shoot.\n");

	/* --- End of messages related to the arrow --- */

	// Both possible messages when the player enters a room that has the Wumpus
	printf( "You hear a slithering sound, as the Wumpus slips away. \n"
			"Whew, that was close! \n");
	printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
			"It is over.\n");

    // Both possible messages after the user guesses where the Wumpus is, for a win or loss.
    printf("You won!\n");
    printf("You lost.\n");
    
	// Prompt for user when they choose to reset
	printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n");

    // Prompt for guess of where Wumpus room is
    printf("Enter room (1..20) you think Wumpus is in: ");

	// Message that prints at the end of the program
	printf("\nExiting Program ...\n");
	
	return 0;
}