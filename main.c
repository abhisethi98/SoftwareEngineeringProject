#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

//Struct to store the information of the player
typedef struct
{
    char name[20];
    char type[20];
    int lifePoints;
    int smartness,strength, magicSkills, luck,dexterity;
    
}

player;

//struct to store the information of the slot
typedef struct
{
    char slotType[20];
    int slotTypeNo;
    int playerNo;
}

slot;

//array to store the player types
char playerTypes[4][20]={"Elf","Human","Ogre","Wizard"};

//array to store the slot types
char slotType[3][20]={"Level Ground","Hill","City"};

//method to set the capabilities of a player
void setCapabilities(player playerArray[],int playerNumber,int playerTypeChoice);

//method to update the capabilities of the player on moving to next slot
void updateCapabiltitesOnMovingSlot(player playerArray[],int playerNumber,int slotTypeMovedTo);

//method to print the details after a successful turn
void printDetails(player playerArray[],int noOfPlayers);

//method to find the closest player to attack
int findClosestPlayer(int attackerPlayerNo,slot slotArray[], int noOfSlots);

//method to implement the attack
void implementingAttack(player playerArray[],int attackerPlayerNo,int attackedPlayerNo);

//method to perform the actions
void performActions(player playerArray[],slot slotArray[],int noOfPlayers,int noOfSlots);


int main()
{
    int noOfPlayers;
    
    printf("Enter the number of players: ");
    scanf("%d",&noOfPlayers);
    
    while(!(noOfPlayers>=2 && noOfPlayers<=6))
    {
        printf("Minimum number of players is 2 and Maximum number of players is 6\nPlease re-enter your choice: ");
        scanf("%d",&noOfPlayers);
    }
    
    //make a player array to store the player information
    player playerArray[noOfPlayers];
    
    //loop to store the player information
    for(int a=0;a<noOfPlayers;a++)
    {
        //input the player name
        printf("Please enter the name of Player %d: ",(a+1));
        scanf("%s",playerArray[a].name);
        
        //input the player type
        printf("Please enter the type of Player %d\n",(a+1));
        printf("Enter 1 for Elf\nEnter 2 for Human\nEnter 3 for Orge\nEnter 4 for Wizard: ");
        int playerTypeChoice=0;
        scanf("%d",&playerTypeChoice);
        
        //loop to check if input is correct
        //if not again asks for input
        while(!(playerTypeChoice>=1 && playerTypeChoice<=4))
        {
            printf("Wrong Choice Entered\nRe-enter your choice: ");
            scanf("%d",&playerTypeChoice);
            
        }
        
        //this works
        
        strcpy(playerArray[a].type,playerTypes[playerTypeChoice-1]);
        
        //calling the method to set the capability of the player
        setCapabilities(playerArray, a,playerTypeChoice);
    }
    
    
    //input the number of slots
    printf("Please Enter the number of slots (Maximum is 20):");
    int noOfSlots;
    scanf("%d",&noOfSlots);
    
    //checks if number of slots is within range
    //if not takes another input
    while(!(noOfSlots>noOfPlayers && noOfSlots<=20))
    {
        printf("Number of slots selected is out of bound\nRe-enter the number of slots: ");
        scanf("%d",&noOfSlots);
    }
    
    //creates slot array to store information about the slots
    slot slotsArray[noOfSlots];
    
    //initialising the slot
    //initialising the slotTypeNo and PlayerNo to -1
    for(int a=0;a<noOfSlots;a++)
    {
        slotsArray[a].slotTypeNo=-1;
        slotsArray[a].playerNo=-1;
    }
    srand((unsigned int)time(NULL));
    
    
    //stores the slotType
    for(int a=0;a<noOfSlots;a++)
    {
        slotsArray[a].slotTypeNo=rand()%3;
        strcpy(slotsArray[a].slotType,slotType[slotsArray[a].slotTypeNo]);
    }
    
    
    //stores the playerNumber
    for(int a=0;a<noOfPlayers;a++)
    {
        //generates a random number
        int randomNumber=rand()%noOfSlots;
        
        //if the slot is not occupied by any player
        //then the player is allocated to the slot
        if(slotsArray[randomNumber].playerNo==-1)
        {
            slotsArray[randomNumber].playerNo=a;
        }
        else
        {
            //generating a random number
            randomNumber=rand()%noOfSlots;
            
            //loop runs until it finds a slot position which was empty and had no player in it
            while(!(slotsArray[randomNumber].playerNo==-1))
            {
                //generating random number
                randomNumber=rand()%noOfSlots;
                
            }
            
            //stores the player in the slot
            slotsArray[randomNumber].playerNo=a;
        }
        
    }
    
    //call the method to start the game
    performActions(playerArray, slotsArray, noOfPlayers, noOfSlots);
    
}

