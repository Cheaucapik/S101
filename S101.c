#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#pragma warning (disable : 4996)

enum{MAX_ETU = 100, NOM_MAX = 30, MAX_ABS = 100}; //pour éviter les nb magiques

unsigned int id_etu = 1, *pid_etu = &id_etu, id_abs = 1, *pid_absence = &id_abs; //permet l'incrémentation sans la modification des valeurs dans la fonction

typedef struct{
    char nom_etu[NOM_MAX];
    unsigned int num_grp;
    unsigned int id_etu_tab;
    unsigned int num_grp_copie; //utile dans le tri à bulles
}Etudiant;

typedef struct{
    unsigned int num_jour;
    char demi_journee[3];
    unsigned int id_abs_tab;
    unsigned int total_abs;
}Absence;

Absence tab_absence[MAX_ABS]; //tableau pour contenir toutes les données de la commande absence
Etudiant tab_etudiant[MAX_ETU]; //tableau pour contenir toutes les données de la commande etudiant

//fonctions prototypes
int execution(char *commande); //exécute une commande par comparaison
int aide(void); //Commande supplémentaire affichant toutes les commandes
int inscription(char nom_etu[NOM_MAX], unsigned int num_grp); //C1 : inscription <nom etu> <nom grp> → inscription de l'étudiant
int absence(int temp_id_etu, int num_jour, char demi_journee[3]); //C2 : absence : <id etu> <Num jour> <am/pm> → enregistrer une absence
int etudiants(int num_jour_courant); //C3 : etudiants <Num jour courant> → liste des etudiants
int compareEtudiant(const void *a, const void *b); //qsort

int main(int argc, char *argv[]){
    char commande[50];

    printf("Entrer votre commande :\n"); //entrée de la commande
    while(1){ //boucle tant que l'on n'arrête pas le programme
        printf("> "); //esthétique
            if (argc > 1) {
                strcpy(commande, argv[1]); //si l'entrée est nulle
            } else {
                fgets(commande, sizeof(commande), stdin);
                commande[strcspn(commande, "\n")] = 0; //évite de prendre en compte la remise à la ligne
                execution(commande); //execution de la commande
            }
    }
    return 0;
}

int execution(char *commande){ //exécute une commande par comparaison
        char nom_etu[NOM_MAX];
        unsigned int num_grp;
        unsigned int num_jour = 1;
        char demi_journee[3];
        unsigned temp_id_etu;
        unsigned int num_jour_courant = 1;

    if(strcmp(commande, "exit") == 0){ //C0 : exit
        exit(0); //arrêt du programme
    }
    else if(sscanf(commande, "inscription %30s %u", nom_etu, &num_grp) == 2){ //C1 : inscription
        inscription(nom_etu, num_grp);
    }
    else if (sscanf(commande, "absence %u %u %2s", &temp_id_etu, &num_jour, demi_journee) == 3) { //C2 : absence
        absence(temp_id_etu, num_jour, demi_journee);
    }
    else if(sscanf(commande, "etudiants %u", &num_jour_courant) == 1){ //C1 : inscription
        etudiants(num_jour_courant);
    }
    else if (strcmp(commande, "help") == 0) { //Cpersonnalisée : help
        aide();
    }
    else{
        printf("Commande inconnue veuillez reessayer.\n");
        return 0;
    }
    return 0;
}

int inscription(char nom_etu[NOM_MAX], unsigned int num_grp){ //C1 : inscription <nom etu> <nom grp> → inscription de l'étudiant
    for(int i = 1; i < id_etu; ++i){
        if((strcasecmp(nom_etu, tab_etudiant[i].nom_etu) == 0) && (num_grp == tab_etudiant[i].num_grp)){ //évite les doublons
            printf("Nom incorrect\n");
            return 0;
        }
    }
    printf("Inscription enregistree (%u)\n", id_etu);
    strcpy(tab_etudiant[id_etu].nom_etu, nom_etu); //copie le nom de l'étudiant dans un tableau (meme position que tout le reste) 
    tab_etudiant[id_etu].num_grp = num_grp; //copie le groupe de l'étudiant dans un tableau (meme position que tout le reste)
    tab_etudiant[id_etu].id_etu_tab = id_etu; //copie l'id de l'etudiant dans un tableau pour les autres commandes
    ++id_etu; //incrémente l'id de l'étudiant à chaque fois
    return 0;
}

