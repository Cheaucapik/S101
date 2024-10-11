#include <stdio.h>
#include <string.h>

int main(){
    char prenom[30];
    printf("qui pue du cul ? : ");
    scanf("%s", prenom);
    if((strcasecmp(prenom, "Chirine" || "Malak") == 0)){
        printf("Bien joue ! ");
    }
    else{
        printf("Faux !");
    }
}