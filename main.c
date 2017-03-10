#include<stdio.h>
#include<string.h>
#include<stdlib.h> // Including needed libraries
#include<time.h>
#include<stdbool.h>

typedef struct // To store the infor of the players entered
{
    char name[20];
    char type[20];
    int lifePoints;
    int smartness,strength, magicSkills, luck, dexterity;
}

play;

typedef struct // To store the info of the slots
{
    char slotType[20];
    int slotTypeNo;
    int playNo;
}

slot;

char playTypes[4][20]={"Elf","Human","Ogre","Wizard"}; // For the play types


char slotType[3][20]={"Level Ground","Hill","City"}; // For the slot types


void to_set_Capabilities(play PlayerArray[],int playNumber,int playTypeChoice); // For capabilities of a play

void updateCapabiltitesOnMovingSlot(play PlayerArray[],int playNumber,int slotTypeMovedTo); // To update the capabilities of the play on moving to next slot

void Printing_Final(play PlayerArray[],int noOfplays); // For printing the details after a successful turn

int findClosestplay(int attackerplayNo,slot slotArr[], int noOfSlots); // For finding the closest play to attack

void Attacking(play PlayerArray[],int attackerplayNo,int attackedplayNo); //For implementing the attack

void Gamestarting(play PlayerArray[],slot slotArr[],int noOfplays,int noOfSlots); // To perform the actions


int main() // Main void
{
    int noOfplays;
    
    printf("Enter the number of players: "); // To get the number of players
    scanf("%d",&noOfplays);
    
    while(!(noOfplays>=2 && noOfplays<=6)) // Checking if the number of players are between min and max value allowed
    {
        printf("Minimum number of players is 2 and Maximum number of players is 6\nPlease re-enter your choice: ");
        scanf("%d",&noOfplays);
    }
    
    play PlayerArray[noOfplays]; // Making a play array
    
    for(int a=0;a<noOfplays;a++)// Loop to store the play information
    {
        printf("Please enter the name of players %d: ",(a+1));
        scanf("%s",PlayerArray[a].name);
        
        printf("Please enter the type of players %d\n",(a+1));
        
        printf("Enter 1 for Elf\nEnter 2 for Human\nEnter 3 for Orge\nEnter 4 for Wizard: ");
        
        int playTypeChoice=0;
        scanf("%d",&playTypeChoice); // Taking choice from user
        
        while(!(playTypeChoice>=1 && playTypeChoice<=4)) // Checking if valid input
        {
            printf("Wrong Choice Entered\nRe-enter your choice: ");
            scanf("%d",&playTypeChoice);
            
        }
        
        strcpy(PlayerArray[a].type,playTypes[playTypeChoice-1]); // Using strcpy to copy the string
        
        to_set_Capabilities(PlayerArray, a,playTypeChoice); // Calling function to set capability of players
    }
    
    int noOfSlots;
    
    printf("Please Enter the number of slots (Maximum is 20):"); // Number of slots
    scanf("%d",&noOfSlots);
    
    while(!(noOfSlots>noOfplays && noOfSlots<=20)) // Checking if valid input or not
    {
        printf("Number of slots selected is out of bound\nRe-enter the number of slots: ");
        scanf("%d",&noOfSlots);
    }
    
    slot slotsArray[noOfSlots];
    
    for(int a=0;a<noOfSlots;a++) //initialising the slotTypeNo and playNo to -1
    {
        slotsArray[a].slotTypeNo=-1;
        slotsArray[a].playNo=-1;
    }
    
    srand((unsigned int)time(NULL)); // Using rand to produce rand values
    

    for(int a=0;a<noOfSlots;a++) // For storing the slot type
    {
        slotsArray[a].slotTypeNo=rand()%3;
        strcpy(slotsArray[a].slotType,slotType[slotsArray[a].slotTypeNo]);
    }
    
    for(int a=0;a<noOfplays;a++) // For storing the playNumber
    {
        int randomNumber=rand()%noOfSlots;
        
        if(slotsArray[randomNumber].playNo==-1) // Checking if the slot is not occupied by any play
        {
            slotsArray[randomNumber].playNo=a;
        }
        
        else
        {
            randomNumber=rand()%noOfSlots; // Creating a random value
            
            while(!(slotsArray[randomNumber].playNo==-1))
            {
                randomNumber=rand()%noOfSlots;
            }

            slotsArray[randomNumber].playNo=a; // Stores the play in slot
        }
        
    }
    
    Gamestarting(PlayerArray, slotsArray, noOfplays, noOfSlots); // For starting the game
    
}

