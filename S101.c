#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning (disable : 4996 6031)

enum{MAX_ETU = 100, NOM_MAX = 30, MAX_ABS = 100}; //pour éviter les nb magiques

typedef struct{
    char nom_etu[NOM_MAX];
    unsigned int num_grp;
    unsigned int id_etu_tab;
}Etudiant; //données concernant l'étudiant

typedef struct{
    unsigned int num_jour;
    char demi_journee[3];
    unsigned int id_abs_tab;
}Absence; //données concernant les absences d'un étudiant

typedef struct{
    unsigned int id_etu, id_abs; //permet l'incrémentation sans la modification des valeurs dans la fonction
    Absence tab_absence[MAX_ABS]; //tableau pour contenir toutes les données de la commande absence
    Etudiant tab_etudiant[MAX_ETU]; //tableau pour contenir toutes les données de la commande etudiant
}Donnees; //struct évitant les variables globales

//fonctions prototypes
int execution(char *commande, Donnees *donnees); //exécute une commande par comparaison
void help(void); //Commande supplémentaire affichant toutes les commandes
int inscription(char nom_etu[NOM_MAX], unsigned int num_grp, Donnees *donnees); //C1 : inscription <nom etu> <nom grp> → inscription de l'étudiant
int absence(int temp_id_etu, int num_jour, char demi_journee[3], Donnees *donnees); //C2 : absence : <id etu> <Num jour> <am/pm> → enregistrer une absence
int etudiants(int num_jour_courant, Donnees *donnees); //C3 : etudiants <Num jour courant> → liste des etudiants
int compare(const void *a, const void *b); //qsort

int main(int argc, char *argv[]){
    char commande[50];
    Donnees donnees = {1, 1}; //initialisation de id_etu et de id_abs
    printf("Entrer votre commande :\n"); //entrée de la commande
    while(1){ //boucle tant que l'on n'arrête pas le programme
            if (argc > 1) {
                strcpy(commande, argv[1]); //si l'entrée n'est pas nulle
            } else {
                fgets(commande, sizeof(commande), stdin);
                commande[strcspn(commande, "\n")] = 0; //évite de prendre en compte la remise à la ligne du fgets
                execution(commande, &donnees); //execution de la commande
            }
    }
    return 0;
}

int execution(char *commande, Donnees *donnees){ //exécute une commande par comparaison
        char nom_etu[NOM_MAX];
        unsigned int num_grp;
        unsigned int num_jour = 1;
        char demi_journee[3];
        unsigned temp_id_etu; //utile dans la commande absence, désigne l'entrée de l'utilisateur
        unsigned int num_jour_courant = 1;

    if(strcmp(commande, "exit") == 0){ //C0 : exit
        exit(0); //arrêt du programme
    }
    else if(sscanf(commande, "inscription %29s %u", nom_etu, &num_grp) == 2){ //C1 : inscription
        inscription(nom_etu, num_grp, donnees);
    }
    else if (sscanf(commande, "absence %u %u %2s", &temp_id_etu, &num_jour, demi_journee) == 3) { //C2 : absence
        absence(temp_id_etu, num_jour, demi_journee, donnees);
    }
    else if(sscanf(commande, "etudiants %u", &num_jour_courant) == 1){ //C1 : inscription
        etudiants(num_jour_courant, donnees);
    }
    else if (strcmp(commande, "help") == 0) { //Cpersonnalisée : help
        help();
    }
    else{ //Cpersonnalisée : help
        printf("Commande inconnue, veuillez reessayer.\n");
        return 0;
    }
    return 0;
}

int inscription(char nom_etu[NOM_MAX], unsigned int num_grp, Donnees *donnees){ //C1 : inscription <nom etu> <nom grp> → inscription de l'étudiant
    for(int i = 1; i < donnees->id_etu; ++i){
        if((strcasecmp(nom_etu, donnees->tab_etudiant[i].nom_etu) == 0) && (num_grp == donnees->tab_etudiant[i].num_grp)){ //évite les doublons
            printf("Nom incorrect\n");
            return 0;
        }
    }
    printf("Inscription enregistree (%u)\n", donnees->id_etu);
    strcpy(donnees->tab_etudiant[donnees->id_etu].nom_etu, nom_etu); //le nom de l'tudiant est copié dans un tableau se trouvant à la même place que l'id de l'étudiant pour pouvoir attribuer ce nom à un étudiant(id)
    donnees->tab_etudiant[donnees->id_etu].num_grp = num_grp; //le numéro de groupe est copié dans un tableau se trouvant à la même place que l'id de l'étudiant pour pouvoir attribuer ce numéro à un étudaint(id)
    donnees->tab_etudiant[donnees->id_etu].id_etu_tab = donnees->id_etu; //l'id de l'étudiant est copié dans un tableau à la même place que son id. Ainsi on a l'id 1 qui se trouve à la position 1 du tableau, permettant une gestion plus facile des données
    ++donnees->id_etu; //incrémente l'id de l'étudiant à chaque nouvelle inscription
    return 0;
}