//method to perform the actions
void performActions(player playerArray[],slot slotArray[],int noOfPlayers,int noOfSlots)
{
    //loop which runs throught the number of players
    //each player gets one chance
    for(int a=0;a<noOfPlayers;a++)
    {
        //boolean to store if the players turn was successful
        //if not successful then player does his turn again
        bool turnSuccessful=false;
        
        //Player gets his turn
        printf("%s : Please do your turn\n",playerArray[a].name);
        
        //player inputs his move
        printf("Enter 1 to attack the closest player\nEnter 2 to move to the previous slot\nEnter 3 to move to the next slot\n");
      int moveChoice;
        scanf("%d",&moveChoice);
        
        //checks if the choice entered was correct
        //if not takes an new input again
        while(!(moveChoice>=1 && moveChoice<=3))
        {
            printf("Wrong Choice Entered\nRe-enter your choice: ");
            scanf("%d",&moveChoice);
        }
        
        int playerSlotPosition=0;
        
        //finds the player position in the slotArray
        for(int j=0;j<noOfSlots;j++)
        {
            if(slotArray[j].playerNo==a)
            {
                playerSlotPosition=j;
            }
        }
        
        //checks the move taken by the user
        if(moveChoice==1)
        {
            //calls the method to return the closest player to be attacked
            int attackedPlayerNo=findClosestPlayer(a,slotArray,noOfSlots);
            
            //calls the method to implement the attack
            implementingAttack(playerArray, a, attackedPlayerNo);
            
            //updates the turnSuccessful
            turnSuccessful=true;
        }
        //checks if the move is to move previous
        else if(moveChoice==2)
        {
            //checks if the previous position is possible
            if(playerSlotPosition-1>=0)
            {
                //checks if the slot has a player
                if(slotArray[playerSlotPosition-1].playerNo==-1)
                {
                    //method to update the capabilities on moving the slot
                    updateCapabiltitesOnMovingSlot(playerArray, a, slotArray[playerSlotPosition-1].slotTypeNo);
                    
                    //updating the turnSuccessful
                    turnSuccessful=true;
                }
                else
                {
                    //prints a message if the previous slot was not possible
                    printf("Unable to move to the previous slot\n");
                }
            }
            else
            {
                //print a message if the previous slot was not possible
                printf("Unable to move to the previous slot\n");
            }
        }
        else
        {
            //checks if the next slot is possible i.e within bounds
            if(playerSlotPosition+1<noOfSlots)
            {
                //checks if the next slot is empty
                if(slotArray[playerSlotPosition+1].playerNo==-1)
                {
                    //calling the method to update the capabilities of the player
                    updateCapabiltitesOnMovingSlot(playerArray, a, slotArray[playerSlotPosition+1].slotTypeNo);
                    
                    //update the turn successful
                    turnSuccessful=true;
                }
                else
                {
                    //print a message if the next slot is not possible
                    printf("Unable to move to the next slot\n");
                }
            }
            else
            {
                //print a message if the next slot is not possible
                printf("Unable to move to the next slot\n");
            }
            
        }
        
        //if the turn was successful
        if(turnSuccessful)
        {
            //calling the method to print the details
            printDetails(playerArray, noOfPlayers);
        }
        else
        {
            //if unsuccessful then decrements the value of i
            //player i gets another chance to make a move
            a--;
        }
        
    }
}
//method to find the closest player
int findClosestPlayer(int attackerPlayerNo,slot slotArray[], int noOfSlots)
{
    //variables to store the attacker nd attacked player slot position
    int attackerPlayerSlotPosition=0;
    int attackedPlayerSlotPosition=0;
    
    //loop to find the position of the attacker
    for(int a=0;a<noOfSlots;a++)
    {
        if(slotArray[a].playerNo==attackerPlayerNo)
        {
            attackerPlayerSlotPosition=a;
        }
    }
    
    //boolean to store if the closest player has been found
    bool foundClosestPlayer=false;
    
    //counter to keep a track of the varaiable
    int counter=1;
    
    //runs loop until finds the closest enemy
    while(foundClosestPlayer!=true)
    {
        //checks if the attacker slot position has a previous or next slot
        if(attackerPlayerSlotPosition-counter>=0 || attackerPlayerSlotPosition+counter<noOfSlots)
        {
            //checks if the attacker slot position has a previous and a next slot
            if(attackerPlayerSlotPosition-counter>=0 && attackerPlayerSlotPosition+counter<noOfSlots)
            {
                //checks if both the slots has a player in it to attack
                if((slotArray[attackerPlayerSlotPosition+counter].playerNo!=-1)&&(slotArray[attackerPlayerSlotPosition-counter].playerNo!=-1))
                {
                    //updates the foundClosestPlayer to true
                    foundClosestPlayer=true;
                    
                    //gives an option to the player to attack either the previous player or the next player
                    printf("There are 2 closest slots to attack\nPress 1 to attack precious slot\nPress 2 to attack next slot\n");
                    int slotChoice;
                    scanf("%d",&slotChoice);
                    
                    //if wrong input then re enter the choice
                    while(!(slotChoice==1 || slotChoice==2))
                    {
                        printf("Wrong Choice Entered\nRe-enter your choice: ");
                        scanf("%d",&slotChoice);
                    }
                    
                    //checks if the user wanted option 1 to attack previous player
                    if(slotChoice==1)
                    {
                        attackedPlayerSlotPosition=attackerPlayerSlotPosition-counter;
                    }
                    else{
                        //if the user wanted to attack the next player
                        attackedPlayerSlotPosition=attackerPlayerSlotPosition+counter;
                    }
                    
                }
                //checks if the previous slot has a player in it
                else if(slotArray[attackerPlayerSlotPosition-counter].playerNo!=-1)
                {
                    //updates the foundClosestPlayer to true
                    foundClosestPlayer=true;
                    
                    //updates the attackedPlayerSlotPosition
                    attackedPlayerSlotPosition=attackerPlayerSlotPosition-counter;
                }
                else if(slotArray[attackerPlayerSlotPosition+counter].playerNo!=-1)
                {
                    //updates the foundClosestPlayer to true
                    foundClosestPlayer=true;
                    
                    //updates the attackedPlayerSlotPosition
                    attackedPlayerSlotPosition=attackerPlayerSlotPosition+counter;
                }
            }
            //checks if the attacker slot position has a previous slot possible
            else if(attackerPlayerSlotPosition-counter>=0)
            {
                //checks if the previous slot has a player to attack
                if(slotArray[attackerPlayerSlotPosition-counter].playerNo!=-1)
                {
                    //updates the foundClosestPlayer to true
                    foundClosestPlayer=true;
                    
                    //updates the attackedPlayerSlotPosition
                    attackedPlayerSlotPosition=attackerPlayerSlotPosition-counter;
                }
            }
            //checks if the attacker slot position has a next slot possible
            else if(attackerPlayerSlotPosition+counter<noOfSlots)
            {
                //checks if the next slot has a player in it to attack
                if(slotArray[attackerPlayerSlotPosition+counter].playerNo!=-1)
                {
                    //updates the foundClosestPlayer to true
                    foundClosestPlayer=true;
                    
                    //updates the attackedPlayerSlotPosition
                    attackedPlayerSlotPosition=attackerPlayerSlotPosition+counter;
                }
            }
        }
        //increments the counter
        counter++;
    }
    return slotArray[attackedPlayerSlotPosition].playerNo;
}

