#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
/*These included header files define and initialize the classes of the tiles, players, and the two decks.
Chance_and_Community_Chest and Tile are initializing classes.
Board and Deck are defining them.
Player initializes and defines the player since all players start out with the same information -
barring their token.*/
#include "Board.h"
#include "Chance_and_Community_Chest.h"
#include "Deck.h"
#include "Player.h"
#include "Tile.h"

using namespace std;

/*void generateBoard initializes and sets starting values for the two decks, the player, and the board/tiles.*/
void generateBoard();
/*void numberOfPlayers takes the global variable howManyPlayers.
It asks the user to give a number between 2 and 8. This will be how many people are playing the game.
This does change variable howManyPlayers - the user inputs a number.*/
void numberOfPlayers();
/*void chooseTokesn uses the variable howManyPlayers to know how many players need tokens.
Each player in turn is given eight choices, then 7, then 6 as people choose some.
They also have the opportunity to directly input their player.token by choosing custom token.
Every token that is choses will no longer be displayed or be up for grabs.
Does not change howManyPlayers. Will change player[].token.*/
void chooseTokens();
/*void mainGame is where the player */
void mainGame();
/*void rollTheDice is what happens when the player selects to "roll the dice" in the mainGame.
There will be many different things that can happen based off of whether the player is in jail and what tile they land on.
They might buy a property, roll to get out of jail, pay rent, pay rent with two houses.
In this function, all class Player properties may change. The board.tile[].ifOwned may also change.
Inputs count and will change it if player needs to end their turn - like if they land on Go To Jail.*/
void rollTheDice();
void movingTo(short& oldTile);
void propertyIsOwned();
void propertyisUnowned();
void communityChest(short& oldTile);
void chance(short& oldTile);
void manageProperties();
void manageHouses();
void manageTwoPropertyHouses(short propertiesWeCareAbout[], short& increaseOrDecrease);
void manageThreePropertyHouses(short propertiesWeCareAbout[], short& increaseOrDecrease);
void forfeit();

Player player[8];
Board board;
Deck deck;
short diceRoll[2], 
howManyPlayers, 
turn = 0, 
chanceSequence[16], 
nextChance = 0, 
communityChestSequence[16], 
nextCommunityChest = 0;