void Gamestarting(play PlayerArray[],slot slotArr[],int noOfplays,int noOfSlots) // For performing actions required
{
    for(int a=0;a<noOfplays;a++) // Each play gets one chance
    {
        bool turnSuccessful=false;
    
        printf("%s : Please do your turn\n",PlayerArray[a].name); // play's turn
        
        printf("Enter 1 to attack the closest play\nEnter 2 to move to the previous slot\nEnter 3 to move to the next slot\n");
        
        int moveChoice;
        scanf("%d",&moveChoice); // plays makes his choice
        
        while(!(moveChoice>=1 && moveChoice<=3)) // Checking if the choice was correct
        {
            printf("Wrong Choice Entered\nRe-enter your choice: ");
            scanf("%d",&moveChoice);
        }
        
        int playSlotPosition=0;
        
        for(int j=0;j<noOfSlots;j++) // For finding the play position in the slotArr
        {
            if(slotArr[j].playNo==a)
            {
                playSlotPosition=j;
            }
        }
        
        if(moveChoice==1) // Depending on the choice
        {
            int attackedplayNo=findClosestplay(a,slotArr,noOfSlots); // Calls the function
            
            Attacking(PlayerArray, a, attackedplayNo); // Now attacks the closest play
            
            turnSuccessful=true;
        }
        
        else if(moveChoice==2)
        {
            if(playSlotPosition-1>=0) // Checks if the previous position is possible
            {
                if(slotArr[playSlotPosition-1].playNo==-1) // Checks if the slot has a play
                {
                    updateCapabiltitesOnMovingSlot(PlayerArray, a, slotArr[playSlotPosition-1].slotTypeNo); // Update the capabilities
                    
                    turnSuccessful=true;
                }
                
                else
                {
                    printf("Unable to move to the previous slot\n");
                }
            }
            
            else
            {
                printf("Unable to move to the previous slot\n");
            }
        }
        
        else
        {
            if(playSlotPosition+1<noOfSlots) // If the next slot is even possible
            {
                if(slotArr[playSlotPosition+1].playNo==-1) // If slot is empty
                {
                    updateCapabiltitesOnMovingSlot(PlayerArray, a, slotArr[playSlotPosition+1].slotTypeNo); // Calling the funtion to update the capabilities

                    turnSuccessful=true;
                }
                
                else
                {
                    printf("Unable to move to the next slot\n");
                }
            }

            else
            {
                printf("Unable to move to the next slot\n");
            }
            
        }

        if(turnSuccessful)
        {
            Printing_Final(PlayerArray, noOfplays); // Calling the function to print
        }
        else
        {
            a--; //Then play gets another chance
        }
        
    }
}

int findClosestplay(int attackerplayNo,slot slotArr[], int noOfSlots) // To find the closest play
{

    int attackerplaySlotPosition=0;
    int attackedplaySlotPosition=0;
    
    for(int a=0;a<noOfSlots;a++) // To find position of attacker
    {
        if(slotArr[a].playNo==attackerplayNo)
        {
            attackerplaySlotPosition=a;
        }
    }
    
    bool foundClosestplay=false;
    
    int counter=1;
    
    while(foundClosestplay!=true) // Runs till it finds the closest enemy
    {
        if(attackerplaySlotPosition-counter>=0 || attackerplaySlotPosition+counter<noOfSlots)
        {
            if(attackerplaySlotPosition-counter>=0 && attackerplaySlotPosition+counter<noOfSlots)
            {
                if((slotArr[attackerplaySlotPosition+counter].playNo!=-1)&&(slotArr[attackerplaySlotPosition-counter].playNo!=-1))
                {
                    foundClosestplay=true;
                    
                    int slotChoice;
                    
                    printf("There are 2 closest slots to attack\nPress 1 to attack precious slot\nPress 2 to attack next slot\n");
                    //Asks if play want to attack the previous play or the next play
                    scanf("%d",&slotChoice);
                    
                    while(!(slotChoice==1 || slotChoice==2)) // Checks if choice is correct
                    {
                        printf("Wrong Choice Entered\nRe-enter your choice: ");
                        scanf("%d",&slotChoice);
                    }
                    
                    if(slotChoice==1)
                    {
                        attackedplaySlotPosition=attackerplaySlotPosition-counter;
                    }
                    
                    else
                    {
                        attackedplaySlotPosition=attackerplaySlotPosition+counter;
                    }
                    
                }
                
                else if(slotArr[attackerplaySlotPosition-counter].playNo!=-1)
                // Checking the previous slot
                {
                    foundClosestplay=true;
                    attackedplaySlotPosition=attackerplaySlotPosition-counter;
                    //updates the attackedplaySlotPosition
                }
                
                else if(slotArr[attackerplaySlotPosition+counter].playNo!=-1)
                {
                    foundClosestplay=true;
                    attackedplaySlotPosition=attackerplaySlotPosition+counter;
                    //updates the attackedplaySlotPosition
                }
            }
            
            else if(attackerplaySlotPosition-counter>=0)
            {
                
                if(slotArr[attackerplaySlotPosition-counter].playNo!=-1)
                {
                    foundClosestplay=true;
                    attackedplaySlotPosition=attackerplaySlotPosition-counter;
                }
            }
            
            else if(attackerplaySlotPosition+counter<noOfSlots)
            {
                if(slotArr[attackerplaySlotPosition+counter].playNo!=-1)
                {
                    foundClosestplay=true; // Updates it to be true

                    attackedplaySlotPosition=attackerplaySlotPosition+counter;
                }
            }
        }
        
        counter++; // Incrementing the counter
    }
    
    return slotArr[attackedplaySlotPosition].playNo; // Exiting the void
}