int absence(int temp_id_etu, int num_jour, char demi_journee[3]){ //C2 : absence : <id etu> <Num jour> <am/pm>
    int id_etu_exite = 0;
    for(int i = 1; i < id_etu; ++i){ //evite que l'on enregistre une absence à l'id d'un étudiant inexistant
        if(temp_id_etu == tab_etudiant[i].id_etu_tab){ 
            id_etu_exite =  1;
            break;
        }
    }
    if(!id_etu_exite){
        printf("Identifiant incorrect\n");
        return 0;
    }
    if(num_jour > 40 || num_jour < 1){ //1 <= num_jour <=40
        printf("Date incorrecte\n");
        return 0;
    }
    else if((strcmp(demi_journee, "am") != 0) && strcmp(demi_journee, "pm") != 0){ //demi_journee soit égale à pm soit à am
        printf("Demi-journee incorrecte\n");
        return 0;
    }
    for(int i = 1; i < id_abs; ++i){
        if(temp_id_etu == tab_absence[i].id_abs_tab && num_jour == tab_absence[i].num_jour && strcmp(demi_journee, tab_absence[i].demi_journee) == 0){ //évite les doublons d'absence
            printf("Absence deja connue\n");
            return 0;
        }
    }
    printf("Absence enregistree [%u]\n", id_abs);
    tab_absence[id_abs].id_abs_tab = temp_id_etu; //copie l'id d'absence de l'étudiant dans un tableau (meme position que tout le reste)
    tab_absence[id_abs].num_jour = num_jour; //copie le num_jour de l'absence de l'étudiant dans un tableau (meme position que tout le reste)
    strcpy(tab_absence[id_abs].demi_journee, demi_journee); //copie la demi_journee d'absence de l'étudiant dans un tableau (meme position que tout le reste)
    ++id_abs; //incrémente l'id de l'absence

    for(int i = 1; i < id_etu; ++i){
        if(temp_id_etu == tab_etudiant[i].id_etu_tab){
            tab_absence[temp_id_etu].total_abs++; //incrémente pour chaque étudiant le total d'absence
        }
    }
    return 0;
}

int etudiants(int num_jour_courant){
    qsort();
    if(num_jour_courant < 1){
        printf("Date incorrecte\n");
        return 0;
    }
    for(int i = 1; i < id_etu; ++i){
        printf("(%u) %-30s %10u %5u\n", tab_etudiant[i].id_etu_tab, tab_etudiant[i].nom_etu, tab_etudiant[i].num_grp, tab_absence[i].total_abs);
    }
    return 0;
}

int compareEtudiant(const void *a, const void *b) {

}

int aide(void){ //Commande supplémentaire affichant toutes les commandes
    printf("****************************************************************************************************\n"); //esthétique
    printf("%60s", "Commandes disponibles\n");
    printf("****************************************************************************************************\n"); //esthétique
    printf("%-50s %50s", "help", "affiche la liste des commandes disponibles\n"); //Cpersonnalisée
    printf("%-50s %50s", "exit", "arrete le programme\n"); //C0
    printf("%-50s %50s", "inscription <nom etu> <nom grp>", "inscription de l'etudiant\n"); //C1
    printf("%-50s %50s", "absence <id etu> <Num jour> <am/pm>", "enregistrer une absence\n"); //C2
    printf("%-50s %50s", "etudiants <Num jour courant>", "liste des etudiants\n"); //C3
    printf("%-50s %50s", "justificatif <Id absence>", "Depot de justificatif\n"); //C4
    printf("%-50s %50s", "validations", "liste des absences a valider\n"); //C5
    printf("%-50s %50s", "validation <id absence> <ok/ko>", "validation d'une justification d'absence\n"); //C6
    printf("%-50s %50s", "etudiant <id etu> <Num jour courant>", "situation d'un etudiant\n"); //C7
    printf("%-50s %50s", "defaillants <Num jour courant>", "liste des absences\n"); //C8
}