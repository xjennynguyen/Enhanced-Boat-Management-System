#include <ctype.h> 

#include <stdio.h> 

#include <stdlib.h> 

#include <string.h> 

#include <string.h> 

 

#define SLIP_RATE 12.50 

#define LAND_RATE 14.00 

#define TRAILOR_RATE 25.00 

#define STORAGE_RATE 11.20 

 

// define boat types. 

typedef enum { slip, land, trailor, storage, no_place } PlaceType; 

 

// different data types stored in same memory location 

typedef union { 

  int slipNumber; 

  char bayLetter; 

  char trailerLicense[10]; 

  int storageSpace; 

} ExtraInfo; 

 

// hold data for a boat 

typedef struct { 

  char name[128]; // name of boat 

  int length;     // length in feet 

  PlaceType type; // type of boat 

  ExtraInfo details; //reference the union details. 

  double amountOwed; 

} Boat; 

 

// convert a string to a place 

PlaceType StringToPlaceType(char * PlaceString) { 

 

    if (!strcasecmp(PlaceString,"slip")) { 

        return(slip); 

    } 

    if (!strcasecmp(PlaceString,"land")) { 

        return(land); 

    } 

    if (!strcasecmp(PlaceString,"trailor")) { 

        return(trailor); 

    } 

    if (!strcasecmp(PlaceString,"storage")) { 

        return(storage); 

    } 

    return(no_place); 

} 

 

// sort an array alphabetically 

int comparator(const void *boat1, const void *boat2) { 

  Boat *boat1_ptr = *(Boat **)boat1; 

  Boat *boat2_ptr = *(Boat **)boat2; 

 

  char* boatName1 = boat1_ptr->name; 

  char* boatName2 = boat2_ptr->name; 

  

  return strcmp(boatName1, boatName2); 

} 

 

// load data from the file provided in the command line. When program closed it should store data in that same file. 

void loadBoatData(char *fileName, Boat **boatArray, int *numBoats) { 

   

  char line[120]; 

  

  if (fileName == NULL){ 

    printf("File not entered. Please enter a file name.\n"); 

    return; 

  } 

 

  FILE *boatFile = fopen(fileName, "r"); 

 

  // check if file is opened and if null, give error message. 

  if (boatFile == NULL) { 

    printf("Error opening the file.\n"); 

    return; 

  } 

  

  // allocate memory for boat 

  while (fgets(line, sizeof(line),boatFile) != NULL) { 

    Boat *boat = (Boat *)malloc(sizeof(Boat)); 

 

    // scan for the name and copy it into the boat->name 

    char *name = strtok(line, ","); 

    strcpy(boat->name, name); 

 

    //scan for length, continue after the NULL, and convert length to int 

    char *length = strtok(NULL, ","); 

    boat->length = atoi(length); 

 

    // scan for type, convert string to a place using string to place type function. 

 

    char *type = strtok(NULL, ",");  

    boat->type = StringToPlaceType(type); 

 

    char* extraInfo = strtok(NULL, ","); 

    

    switch(boat->type){ 

      case slip: 

        boat->details.slipNumber = atoi(extraInfo); 

        break; 

      case land: 

        boat->details.bayLetter = toupper(extraInfo[0]); 

      break; 

      case trailor: 

        strcpy(boat->details.trailerLicense, extraInfo); 

      break; 

      case storage: 

        boat->details.storageSpace = atoi(extraInfo); 

      break; 

      case no_place: 

        printf("No place data for boat \n"); 

      break; 

 

      default: 

      printf("Boat type invalid.\n"); 

      break; 

    } 

    // scan for amount owed 

    char *amountOwed = strtok(NULL, ","); 

    boat->amountOwed = atof(amountOwed);       

  

    // add the boat to array. 

    boatArray[*numBoats] = boat; 

 

    // increment pointer to indicate new boat added. 

    (*numBoats)++; 

  } 

  fclose(boatFile); 

  

  //sort list 

  qsort(boatArray, *numBoats, sizeof(Boat *), comparator); 

 

} 

 

char * PlaceToString(PlaceType Place) { 

 

      switch (Place) { 

          case slip: 

              return("slip"); 

          case land: 

              return("land"); 

          case trailor: 

              return("trailor"); 

          case storage: 

              return("storage"); 

          case no_place: 

              return("no_place"); 

          default: 

              printf("How the faaark did I get here?\n"); 

              exit(EXIT_FAILURE); 

              break; 

      } 

} 

 