int absence(int temp_id_etu, int num_jour, char demi_journee[3], Donnees *donnees){ //C2 : absence : <id etu> <Num jour> <am/pm>
    int id_etu_exite = 0;
    for(int i = 1; i < donnees->id_etu; ++i){ //evite que l'on enregistre une absence à l'id d'un étudiant inexistant
        if(temp_id_etu == donnees->tab_etudiant[i].id_etu_tab){ 
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
    for(int i = 1; i < donnees->id_abs; ++i){
        if(temp_id_etu == donnees->tab_absence[i].id_abs_tab && num_jour == donnees->tab_absence[i].num_jour && strcmp(demi_journee, donnees->tab_absence[i].demi_journee) == 0){ //évite les doublons d'absence
            printf("Absence deja connue\n");
            return 0;
        }
    }
    printf("Absence enregistree [%u]\n", donnees->id_abs);
    donnees->tab_absence[donnees->id_abs].id_abs_tab = temp_id_etu; //l'id de l'absence est copiée dans un tableau où elle dépend de l'id de l'étudiant pour pouvoir attribuer une absence à un étudiant, tout cela à la position id_abs
    donnees->tab_absence[donnees->id_abs].num_jour = num_jour; //le numéro de jour est copié dans un tableau se trouvant à la même place que l'absence pour pouvoir attribuer ce numéro à l'absence
    strcpy(donnees->tab_absence[donnees->id_abs].demi_journee, demi_journee); //la demi-journée est copiée dans un tableau se trouvant à la même place que l'absence pour pouvoir attribuer cette demi-journée à l'absence
    ++donnees->id_abs; //incrémente l'id de l'absence à chaque nouvelle absence

    return 0;
}

int etudiants(int num_jour_courant, Donnees *donnees){
    qsort(donnees->tab_etudiant + 1, donnees->id_etu - 1, sizeof(Etudiant), compare); //trie le tableau d'étudiants dans un ordre croissant (par groupe), puis par ordre alphabétique
    if(num_jour_courant < 1){
        printf("Date incorrecte\n");
        return 0;
    }

    for(int i = 1; i < donnees->id_etu; ++i){
        unsigned int total_abs = 0; //le total d'absences revient à 0 pour chaque nouveau étudiant
        for(int j = 1; j < donnees->id_abs; ++j){
            if(donnees->tab_absence[j].id_abs_tab == donnees->tab_etudiant[i].id_etu_tab &&
               donnees->tab_absence[j].num_jour <= num_jour_courant){
                total_abs++; //incrémente le total d'absence si on trouve une absence (pour l'étudiant) pour le num de jour courant supérieur ou égal au num de jour de l'absence
            }
        }
        printf("(%u) %-30s %10u %5u\n", donnees->tab_etudiant[i].id_etu_tab, donnees->tab_etudiant[i].nom_etu, donnees->tab_etudiant[i].num_grp, total_abs); //printf pour chaque étudiant, ligne par ligne
    }
    return 0;
}

int compare(const void *a, const void *b){
    Etudiant *etudiantA = (Etudiant *)a;
    Etudiant *etudiantB = (Etudiant *)b;
    if (etudiantA->num_grp < etudiantB->num_grp) { //si le num_grp de l'étudiant A < num_grp de l'étudiant
        return -1;
    } //num_grp de l'étudiant A est bien inférieur à celui de l'étudiant B
    else if(etudiantA->num_grp > etudiantB->num_grp) { //si le num_grp de l'étudiant A < num_grp de l'étudiant
        return 1; //num_grp de l'étudiant A est bien inférieur à celui de l'étudiant B
    } 
    else{
        return strcmp(etudiantA->nom_etu, etudiantB->nom_etu); //Ordre alphabétique (les num_grp de l'étudiant A et B sont identiques)
    }
}

void help(void){ //Commande supplémentaire affichant toutes les commandes
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