int main() {
	generateBoard();
	numberOfPlayers();
	chooseTokens();
	mainGame();
}
void generateBoard() {
	bool already;
	srand(time(NULL));

	chanceSequence[0] = rand() % 16;
	for (short count = 1; count < 16;) {
		chanceSequence[count] = rand() % 16;
		already = false;
		for (short count2 = 0; count2 < count; count2++) {
			if (chanceSequence[count] == chanceSequence[count2]) {
				already = true;
			}
		}
		if (!already)
			count++;
	}
	communityChestSequence[0] = rand() % 16;
	for (short count = 1; count < 16;) {
		communityChestSequence[count] = rand() % 16;
		already = false;
		for (short count2 = 0; count2 < count; count2++) {
			if (communityChestSequence[count] == communityChestSequence[count2]) {
				already = true;
			}
		}
		if (!already)
			count++;
	}

	deck.createDecks(communityChestSequence, chanceSequence);
	board.createBoard();
}
/*Very simple function, used once.
Loops how many players are playing until the user gives an acceptable number.*/
void numberOfPlayers() {
	for (;;) {
		cout << "Enter how many players are playing: ";
		cin >> howManyPlayers;
		cin.clear();
		cin.ignore();

		if (howManyPlayers < 2)
			cout << "That's not enough players.\n";
		else if (howManyPlayers > 8)
			cout << "That's too many players.\n";

		else break;
	}
	for (short count = 0; count < howManyPlayers; count++) {
		player[count].isPlaying = 1;
	}
}
/*Loop displays at least once for every player who is playing.
If it has not been taken, the player token is changed.
If it has been taken, the loop loops back.*/
void chooseTokens()
{
	bool tokensLeft[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	for (short count = 0; count < howManyPlayers; count++) {
		short tokenSelected;
		cout << "\n\nWhich token would you like to use, player " << count + 1 << "?" << endl;
		if (tokensLeft[0])
			cout << "1. Scottish Terrier\n";
		if (tokensLeft[1])
			cout << "2. Battleship\n";
		if (tokensLeft[2])
			cout << "3. Automobile\n";
		if (tokensLeft[3])
			cout << "4. Top Hat\n";
		if (tokensLeft[4])
			cout << "5. Thimble\n";
		if (tokensLeft[5])
			cout << "6. Shoe\n";
		if (tokensLeft[6])
			cout << "7. Wheelbarrow\n";
		if (tokensLeft[7])
			cout << "8. Cat\n";
		cout << "9. Custom Token\n";

		cin >> tokenSelected;
		cin.clear();
		cin.ignore();

		if ((tokenSelected < 1) || (tokenSelected > 9)) {
			cout << "That is not a token.\n";
			count--;
			continue;
		}

		if ((tokenSelected == 1) && (tokensLeft[0]))
			player[count].token = "Scottish Terrier";
		else if ((tokenSelected == 2) && (tokensLeft[1]))
			player[count].token = "Battleship";
		else if ((tokenSelected == 3) && (tokensLeft[2]))
			player[count].token = "Automobile";
		else if ((tokenSelected == 4) && (tokensLeft[3]))
			player[count].token = "Top Hat";
		else if ((tokenSelected == 5) && (tokensLeft[4]))
			player[count].token = "Thimble";
		else if ((tokenSelected == 6) && (tokensLeft[5]))
			player[count].token = "Shoe";
		else if ((tokenSelected == 7) && (tokensLeft[6]))
			player[count].token = "Wheelbarrow";
		else if ((tokenSelected == 8) && (tokensLeft[7]))
			player[count].token = "Cat";
		else if (tokenSelected == 9) {
			getline(cin, player[count].token);
			cin.clear();
			cin.ignore();
		}
		else {
			cout << "That token has already been taken. Please choose another.\n";
			count--;
			continue;
		}

		if (tokenSelected == 1)
			tokensLeft[0] = 0;
		else if (tokenSelected == 2)
			tokensLeft[1] = 0;
		else if (tokenSelected == 3)
			tokensLeft[2] = 0;
		else if (tokenSelected == 4)
			tokensLeft[3] = 0;
		else if (tokenSelected == 5)
			tokensLeft[4] = 0;
		else if (tokenSelected == 6)
			tokensLeft[5] = 0;
		else if (tokenSelected == 7)
			tokensLeft[6] = 0;
		else if (tokenSelected == 8)
			tokensLeft[7] = 0;
	}
}
void mainGame() {
	short importantDecision;
	/*Loop that will continuously increase. The count of this loop determines whose turn it is.
	Count 0 - 7 for players 1 - 8 (Players are refernced by their token and array numbers 0 - 7,
	so as to not be confusing. 1 - 8 is only used in deciding the token.*/
	for (;;) {
		/*Count is modulus 8 so the program can keep track of players[0] - [7], and not go over.
		It was this or use count % 8 in every array. At some point that became tedious.*/
		turn = turn % 8;
		/*Only plays the turn if bool isPlaying. Otherwise, count is increased.*/
		if (player[turn].isPlaying) {
			//Color per each player
			if (turn == 0)
				system("Color 70");
			else if (turn == 1)
				system("Color 20");
			else if (turn == 2)
				system("Color 30");
			else if (turn == 3)
				system("Color 80");
			else if (turn == 4)
				system("Color 50");
			else if (turn == 5)
				system("Color 60");
			else if (turn == 6)
				system("Color 40");
			else if (turn == 7)
				system("Color F0");

			cout << "\n\nIt is " << player[turn].token << "'s turn." << endl;
			cout << "You have $" << player[turn].heldMoney << endl;
			if (player[turn].heldMoney < 0)
				cout << "You are in debt. Please mortgage your property or drop out of the game.\n";
			/*Displays options only is they are pertinent.*/
			if (player[turn].inJail)
				cout << "You are in jail.\n";
			if (!player[turn].hasRolled)
				cout << "1. Roll the dice\n";
			if (player[turn].hasMonopoly)
				cout << "2. Manage Houses\n";
			cout << "3. Manage Properties\n";
			cout << "4. Forfeit\n";
			if (player[turn].hasRolled)
				cout << "5. End Turn\n";

			/*Inut choice, and if that was presented to them, it will do something.*/
			cin >> importantDecision;
			cin.clear();
			cin.ignore();

			if ((importantDecision < 1) || (importantDecision > 5)) {
				cout << "That is not a legal choice. Try again" << endl;
				continue;
			}
			/*For example: rolling the dice, rolls the dice and moves the player.
			Each option will have its own function and involve the playe(s).*/
			if (importantDecision == 1) {
				if (player[turn].hasRolled) {
					cout << "You have already rolled this turn. Choose something else. " << endl;
					continue;
				}
				diceRoll[0] = rand() % 6 + 1;
				diceRoll[1] = rand() % 6 + 1;
				rollTheDice();
				if (diceRoll[0] == diceRoll[1])
					continue;
			}
			else if (importantDecision == 2)
				manageHouses();
			else if (importantDecision == 3)
				manageProperties();
			else if (importantDecision == 4)
				forfeit();
			else if (importantDecision == 5) {
				if (player[turn].hasRolled) {
					player[turn].hasRolled = false;
					player[turn].rolledDoubles = 0;
					turn++;
					continue;
				}
				else cout << "You still need to roll.\n" << endl;
			}
		}
		else turn++;
	}
}
void rollTheDice() {
	cout << "You rolled a " << diceRoll[0] + diceRoll[1] << endl;
	player[turn].hasRolled = true;
	if ((player[turn].inJail) && (diceRoll[0] != diceRoll[1])) {
		cout << "You did not roll doubles." << endl;
		player[turn].turnsInJail++;
		if (player[turn].turnsInJail == 3) {
			player[turn].turnsInJail = 0;
			cout << "You did not roll doubles to get out of jail in three turns." << endl
				<< "You are forced to pay bail (or use the get out of jail free card if you have it)." << endl
				<< "You have " << player[turn].heldOutOFJailCards << " get out of jail free card(s)";
			if (player[turn].heldOutOFJailCards > 0)
				player[turn].heldOutOFJailCards--;
			else
				player[turn].heldMoney -= 50;
			player[turn].inJail = false;
		}
		mainGame();
	}
	short oldTile = player[turn].onTile;
	player[turn].onTile += (diceRoll[0] + diceRoll[1]);
	player[turn].onTile = player[turn].onTile % 40;
	movingTo(oldTile);
}
void movingTo(short& oldTile) {
	if (oldTile > player[turn].onTile) {
		cout << "You passed Go and collected $200." << endl;
		player[turn].heldMoney += 200;
	}

	cout << "You landed on " << board.tile[player[turn].onTile].name << endl;

	if (board.tile[player[turn].onTile].type == "property") {
		if (board.tile[player[turn].onTile].owned == true) {
			if (board.tile[player[turn].onTile].ifMortgaged)
				cout << "It is mortgaged.\n";
			else if (board.tile[player[turn].onTile].ownedByPlayer == turn)
				cout << "You own it.\n";
			else
				propertyIsOwned();
		}
		else if (board.tile[player[turn].onTile].owned == false)
			propertyisUnowned();
	}
	else if (board.tile[player[turn].onTile].type == "community chest")
		communityChest(oldTile);
	else if (board.tile[player[turn].onTile].type == "chance")
		chance(oldTile);
	else if (board.tile[player[turn].onTile].type == "tax") {
		cout << "You need to pay $" << board.tile[player[turn].onTile].value << endl;
		player[turn].heldMoney -= board.tile[player[turn].onTile].value;
	}
	else if (player[turn].onTile == 30) {
		player[turn].rolledDoubles = 0;
		player[turn].inJail = true;
		player[turn].onTile = 10;
		cout << "You are in jail.\n";
		turn++;
	}
	if (diceRoll[0] == diceRoll[1]) {
		cout << "You rolled doubles. Please roll again." << endl;
		player[turn].rolledDoubles++;
		player[turn].hasRolled = false;
		//if the player rolls three doubles, they are put in jail and their turn ends
		if (player[turn].rolledDoubles == 3) {
			player[turn].rolledDoubles = 0;
			player[turn].inJail = true;
			player[turn].onTile = 10;
			cout << "You are in jail.\n";
			turn++;
		}
	}
}
void propertyIsOwned() {
	cout << "You landed on " << player[board.tile[player[turn].onTile].ownedByPlayer].token << "'s property.\n" << "You have to pay $";
	if ((player[turn].onTile == 12) || (player[turn].onTile == 28)) {
		if (player[turn].specialUtility) {
			cout << ((diceRoll[0] + diceRoll[1]) * 10);
			player[turn].specialUtility = false;
			player[board.tile[player[turn].onTile].ownedByPlayer].heldMoney += ((diceRoll[0] + diceRoll[1]) * 10);
			player[turn].heldMoney -= ((diceRoll[0] + diceRoll[1]) * 10);
		}
		else if (player[board.tile[player[turn].onTile].ownedByPlayer].ownedUtlities == 1) {
			cout << ((diceRoll[0] + diceRoll[1]) * 4);
			player[board.tile[player[turn].onTile].ownedByPlayer].heldMoney += ((diceRoll[0] + diceRoll[1]) * 4);
			player[turn].heldMoney -= ((diceRoll[0] + diceRoll[1]) * 4);
		}
		else if (player[board.tile[player[turn].onTile].ownedByPlayer].ownedUtlities == 2) {
			cout << ((diceRoll[0] + diceRoll[1]) * 10);
			player[board.tile[player[turn].onTile].ownedByPlayer].heldMoney += ((diceRoll[0] + diceRoll[1]) * 10);
			player[turn].heldMoney -= ((diceRoll[0] + diceRoll[1]) * 10);
		}
	}
	else if ((player[turn].onTile % 10) == 5) {
		if (player[turn].specialRailroad) {
			cout << (25 * pow(2, (player[board.tile[player[turn].onTile].ownedByPlayer].ownedRailroads)));
			player[turn].specialRailroad = false;
			player[board.tile[player[turn].onTile].ownedByPlayer].heldMoney += (25 * pow(2, (player[board.tile[player[turn].onTile].ownedByPlayer].ownedRailroads)));
			player[turn].heldMoney -= (25 * pow(2, (player[board.tile[player[turn].onTile].ownedByPlayer].ownedRailroads)));
		}
		else {
			cout << (25 * pow(2, (player[board.tile[player[turn].onTile].ownedByPlayer].ownedRailroads - 1)));
			player[board.tile[player[turn].onTile].ownedByPlayer].heldMoney += (25 * pow(2, (player[board.tile[player[turn].onTile].ownedByPlayer].ownedRailroads - 1)));
			player[turn].heldMoney -= (25 * pow(2, (player[board.tile[player[turn].onTile].ownedByPlayer].ownedRailroads - 1)));
		}
	}
	else {
		if ((board.tile[player[turn].onTile].ifMonopoly == true) && (board.tile[player[turn].onTile].numberOfHouses == 0)) {
			cout << board.tile[player[turn].onTile].rent[0] * 2;
			player[board.tile[player[turn].onTile].ownedByPlayer].heldMoney += (board.tile[player[turn].onTile].rent[0] * 2);
			player[turn].heldMoney -= (board.tile[player[turn].onTile].rent[0] * 2);
		}
		else {
			cout << board.tile[player[turn].onTile].rent[board.tile[player[turn].onTile].numberOfHouses];
			player[board.tile[player[turn].onTile].ownedByPlayer].heldMoney += (board.tile[player[turn].onTile].rent[board.tile[player[turn].onTile].numberOfHouses]);
			player[turn].heldMoney -= (board.tile[player[turn].onTile].rent[board.tile[player[turn].onTile].numberOfHouses]);
		}
	}
	cout << endl;
}
void propertyisUnowned() {
	cout << board.tile[player[turn].onTile].name
		<< " is unowned. Would you like to buy "
		<< board.tile[player[turn].onTile].name << " for $"
		<< board.tile[player[turn].onTile].value << "?" << endl
		<< "1. Yes\n2. No" << endl;
	int iWouldLikeToBuyThatSir;
	cin >> iWouldLikeToBuyThatSir;
	cin.clear();
	cin.ignore();
	if (iWouldLikeToBuyThatSir == 1) {
		player[turn].heldMoney -= board.tile[player[turn].onTile].value;
		board.tile[player[turn].onTile].owned = true;
		board.tile[player[turn].onTile].ownedByPlayer = turn;
		if ((player[turn].onTile == 12) || (player[turn].onTile == 28))
			player[turn].ownedUtlities++;
		else if ((player[turn].onTile % 10) == 5)
			player[turn].ownedRailroads++;
		else if (((player[turn].onTile == 1) || (player[turn].onTile == 3)) && ((board.tile[1].ownedByPlayer == turn) && (board.tile[3].ownedByPlayer == turn))) {
			board.tile[1].ifMonopoly = true;
			board.tile[3].ifMonopoly = true;
			player[turn].hasMonopoly = true;
		}
		else if ((player[turn].onTile == 6) || (player[turn].onTile == 8) || (player[turn].onTile == 9)) {
			if ((board.tile[6].ownedByPlayer == turn) && (board.tile[8].ownedByPlayer == turn) && (board.tile[9].ownedByPlayer == turn)) {
				board.tile[6].ifMonopoly = true;
				board.tile[8].ifMonopoly = true;
				board.tile[9].ifMonopoly = true;
				player[turn].hasMonopoly = true;
			}
		}
		else if ((player[turn].onTile == 11) || (player[turn].onTile == 13) || (player[turn].onTile == 14)) {
			if ((board.tile[11].ownedByPlayer == turn) && (board.tile[13].ownedByPlayer == turn) && (board.tile[14].ownedByPlayer == turn)) {
				board.tile[11].ifMonopoly = true;
				board.tile[13].ifMonopoly = true;
				board.tile[14].ifMonopoly = true;
				player[turn].hasMonopoly = true;
			}
		}
		else if ((player[turn].onTile == 16) || (player[turn].onTile == 18) || (player[turn].onTile == 19)) {
			if ((board.tile[16].ownedByPlayer == turn) && (board.tile[18].ownedByPlayer == turn) && (board.tile[19].ownedByPlayer == turn)) {
				board.tile[16].ifMonopoly = true;
				board.tile[18].ifMonopoly = true;
				board.tile[19].ifMonopoly = true;
				player[turn].hasMonopoly = true;
			}
		}
		else if ((player[turn].onTile == 21) || (player[turn].onTile == 23) || (player[turn].onTile == 24)) {
			if ((board.tile[21].ownedByPlayer == turn) && (board.tile[23].ownedByPlayer == turn) && (board.tile[24].ownedByPlayer == turn)) {
				board.tile[21].ifMonopoly = true;
				board.tile[23].ifMonopoly = true;
				board.tile[24].ifMonopoly = true;
				player[turn].hasMonopoly = true;
			}
		}
		else if ((player[turn].onTile == 26) || (player[turn].onTile == 27) || (player[turn].onTile == 29)) {
			if ((board.tile[26].ownedByPlayer == turn) && (board.tile[27].ownedByPlayer == turn) && (board.tile[29].ownedByPlayer == turn)) {
				board.tile[6].ifMonopoly = true;
				board.tile[8].ifMonopoly = true;
				board.tile[9].ifMonopoly = true;
				player[turn].hasMonopoly = true;
			}
		}
		else if ((player[turn].onTile == 31) || (player[turn].onTile == 32) || (player[turn].onTile == 34)) {
			if ((board.tile[31].ownedByPlayer == turn) && (board.tile[32].ownedByPlayer == turn) && (board.tile[34].ownedByPlayer == turn)) {
				board.tile[31].ifMonopoly = true;
				board.tile[32].ifMonopoly = true;
				board.tile[34].ifMonopoly = true;
				player[turn].hasMonopoly = true;
			}
		}
		else if (((player[turn].onTile == 37) || (player[turn].onTile == 39)) && ((board.tile[37].ownedByPlayer == turn) && (board.tile[39].ownedByPlayer == turn))) {
			board.tile[37].ifMonopoly = true;
			board.tile[39].ifMonopoly = true;
			player[turn].hasMonopoly = true;
		}
	}
}
void communityChest(short& oldTile) {
	nextCommunityChest++;
	nextCommunityChest = nextCommunityChest % 16;
	short totalHouses = 0, totalHotels = 0;
	cout << deck.communityChest[communityChestSequence[nextCommunityChest]].text << endl;
	player[turn].heldMoney += deck.communityChest[communityChestSequence[nextCommunityChest]].getMoney;
	player[turn].heldMoney += (deck.communityChest[communityChestSequence[nextCommunityChest]].getMoneyFromPlayers * (howManyPlayers - 1));
	for (int giveMoney = 0; giveMoney < 8; giveMoney++) {
		if ((player[giveMoney].isPlaying) && (turn != giveMoney)) {
			player[giveMoney].heldMoney += deck.communityChest[communityChestSequence[nextCommunityChest]].getMoneyFromPlayers;
		}
	}
	if (deck.communityChest[communityChestSequence[nextCommunityChest]].goToJail) {
		player[turn].onTile = 10;
		player[turn].inJail = true;
	}
	else if (communityChestSequence[nextCommunityChest] == communityChestSequence[13]) {
		for (int count = 0; count < 40; count++) {
			if (board.tile[count].ownedByPlayer == turn) {
				if (board.tile[count].numberOfHouses == 5)
					totalHotels++;
				else
					totalHouses += board.tile[count].numberOfHouses;
			}
		}
		player[turn].heldMoney -= ((totalHouses * 40) + (totalHotels * 115));
	}
	else if (deck.communityChest[communityChestSequence[nextCommunityChest]].movePlayer) {
		player[turn].onTile = deck.communityChest[communityChestSequence[nextCommunityChest]].moveTo;
		movingTo(oldTile);
	}
	else if (deck.communityChest[communityChestSequence[nextCommunityChest]].getOutOfJail) {
		player[turn].heldOutOFJailCards++;
	}
}
void chance(short& oldTile) {
	nextChance++;
	nextChance = nextChance % 16;
	short totalHouses = 0, totalHotels = 0;
	cout << deck.chance[chanceSequence[nextChance]].text << endl;
	player[turn].heldMoney += deck.chance[chanceSequence[nextChance]].getMoney;
	player[turn].heldMoney += (deck.chance[chanceSequence[nextChance]].getMoneyFromPlayers * (howManyPlayers - 1));
	for (short giveMoney = 0; giveMoney < 8; giveMoney++) {
		if ((player[giveMoney].isPlaying) && (turn != giveMoney)) {
			player[giveMoney].heldMoney += deck.communityChest[chanceSequence[nextChance]].getMoneyFromPlayers;
		}
	}
	if (deck.chance[chanceSequence[nextChance]].goToJail) {
		player[turn].onTile = 10;
		player[turn].inJail = true;
	}
	else if (chanceSequence[nextChance] == chanceSequence[3]) {
		player[turn].specialUtility = true;
		if (player[turn].onTile == 7) {
			player[turn].onTile = 12;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			if (board.tile[player[turn].onTile].owned)
				propertyIsOwned();
			else {
				player[turn].specialUtility = false;
				propertyisUnowned();
			}
		}
		else if (player[turn].onTile == 22) {
			player[turn].onTile = 28;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			if (board.tile[player[turn].onTile].owned)
				propertyIsOwned();
			else {
				player[turn].specialUtility = false;
				propertyisUnowned();
			}
		}
		else if (player[turn].onTile == 36) {
			player[turn].onTile = 12;
			cout << "You passed Go and collected $200." << endl;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			player[turn].heldMoney += 200;
			if (board.tile[player[turn].onTile].owned)
				propertyIsOwned();
			else {
				player[turn].specialUtility = false;
				propertyisUnowned();
			}
		}
	}
	else if (chanceSequence[nextChance] == chanceSequence[4]) {
		player[turn].specialRailroad = true;
		if (player[turn].onTile == 7) {
			player[turn].onTile = 15;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			if (board.tile[player[turn].onTile].owned)
				propertyIsOwned();
			else {
				player[turn].specialRailroad = false;
				propertyisUnowned();
			}
		}
		else if (player[turn].onTile == 22) {
			player[turn].onTile = 25;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			if (board.tile[player[turn].onTile].owned)
				propertyIsOwned();
			else {
				player[turn].specialRailroad = false;
				propertyisUnowned();
			}
		}
		else if (player[turn].onTile == 36) {
			player[turn].onTile = 5;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			cout << "You passed Go and collected $200." << endl;
			player[turn].heldMoney += 200;
			if (board.tile[player[turn].onTile].owned)
				propertyIsOwned();
			else {
				player[turn].specialRailroad = false;
				propertyisUnowned();
			}
		}
	}
	else if (chanceSequence[nextChance] == chanceSequence[7]) {
		if (player[turn].onTile == 7) {
			player[turn].onTile = 4;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			cout << "You need to pay " << board.tile[player[turn].onTile].value << endl;
			player[turn].heldMoney -= board.tile[player[turn].onTile].value;
			mainGame();
		}
		else if (player[turn].onTile == 22) {
			player[turn].onTile = 19;
			cout << "You landed on " << board.tile[player[turn].onTile].name << endl;
			if (board.tile[player[turn].onTile].owned)
				propertyIsOwned();
			else {
				propertyisUnowned();
			}
		}
		else if (player[turn].onTile == 36) {
			player[turn].onTile = 33;
			communityChest(oldTile);
		}
	}
	else if (chanceSequence[nextChance] == chanceSequence[9]) {
		for (int count = 0; count < 40; count++) {
			if (board.tile[count].ownedByPlayer == turn) {
				if (board.tile[count].numberOfHouses == 5)
					totalHotels++;
				else
					totalHouses += board.tile[count].numberOfHouses;
			}
		}
		player[turn].heldMoney -= ((totalHouses * 25) + (totalHotels * 100));
	}
	else if (deck.chance[chanceSequence[nextChance]].movePlayer) {
		player[turn].onTile = deck.chance[chanceSequence[nextChance]].moveTo;
		movingTo(oldTile);
	}
	else if (deck.chance[chanceSequence[nextChance]].getOutOfJail) {
		player[turn].heldOutOFJailCards++;
	}
	else if (board.tile[player[turn].onTile].type == "go to jail") {
		player[turn].onTile = 10;
		player[turn].inJail = true;
		turn++;
	}
}
void manageProperties() {
	cout << "These are the properties you own:\n";
	short propertiesOwned;
	for (propertiesOwned = 0; propertiesOwned < 40; propertiesOwned++) {
		if (board.tile[propertiesOwned].ownedByPlayer == turn) {
			cout << propertiesOwned << ". " << board.tile[propertiesOwned].name;
			if (board.tile[propertiesOwned].ifMortgaged)
				cout << ", Mortgaged";
			else
				cout << ", Not Mortgaged";
			cout << ", Mortgage value is: " << board.tile[propertiesOwned].value / 2 << endl;
		}
	}
	short youWantToMortgage;
	cout << endl << "What 1 property would you like to toggle the mortgage on?" << endl
		<< "Remember, mortgage value is half of the value of the property, but it takes 10% more than that to unmortage.\n"
		<< "You can do this as many times as you would like during your turn. If you are done, type 40." << endl;
	for (;;) {
		cin >> youWantToMortgage;
		cin.clear();
		cin.ignore();
		if (youWantToMortgage == 40) {
			//turn--;
			mainGame();
		}
		if (board.tile[youWantToMortgage].ownedByPlayer != turn)
			cout << "I am sorry. You do not own this property. Please try a different one.\n";
		else {
			if (board.tile[propertiesOwned].ifMortgaged) {
				board.tile[propertiesOwned].ifMortgaged = false;
				player[turn].heldMoney -= ((board.tile[youWantToMortgage].value / 2) * 1.1);
				cout << "It is now unmortgaged. You now have $" << player[turn].heldMoney << endl;
			}
			else if (!board.tile[propertiesOwned].ifMortgaged) {
				board.tile[propertiesOwned].ifMortgaged = true;
				player[turn].heldMoney += (board.tile[youWantToMortgage].value / 2);
				cout << "It is now mortgaged. You now have $" << player[turn].heldMoney << endl;
			}
		}
	}
}
void manageHouses() {
	cout << "These are the properties you own that are also part of a monopoly:\n";
	short propertiesOwned;
	for (propertiesOwned = 0; propertiesOwned < 40; propertiesOwned++) {
		if (board.tile[propertiesOwned].ownedByPlayer == turn) {
			if (board.tile[propertiesOwned].ifMonopoly) {
				cout << board.tile[propertiesOwned].color << ". " << board.tile[propertiesOwned].name;
				cout << ", Number of houses: " << board.tile[propertiesOwned].value / 2 << endl;
			}
		}
	}
	//This variable is a joke (praise the claw)
	//TrumBuildingCo allows for building houses - get it because Trump is a monopoly. Hahahahaaaa... *gets punched
	string TrumpBuildingCo;
	short increaseOrDecrease;
	short propertiesWeCareAbout[3];
	for (;;) {
		cout << endl << "Type one color that corresponds to a monopoly you own." << endl
			<< "You can do this as many times as you would like during your turn. If you are done, type stop." << endl;
		getline(cin, TrumpBuildingCo);
		transform(TrumpBuildingCo.begin(), TrumpBuildingCo.end(), TrumpBuildingCo.begin(), tolower);
		if (TrumpBuildingCo == "stop")
			mainGame();
		if (TrumpBuildingCo == "brown") {
			propertiesWeCareAbout[0] = 1;
			propertiesWeCareAbout[1] = 3;
			manageTwoPropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
		else if (TrumpBuildingCo == "light blue") {
			propertiesWeCareAbout[0] = 6;
			propertiesWeCareAbout[1] = 8;
			propertiesWeCareAbout[2] = 9;
			manageThreePropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
		else if (TrumpBuildingCo == "magenta") {
			propertiesWeCareAbout[0] = 11;
			propertiesWeCareAbout[1] = 13;
			propertiesWeCareAbout[2] = 14;
			manageThreePropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
		else if (TrumpBuildingCo == "orange") {
			propertiesWeCareAbout[0] = 16;
			propertiesWeCareAbout[1] = 18;
			propertiesWeCareAbout[2] = 19;
			manageThreePropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
		else if (TrumpBuildingCo == "red") {
			propertiesWeCareAbout[0] = 21;
			propertiesWeCareAbout[1] = 23;
			propertiesWeCareAbout[2] = 24;
			manageThreePropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
		else if (TrumpBuildingCo == "yellow") {
			propertiesWeCareAbout[0] = 26;
			propertiesWeCareAbout[1] = 27;
			propertiesWeCareAbout[2] = 29;
			manageThreePropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
		else if (TrumpBuildingCo == "green") {
			propertiesWeCareAbout[0] = 31;
			propertiesWeCareAbout[1] = 32;
			propertiesWeCareAbout[2] = 34;
			manageThreePropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
		else if (TrumpBuildingCo == "blue") {
			propertiesWeCareAbout[0] = 37;
			propertiesWeCareAbout[1] = 39;
			manageTwoPropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
		}
	}
}
void manageTwoPropertyHouses(short propertiesWeCareAbout[], short& increaseOrDecrease) {
	if ((board.tile[propertiesWeCareAbout[0]].ownedByPlayer == turn) && (board.tile[propertiesWeCareAbout[1]].ownedByPlayer == turn))
		cout << "Would you like to increase or decrease the number of houses?\n"
		<< "Each house costs " << board.tile[propertiesWeCareAbout[0]].costOfHouse << ", but since there are two properties in the monopoly,"
		<< " it will cost " << (board.tile[propertiesWeCareAbout[0]].costOfHouse * 2) << " to put a house on both. \nLikewise,"
		<< " you will receive " << board.tile[propertiesWeCareAbout[0]].costOfHouse << " to sell a house.\n"
		<< " You can do this as many times as you would like.\n"
		<< "1. Increase by 1\n2. Decrease by 1.\n";
	cin >> increaseOrDecrease;
	cin.clear();
	cin.ignore();
	if ((increaseOrDecrease < 1) || (increaseOrDecrease > 2))
		cout << "Not an option. Try again.\n";
	else if (increaseOrDecrease == 1) {
		board.tile[propertiesWeCareAbout[0]].numberOfHouses++;
		board.tile[propertiesWeCareAbout[1]].numberOfHouses++;
		cout << endl << board.tile[propertiesWeCareAbout[0]].color << " now has " << board.tile[propertiesWeCareAbout[0]].numberOfHouses << " houses.\n";
		player[turn].heldMoney -= (board.tile[propertiesWeCareAbout[0]].costOfHouse * 2);
		cout << "You have $" << player[turn].heldMoney << endl;
	}
	else if (increaseOrDecrease == 2) {
		board.tile[propertiesWeCareAbout[0]].numberOfHouses--;
		board.tile[propertiesWeCareAbout[1]].numberOfHouses--;
		cout << endl << board.tile[propertiesWeCareAbout[0]].color << " now has " << board.tile[propertiesWeCareAbout[0]].numberOfHouses << " houses.\n";
		player[turn].heldMoney -= board.tile[propertiesWeCareAbout[0]].costOfHouse;
		cout << "You have $" << player[turn].heldMoney << endl;
	}
	else {
		cout << "That was not an option.\n";
		manageTwoPropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
	}
}
void manageThreePropertyHouses(short propertiesWeCareAbout[], short& increaseOrDecrease) {
	if ((board.tile[propertiesWeCareAbout[0]].ownedByPlayer == turn) && (board.tile[propertiesWeCareAbout[1]].ownedByPlayer == turn) && (board.tile[propertiesWeCareAbout[2]].ownedByPlayer == turn))
		cout << "Would you like to increase or decrease the number of houses?\n"
		<< "Each house costs " << board.tile[propertiesWeCareAbout[0]].costOfHouse << ", but since there are three properties in the monopoly,"
		<< " it will cost " << (board.tile[propertiesWeCareAbout[0]].costOfHouse * 3) << " to put a house on both. \nLikewise,"
		<< " you will receive " << (board.tile[propertiesWeCareAbout[0]].costOfHouse * 1.5) << " to sell a house.\n"
		<< " You can do this as many times as you would like.\n"
		<< "1. Increase by 1\n2. Decrease by 1.\n";
	cin >> increaseOrDecrease;
	cin.clear();
	cin.ignore();
	if ((increaseOrDecrease < 1) || (increaseOrDecrease > 2))
		cout << "Not an option. Try again.\n";
	else if (increaseOrDecrease == 1) {
		board.tile[propertiesWeCareAbout[0]].numberOfHouses++;
		board.tile[propertiesWeCareAbout[1]].numberOfHouses++;
		board.tile[propertiesWeCareAbout[2]].numberOfHouses++;
		cout << endl << board.tile[propertiesWeCareAbout[0]].color << " now has " << board.tile[propertiesWeCareAbout[0]].numberOfHouses << " houses.\n";
		player[turn].heldMoney -= (board.tile[propertiesWeCareAbout[0]].costOfHouse * 3);
		cout << "You have $" << player[turn].heldMoney << endl;
	}
	else if (increaseOrDecrease == 2) {
		board.tile[propertiesWeCareAbout[0]].numberOfHouses--;
		board.tile[propertiesWeCareAbout[1]].numberOfHouses--;
		board.tile[propertiesWeCareAbout[2]].numberOfHouses--;
		cout << endl << board.tile[propertiesWeCareAbout[0]].color << " now has " << board.tile[propertiesWeCareAbout[0]].numberOfHouses << " houses.\n";
		player[turn].heldMoney += (board.tile[propertiesWeCareAbout[0]].costOfHouse * 1.5);
		cout << "You have $" << player[turn].heldMoney << endl;
	}
	else {
		cout << "That was not an option.\n";
		manageThreePropertyHouses(propertiesWeCareAbout, increaseOrDecrease);
	}
}
void forfeit() {
	cout << "You are out of the game. All your properties are now unowned, unmortgaged, and unhoused." << endl;
	player[turn].isPlaying = false;
	player[turn].hasRolled = false;
	player[turn].rolledDoubles = 0;
	player[turn].turnsInJail = 0;
	player[turn].inJail = false;
	player[turn].heldMoney = 0;
	player[turn].onTile = 0;
	player[turn].heldOutOFJailCards = 0;
	player[turn].hasMonopoly = false;
	player[turn].ownedRailroads = 0;
	player[turn].ownedUtlities = 0;
	for (short count = 0; count < 40; count++) {
		if (board.tile[count].ownedByPlayer = turn) {
			board.tile[count].ifMortgaged = false;
			board.tile[count].numberOfHouses = 0;
			board.tile[count].ifMonopoly = false;
			board.tile[count].owned = false;
			board.tile[count].ownedByPlayer = 8;
		}
	}
	turn++;
}