void printBoatInventory(Boat **boatArray, int numBoats) { 

  for (int i=0; i < numBoats; i++){ 

    printf("%-22s %3d' %10s ", boatArray[i]->name, boatArray[i]->length, PlaceToString(boatArray[i]->type)); 

    switch(boatArray[i]->type){ 

      case slip: 

        printf("  # %3d", boatArray[i]->details.slipNumber); 

      break; 

      case land: 

        printf("%7c", boatArray[i]->details.bayLetter); 

      break; 

      case trailor: 

        printf("%7s", boatArray[i]->details.trailerLicense); 

        break; 

      case storage: 

      printf("  # %3d", boatArray[i]->details.storageSpace); 

      break; 

      case no_place: 

      printf("no_place"); 

      break; 

      default: 

      printf("Boat type invalid.\n"); 

      break; 

    } 

    printf(" Owes $%7.2lf\n", boatArray[i]->amountOwed);     

  } 

} 

 

void addBoat(Boat **boatArray, int *numBoats) { 

  int maxBoats = 120; 

    

  // if the number is over max, write error. 

  if (*numBoats == maxBoats) { 

    printf("Boat storage at capacity. \n"); 

    return; 

  } 

    

  // malloc to allocate for the newBoat. 

  Boat *newBoat = (Boat *)malloc(sizeof(Boat)); 

  if (newBoat == NULL) { 

    printf("Cannot add boat. \n"); 

    return; 

  } 

 

  // read details for the new boat from the user. 

  char boatInput[256]; 

  printf("Please enter the boat data in CSV format                : "); 

  getchar(); 

  if (fgets(boatInput, sizeof(boatInput), stdin) == NULL) { 

    printf("Invalid data. Please re-enter. \n"); 

    return; 

  } 

 

    //use strtok to parse input and scan for name to copy it into the boat->name 

    char *name = strtok(boatInput, ","); 

    strcpy(newBoat->name, name); 

 

    //scan for the length, continue after the NULL 

    char *length = strtok(NULL, ","); 

    newBoat->length = atoi(length); 

 

    //scan for the type, convert string to place using string to place type function 

    char *type = strtok(NULL, ","); 

    newBoat->type = StringToPlaceType(type); 

 

    char* extraInfo = strtok(NULL, ","); 

 

    switch(newBoat->type){ 

      case slip: 

        newBoat->details.slipNumber = atoi(extraInfo); 

        break; 

      case land: 

        newBoat->details.bayLetter = toupper(extraInfo[0]); 

      break; 

      case trailor: 

        strcpy(newBoat->details.trailerLicense, extraInfo); 

      break; 

      case storage: 

        newBoat->details.storageSpace = atoi(extraInfo); 

      break; 

      case no_place: 

        printf("No place data for boat \n"); 

      break; 

 

      default: 

      printf("Boat type invalid.\n"); 

      break; 

    } 

 

    //scan for the amount owed, add new boat to array and increment boat count 

    char *amountOwed = strtok(NULL, ","); 

    newBoat->amountOwed = atof(amountOwed); 

      boatArray[*numBoats] = newBoat; 

      (*numBoats)++; 

 

  qsort(boatArray, *numBoats, sizeof(Boat *), comparator); 

  

} 

 

void removeBoat(Boat **boatArray, int *numBoats) { 

   char boatToDelete[128]; 

  

  // prompt the user for name of boat, set variable to determine if found 

  printf("Please enter the boatName                : "); 

  scanf(" %[^\n]s", boatToDelete); 

  int found = 0; 

  

  //if in the array, remove it, shift other cells 

  for (int i =0; i< *numBoats; i++){ 

    if (strcasecmp(boatToDelete, boatArray[i]->name) == 0){ 

      free(boatArray[i]); 

      for (int j = i; j < *numBoats - 1; j++) { 

        boatArray[j] = boatArray[j + 1]; 

      } 

      (*numBoats)--; 

      found = 1; 

      break; 

    } 

  } 

  if (found != 1){ 

    printf("No boat with that name."); 

  } 

  

} 

  

void acceptPayment(Boat **boatArray, int numBoats) { 

  char boatToPay[127]; 

  double payment; 

  int found = 0; 

 

  //enter boat name, look for it in the array. 

  printf("Please enter the boat name                : "); 

  scanf(" %[^\n]s", boatToPay); 

  for (int i =0; i< numBoats; i++){ 

    if (strcasecmp(boatToPay, boatArray[i]->name) == 0){ 

 

      //if found enter payment and change found var. 

      printf("Please enter the amount to be paid                : "); 

      scanf(" %lf", &payment); 

 

      //if payment is more than amount owed, write error. if less, subract payment owed 

      if (payment > boatArray[i]->amountOwed){ 

        printf("\nThat is more than the amount owed, $%.2lf\n", boatArray[i]->amountOwed); 

      } else{ 

        boatArray[i]->amountOwed = boatArray[i]->amountOwed - payment; 

      } 

      found = 1; 

    } 

  } 

//if boat not found return error. 

  if (found != 1){ 

    printf("No boat with that name. \n"); 

  } 

  

} 

 