//method to print the details of the player
void printDetails(player playerArray[],int noOfPlayers)
{
    //loop to print all the player details
    for(int a=0;a<noOfPlayers;a++)
    {
        //prints the player  name, type and life points
        printf("%s\t(%s, %d)\n",playerArray[a].name,playerArray[a].type,playerArray[a].lifePoints);
    }
}

//method to set the capabilities of the players
void setCapabilities(player playerArray[],int playerNumber,int playerTypeChoice)
{
    //updates the life points of the player
    playerArray[playerNumber].lifePoints=100;
    srand((unsigned int)time(NULL));
    
    //checks if the is a Elf
    if(playerTypeChoice==1)
    {
        //magic skills greater than 50 and less than 80
        playerArray[playerNumber].magicSkills=51+(rand()%29);
        
        //smartness greater than 70 and less than 100
        playerArray[playerNumber].smartness=70+(rand()%31);
        
        //luck greater than 60 and less than 100
        playerArray[playerNumber].luck=60+(rand()%41);
        
        //dexerity greater than 0 and less than equal to 100
        playerArray[playerNumber].dexterity=1+(rand()%100);
        
        //strength greater than 0 and less than equal to 50
        playerArray[playerNumber].strength=1+(rand()%50);
        
    }
    
    //checks if the player is a Human
    else if(playerTypeChoice==2)
    {
        //magic skills greater than 0 and less than 100
        playerArray[playerNumber].magicSkills=1+(rand()%100);
        
        //smartness greater than 0 and less than 100
        playerArray[playerNumber].smartness=1+(rand()%100);
        
        //luck greater than 0 and less than 100
        playerArray[playerNumber].luck=1+(rand()%100);
        
        //dexerity greater than 0 and less than 100
        playerArray[playerNumber].dexterity=1+(rand()%100);
        
        //strength greater than 0 and less than 100
        playerArray[playerNumber].strength=1+(rand()%100);
        
        //loop runs until the sum of all the capabilties is less than 300
        while((playerArray[playerNumber].luck+playerArray[playerNumber].magicSkills+playerArray[playerNumber].smartness+playerArray[playerNumber].strength+playerArray[playerNumber].dexterity)>=300)
        {
            //magic skills greater than 0 and less than 100
            playerArray[playerNumber].magicSkills=1+(rand()%100);
            
            //smartness greater than 0 and less than 100
            playerArray[playerNumber].smartness=1+(rand()%100);
            
            //luck greater than 0 and less than 100
            playerArray[playerNumber].luck=1+(rand()%100);
            
            //dexertity greater than 0 and less than 100
            playerArray[playerNumber].dexterity=1+(rand()%100);
            
            //strength greater than 0 and less than 100
            playerArray[playerNumber].strength=1+(rand()%100);
        }
    }
    //checks if player is a Orge
    else if(playerTypeChoice==3)
    {
        //magicSkills equal to zero
        playerArray[playerNumber].magicSkills=0;
        
        //smartness greater than 0 and less than 20
        playerArray[playerNumber].smartness=rand()%21;
        
        //luck such that the sum of luck and smartness is less than 50
        playerArray[playerNumber].luck=(rand()%(50-playerArray[playerNumber].smartness));
        
        //dexerity greater than 80 and less than 100
        playerArray[playerNumber].dexterity=80+(rand()%21);
        
        //strength greater than 80 and less than 100
        playerArray[playerNumber].strength=80+(rand()%21);
    }
    else
    {
        //if the player is a wizard
        
        //magicSkills greater than 80 and less than 100
        playerArray[playerNumber].magicSkills=80+(rand()%21);
        
        //smartness greater than 90 and less than 100
        playerArray[playerNumber].smartness=90+(rand()%11);
        
        //luck greater than 50 and less than 100
        playerArray[playerNumber].luck=50+(rand()%51);
        
        //dexerity greater than 0 and less than 100
        playerArray[playerNumber].dexterity=1+(rand()%100);
        
        //strength greater than 0 and less than 20
        playerArray[playerNumber].strength=1+(rand()%20);
    }
}