void Printing_Final(play PlayerArray[],int noOfplays) // For printing
{
    for(int a=0;a<noOfplays;a++)
    {
        printf("%s\t(%s, %d)\n",PlayerArray[a].name,PlayerArray[a].type,PlayerArray[a].lifePoints);
        // For printing the player's name, type and life points
    }
}

void to_set_Capabilities(play PlayerArray[],int playNumber,int playTypeChoice)
{
    PlayerArray[playNumber].lifePoints=100; // Initializes life-points to 100
    srand((unsigned int)time(NULL)); // Using srand to successfully generate random

    if(playTypeChoice==1) // If elf
    {
        PlayerArray[playNumber].magicSkills=51+(rand()%29); // Creates random between 50 and 80
        
        PlayerArray[playNumber].smartness=70+(rand()%31); // Creates random between 70 and 100
        
        PlayerArray[playNumber].luck=60+(rand()%41); // Creates random between 60 and 100
        
        PlayerArray[playNumber].dexterity=1+(rand()%100); // Creates random between 0 and 100
        
        PlayerArray[playNumber].strength=1+(rand()%50); // Creates random between 0 and 50
        
    }
    
    else if(playTypeChoice==2) // If human
    {
        PlayerArray[playNumber].magicSkills=1+(rand()%100); // Creates random between 0 and 100
        
        PlayerArray[playNumber].smartness=1+(rand()%100); // Creates random between 0 and 100
        
        PlayerArray[playNumber].luck=1+(rand()%100); // Creates random between 0 and 1000

        PlayerArray[playNumber].dexterity=1+(rand()%100); // Creates random between 0 and 100

        PlayerArray[playNumber].strength=1+(rand()%100); // Creates random between 0 and 100
        

        while((PlayerArray[playNumber].luck+PlayerArray[playNumber].magicSkills+PlayerArray[playNumber].smartness+PlayerArray[playNumber].strength+PlayerArray[playNumber].dexterity)>=300)
        {
            PlayerArray[playNumber].magicSkills=1+(rand()%100); // Creates random between 0 and 100
            
            PlayerArray[playNumber].smartness=1+(rand()%100); // Creates random between 0 and 100
            
            PlayerArray[playNumber].luck=1+(rand()%100); // Creates random between 0 and 100
            
            PlayerArray[playNumber].dexterity=1+(rand()%100); // Creates random between 0 and 100
            
            PlayerArray[playNumber].strength=1+(rand()%100); // Creates random between 0 and 100
        }
    }

    else if(playTypeChoice==3) // If orge
    {
        PlayerArray[playNumber].magicSkills=0; // Initializing it to 0
        

        PlayerArray[playNumber].smartness=rand()%21; // Creates random between 0 and 20

        PlayerArray[playNumber].luck=(rand()%(50-PlayerArray[playNumber].smartness)); // Creates random between 0 and 50

        PlayerArray[playNumber].dexterity=80+(rand()%21); // Creates random between 80 and 100

        PlayerArray[playNumber].strength=80+(rand()%21); // Creates random between 80 and 100
    }
    
    else // If wizard
    {
        PlayerArray[playNumber].magicSkills=80+(rand()%21); // Creates random between 80 and 100

        PlayerArray[playNumber].smartness=90+(rand()%11); // Creates random between 90 and 100

        PlayerArray[playNumber].luck=50+(rand()%51); // Creates random between 50 and 100

        PlayerArray[playNumber].dexterity=1+(rand()%100); // Creates random between 0 and 100

        PlayerArray[playNumber].strength=1+(rand()%20); // Creates random between 0 and 20
    }
}

void updateCapabiltitesOnMovingSlot(play PlayerArray[],int playNumber,int slotTypeMovedTo)
{
    if(slotTypeMovedTo==1) // If player moved to a hill
    {
        if(PlayerArray[playNumber].dexterity<50) // If dexterity is less than 50
        {
            PlayerArray[playNumber].strength-=10;
        }
        
        else if(PlayerArray[playNumber].dexterity>=60) // If dexerity is greater than 60
        {
            PlayerArray[playNumber].strength+=10;
        }
    }
    
    else if(slotTypeMovedTo==2) // If player moved to a city
    {
        if(PlayerArray[playNumber].smartness>60) // If smartness is greater than 60
        {
            PlayerArray[playNumber].magicSkills+=10;
        }
        
        else if(PlayerArray[playNumber].smartness<=50) // If smartness is less than 50
        {
            PlayerArray[playNumber].dexterity-=10;
        }
        
    }
}

void Attacking(play PlayerArray[],int attackerplayNo,int attackedplayNo) // For attacking
{
   
    if(PlayerArray[attackedplayNo].strength<=70) // If the attacked player strength is less than 70
    {
        PlayerArray[attackedplayNo].lifePoints=PlayerArray[attackedplayNo].lifePoints-(0.5*PlayerArray[attackedplayNo].strength);
    }
    
    else //updates the lifePoints of the attacker play
    {
        PlayerArray[attackerplayNo].lifePoints=PlayerArray[attackerplayNo].lifePoints-(0.3*PlayerArray[attackedplayNo].strength);
    }
}