void updateAmountOwed(Boat **boatArray, int numBoats) { 

//loop through array, if less then subract payment from owed, multiply length by rate based on type 

  for (int i =0; i< numBoats; i++){ 

    int length = boatArray[i]->length; 

    switch (boatArray[i]->type){ 

      case slip: 

        boatArray[i]->amountOwed = boatArray[i]->amountOwed + (length * SLIP_RATE); 

      break; 

      case land: 

        boatArray[i]->amountOwed = boatArray[i]->amountOwed + (length * LAND_RATE); 

      break; 

      case trailor: 

        boatArray[i]->amountOwed = boatArray[i]->amountOwed + (length * TRAILOR_RATE); 

      break; 

      case storage: 

        boatArray[i]->amountOwed = boatArray[i]->amountOwed + (length * STORAGE_RATE); 

      break; 

      case no_place: 

      default: 

        break; 

  } 

  

  } 

} 

 

void saveBoatData(char *fileName, Boat **boatArray,int numBoats){ 

  // check if the file name is not null. 

  if (fileName == NULL) { 

      printf("Please enter a file name. \n"); 

      return; 

  } 

  FILE *boatFile = fopen(fileName, "w"); 

  if (boatFile == NULL) { 

      printf("Error opening the file. \n"); 

      return; 

  } 

  for (int i = 0; i < numBoats; i++) { 

      Boat *boat = boatArray[i]; 

      fprintf(boatFile, "%s,%d,%s,", boat->name, boat->length, PlaceToString(boat->type)); 

 

      // based on boat type, write extra info. 

      switch (boat->type) { 

          case slip: 

              fprintf(boatFile, "%d,", boat->details.slipNumber); 

              break; 

          case land: 

              fprintf(boatFile, "%c,", boat->details.bayLetter); 

              break; 

          case trailor: 

              fprintf(boatFile, "%s,", boat->details.trailerLicense); 

              break; 

          case storage: 

              fprintf(boatFile, "%d,", boat->details.storageSpace); 

              break; 

          case no_place: 

              fprintf(boatFile, "no_place,"); 

              break; 

        default: 

            fprintf(boatFile, "Error. Invalid boat type."); 

            break; 

      } 

        fprintf(boatFile, "%.2lf\n", boat->amountOwed); 

    } 

    fclose(boatFile); 

} 

// to handle command line input 

int main(int argc, char *argv[]){ 

  

  // check command line for correct number of prompts. must have 2 (prog name + file) 

if (argc != 2) { 

  printf("Error, incorrect number of arguments."); 

  return(EXIT_FAILURE); 

} 

  

  Boat *boatArray[120]; 

  int numBoats = 0; 

 

  

  printf("Welcome to the Boat Management System\n"); 

  printf("-------------------------------------\n"); 

 

  loadBoatData(argv[1], boatArray, &numBoats); 

 

  char userOption; 

 

  do { 

    printf("\n(I)nventory, (A)dd, (R)emove, (P)ayment, (M)onth, e(X)it : "); 

    scanf(" %c", &userOption); 

    

    if(isalpha(userOption)){ 

      userOption = tolower(userOption); //could jsut have this line. 

    } else{ 

      printf("Please enter an a valid option.\n"); 

    } 

   

    switch(userOption){ 

      case 'i' : 

        printBoatInventory(boatArray, numBoats); 

        break; 

      case 'a': 

      addBoat(boatArray, &numBoats); 

      break; 

      case 'r': 

      removeBoat(boatArray,&numBoats); 

      break; 

      case 'p': 

      acceptPayment(boatArray, numBoats); 

      break; 

      case 'm': 

      updateAmountOwed(boatArray, numBoats); 

      break; 

      case 'x': 

      printf("Exiting the Boat Management System\n"); 

        break; 

      default: 

        printf("Invalid option %c\n", toupper(userOption)); 

      break; 

   } 

  } while (userOption != 'x'); 

 

  saveBoatData(argv[1], boatArray, numBoats); 

 

  for(int i=0; i<numBoats; i++){ 

    free(boatArray[i]); 

  } 

  

  return(EXIT_SUCCESS); 

} 

