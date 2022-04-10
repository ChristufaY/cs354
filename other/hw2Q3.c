#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char firstname[22];
	char lastname[22];
	char position[22];
} Teammate;

typedef struct {
	int size;
	char name[22];
	Teammate roster[33];
} Team;

int main(void) {
	//Team *team = malloc(sizeof(Team));
	//code goes here
	//*(team.roster).lastname = "Yang"; doesn't work
	//(*team.roster).lastname = "Yang"; doesn't work
	//team->roster[0].lastname = "Yang"; doesn't work
	//team.roster[0].lastname = "Yang";
	//(*(*team).roster).lastname = "Yang"; 

	//strcpy(team->roster[0].lastname, "Yang"); works
	//strcpy((*(*team).roster).lastname, "Yang"); works
	//strcpy(team->roster->lastname, "Yang");
	//strcpy(team->roster.lastname, "Yang");
	//printf("last name 1: %s\n", *(team.roster).lastname);
	//printf("last name 2: %s\n", (*team.roster).lastname);
	//printf("last name 3: %s\n", team->roster[0].lastname);
	//printf("last name 4: %s\n", team.roster[0].lastname);
	//printf("last name 5: %s\n", (*(*team).roster).lastname);
	Team team;
//	strcpy(team->roster->lastname, "Yang");
	strcpy(team.roster->lastname, "Yang");
	printf("last name 1: %s", (*team->roster).lastname);
	printf("last name 2: %s", (*team.roster).lastname);
	printf("last name 3: %s", (*(*team).roster).lastname);
	printf("last name 4: %s", team.roster[0].lastname);
	printf("last name 5: %s", team->roster[0].lastname);


return 0;
}
