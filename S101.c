#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#pragma warning (disable : 4996 6031)

enum{MAX_ETU = 100, //il ne peut pas y avoir plus de 100 étudiants inscrits
    NOM_MAX = 31, //limites numériques du programme (un nom ne peut pas excéder 30 caractères)
    MAX_ABS = 100,
    MAX_JUSTIFICATIF = 51, //Le texte de justification ne peux pas excéder 50 caractères
    MAX_COMMANDE = 70, //La commande justificatif fait un peu moins de 70 caractères si on utilise les 50 caractères possibles
    MAX_JOUR = 40,
    MIN_JOUR = 1,
    JOUR_JUST = 3};

typedef struct{
    char nomEtuTab[NOM_MAX]; //Contient le nom d'étudiant
    unsigned int numGrpTab; //Contient le numéro de groupe de chaque étudiant
    unsigned int idEtuTab; //Contient l'id étudiant de chaque étudiant
}Etudiant; //données concernant l'étudiant

typedef struct{
    unsigned int numJourTab; //Contient le numéro de jour de chaque absence
    char demiJourneeTab[3]; //Contient la demi-journée de chaque absence
    unsigned int idAbsEtuTab; //Contient l'id d'absence selon l'id de l'étudiant

    char justificatifTxtTab[MAX_JUSTIFICATIF]; //contient le texte justificatif d'absence de chaque étudiant ayant justifié une absence dans les temps
    char justificatifTxtNonJustTab[MAX_JUSTIFICATIF]; //contient le texte justificatif d'absence de chaque étudiant ayant justifié une absence mais non dans les temps
    unsigned int idAbsJustifieeTab; //contient l'id de l'absence si elle est justifiée
    unsigned int idAbsNonJustifeeTab; //contient les absences non justifées
    unsigned int idValidation; //contient l'id de validation de chaque justificatif
    unsigned int idAbsTab;
}Absence; //données concernant les absences d'un étudiant

typedef struct{
    unsigned int idEtuInc, idAbsInc; //permet l'incrémentation sans la modification des valeurs dans la fonction
    Absence tabAbsence[MAX_ABS]; //tableau pour contenir toutes les données de la commande absence
    Etudiant tabEtudiant[MAX_ETU]; //tableau pour contenir toutes les données de la commande etudiant
    Absence tabAbsenceCopie[MAX_ABS];
}Donnees; //struct évitant les variables globales

//fonctions prototypes
//commandes
int execution(char *commande, Donnees *donnees); //exécute une commande par comparaison
void help(void); //Commande supplémentaire affichant toutes les commandes
int inscription(char nomEtu[NOM_MAX], unsigned int numGrp, Donnees *donnees); //C1 : inscription <nom etu> <nom grp> → inscription de l'étudiant
int absence(int tempIdEtu, int numJour, char demiJournee[3], Donnees *donnees); //C2 : absence : <id etu> <Num jour> <am/pm> → enregistrer une absence
int etudiants(int numJourCourant, Donnees *donnees); //C3 : etudiants <Num jour courant> → liste des etudiants
int justificatif(unsigned int tempIdAbs, unsigned int numJour, char justificatifTxt[MAX_JUSTIFICATIF], Donnees *donnees); //C4 : justificatif <id etu> <numJour> <justificatif> → enregistre un justificatif pour une absence existante
int validations(Donnees *donnees); //C5 : validations → affiche toutes les justifications en attente d'une validation
int validation(unsigned int tempIdAbsJust, char code[3], Donnees* donnees); //C6 : validation <id abs> <ok/ko> → valide une absence justifiée
int etudiant(unsigned int tempIdEtu, unsigned int numJourCourant, Donnees *donnees); //C7 : situation d'un étudiant
int defaillants(unsigned int numJourCourant, Donnees *donnees); //C8 : liste des étudiants défaillants

//fonctions bool
bool etudiantExistance(Donnees *donnees, int tempIdEtu); //vérifie si l'id étudiant existe
bool absenceExistance(Donnees *donnees, int tempIdAbs); //vérifie si l'id de l'absence existe (selon l'id etu temp)
bool absenceJustExistance(Donnees *donnees, int tempIdAbsJust); //vérifie si l'id de l'absence existe
bool validationAttente(Donnees *donnees); //vérifie s'il y a des validations en attente

//fonctions pour les qsort
int compareEtu(const void *a, const void *b); //qsort utile dans le C3 (trie l'affichage de la C3, par ordre croissant des grpEtu puis au sein d'un même groupe par ordre alphabétique)
int comparerAbsences(const void *a, const void *b); //qsort utile dans la C5 (trie l'affichage de la C5, par ordre croissant des idEtu puis par ordre chronologique des absences)
int comparerChronos(const void *a, const void *b); //qsort utile dans la C7 (trie l'affichage de la C7, par ordre chronologique)
void copierAbsences(Donnees *donnees); //copie le tableau des absences pour éviter une modification des index(lors du tri de la C5)

int execution(char *commande, Donnees *donnees){ //exécute une commande par comparaison
        char nomEtu[NOM_MAX]; //nom d'étudiant ne peut pas excéder NOM_MAX de caractères
        unsigned int numGrp;
        unsigned int numJour = 1;
        char demiJournee[3];
        unsigned tempIdEtu; //utile dans la commande absence, désigne l'entrée de l'utilisateur
        unsigned int numJourCourant = 1;
        unsigned int tempIdAbs; //utile dans la commande justificatif, désigne l'éntrée de l'utilisateur
        unsigned int tempIdAbsJust; //utilise dans la commande validation
        char justificatifTxt[MAX_JUSTIFICATIF];
        char code[3];

    if(strcmp(commande, "exit") == 0){ //C0 : exit
        exit(0); //arrêt du programme
    }
    else if(sscanf(commande, "inscription %29s %u", nomEtu, &numGrp) == 2){ //C1 : inscription
        inscription(nomEtu, numGrp, donnees);
    }
    else if (sscanf(commande, "absence %u %u %2s", &tempIdEtu, &numJour, demiJournee) == 3) { //C2 : absence
        absence(tempIdEtu, numJour, demiJournee, donnees);
    }
    else if(sscanf(commande, "etudiants %u", &numJourCourant) == 1){ //C3 : etudiants
        etudiants(numJourCourant, donnees);
    }
    else if(sscanf(commande, "justificatif %u %u %[^\n]s", &tempIdAbs, &numJour, justificatifTxt) == 3){ //C4 : justificatif
        justificatif(tempIdAbs, numJour, justificatifTxt, donnees);
    }
    else if(strcmp(commande, "validations") == 0){ //C5 : validations
        validations(donnees);
    }
    else if(sscanf(commande, "validation %u %2s", &tempIdAbsJust, code) == 2){ //C6 : validation
        validation(tempIdAbsJust, code, donnees);
    }
    else if (sscanf(commande, "etudiant %u %u", &tempIdEtu, &numJourCourant) == 2) {
        etudiant(tempIdEtu, numJourCourant, donnees); //C7 : situation d'un étudiant
    }
    else if (sscanf(commande, "defaillants %u %u", &numJourCourant) == 1) {
        defaillants(numJourCourant, donnees); //C8 : liste des étudiants défaillants
    }
    else if (strcmp(commande, "help") == 0) { //Cpersonnalisée : help
        help();
    }
    return 1;
}

int main(int argc, char *argv[]){
    char commande[MAX_COMMANDE];
    Donnees donnees = {1, 1};
    while(1){ //boucle tant que l'on n'arrête pas le programme
            if (argc > 1) { //si l'entrée n'est pas nulle
                strcpy(commande, argv[1]);
            } else {
                fgets(commande, sizeof(commande), stdin);
                commande[strcspn(commande, "\n")] = 0; //évite de prendre en compte du retour à la ligne du fgets -> dans la commande dès qu'il y a un retour à la ligne il est remplacé par le caractère nul
                execution(commande, &donnees); //execution de la commande
            }
    }
}

bool etudiantExistance(Donnees *donnees, int tempIdEtu){
    for(int i = 1; i < donnees->idEtuInc; ++i){ //evite que l'on enregistre une absence à l'id d'un étudiant inexistant
        if(tempIdEtu == donnees->tabEtudiant[i].idEtuTab){
            return true; //l'id existe
        }
    }
    return false; //l'id n'existe pas
}

bool absenceExistance(Donnees *donnees, int tempIdAbs){
    for(int i = 1; i < donnees->idAbsInc; ++i){
        if(tempIdAbs == donnees->tabAbsence[i].idAbsTab){
        return true; //l'id existe
        }
    }
    return false; //l'id n'existe pas
}

bool absenceJustExistance(Donnees *donnees, int tempIdAbsJust){
    for(int i = 1; i < donnees->idAbsInc; ++i){
        if(tempIdAbsJust == donnees->tabAbsence[i].idAbsJustifieeTab){
        return true; //l'id existe
        }
    }
    return false; //l'id n'existe pas
}

bool validationAttente(Donnees *donnees){
    for(int i = 1; i < donnees->idAbsInc; ++i){
        if(donnees->tabAbsence[i].idAbsJustifieeTab != 0 && donnees->tabAbsence[i].idValidation == 0){
            return true;
        }
    }
    return false;
}

void copierAbsences(Donnees *donnees){
    for (int i = 0; i < donnees->idAbsInc; ++i) {
        donnees->tabAbsenceCopie[i] = donnees->tabAbsence[i];
    }
}

int inscription(char nomEtu[NOM_MAX], unsigned int numGrp, Donnees *donnees){ //C1 : inscription <nom etu> <nom grp> → inscription de l'étudiant

    for(int i = 1; i < donnees->idEtuInc; ++i){
        if((strcmp(nomEtu, donnees->tabEtudiant[i].nomEtuTab) == 0) && (numGrp == donnees->tabEtudiant[i].numGrpTab)){ //évite les doublons
            printf("Nom incorrect\n");
            return 0;
        }
    }
    strcpy(donnees->tabEtudiant[donnees->idEtuInc].nomEtuTab, nomEtu); //le nom de l'étudiant est copié dans un tableau se trouvant à la même place que l'id de l'étudiant pour pouvoir attribuer ce nom à un étudiant(id)
    donnees->tabEtudiant[donnees->idEtuInc].numGrpTab = numGrp; //le numéro de groupe est copié dans un tableau se trouvant à la même place que l'id de l'étudiant pour pouvoir attribuer ce numéro à un étudaint(id)
    donnees->tabEtudiant[donnees->idEtuInc].idEtuTab = donnees->idEtuInc; //l'id de l'étudiant est copié dans un tableau à la même place que son id. Ainsi on a l'id 1 qui se trouve à la position 1 du tableau, permettant une gestion plus facile des données
    printf("Inscription enregistree (%u)\n", donnees->idEtuInc);
    ++donnees->idEtuInc; //incrémente l'id de l'étudiant à chaque nouvelle inscription
    return 1;
}

int absence(int tempIdEtu, int numJour, char demiJournee[3], Donnees *donnees){ //C2 : absence : <id etu> <Num jour> <am/pm>

    if(!etudiantExistance(donnees, tempIdEtu)){ //si l'identifiant n'existe pas
        printf("Identifiant incorrect\n");
        return 0;
    }
    if(numJour > MAX_JOUR || numJour < MIN_JOUR){ //1 <= numJour <=40
        printf("Date incorrecte\n");
        return 0;
    }
    if((strcmp(demiJournee, "am") != 0) && (strcmp(demiJournee, "pm") != 0)){ //demiJournee soit égale à pm soit à am
        printf("Demi-journee incorrecte\n");
        return 0;
    }
    for(int i = 1; i < donnees->idAbsInc; ++i){
        if(tempIdEtu == donnees->tabAbsence[i].idAbsEtuTab && numJour == donnees->tabAbsence[i].numJourTab && strcmp(demiJournee, donnees->tabAbsence[i].demiJourneeTab) == 0){ //évite les doublons d'absence
            printf("Absence deja connue\n");
            return 0;
        }
    }
    donnees->tabAbsence[donnees->idAbsInc].idAbsEtuTab = tempIdEtu; //on enregiste l'id de l'absence de l'étudiant selon l'id de l'étudiant puisque c'est celui-ci que l'on rentre
    donnees->tabAbsence[donnees->idAbsInc].idAbsTab = donnees->idAbsInc; //on enregistre l'id de l'absence pour les futures commandes nécessitant l'id de l'absence et non l'id de l'étudiant
    donnees->tabAbsence[donnees->idAbsInc].numJourTab = numJour; //le numéro de jour est copié dans un tableau se trouvant à la même place que l'absence pour pouvoir attribuer ce numéro à l'absence
    strcpy(donnees->tabAbsence[donnees->idAbsInc].demiJourneeTab, demiJournee); //la demi-journée est copiée dans un tableau se trouvant à la même place que l'absence pour pouvoir attribuer cette demi-journée à l'absence
    printf("Absence enregistree [%u]\n", donnees->idAbsInc);
    ++donnees->idAbsInc; //incrémente l'id de l'absence à chaque nouvelle absence

    return 1;
}

int etudiants(int numJourCourant, Donnees *donnees){
    qsort(donnees->tabEtudiant + 1, donnees->idEtuInc - 1, sizeof(Etudiant), compareEtu); //trie le tableau d'étudiants dans un ordre croissant (par groupe), puis par ordre alphabétique
    if(donnees->idEtuInc <= 1){ //s'il n'y a aucun inscrit
        printf("Aucun inscrit\n");
        return 0;
    }
    if(numJourCourant < MIN_JOUR){
        printf("Date incorrecte\n");
        return 0;
    }
    for(int i = 1; i < donnees->idEtuInc; ++i){
        unsigned int totalAbs = 0; //le total d'absences revient à 0 pour chaque nouveau étudiant
        for(int j = 1; j < donnees->idAbsInc; ++j){
            if(donnees->tabAbsence[j].idAbsEtuTab == donnees->tabEtudiant[i].idEtuTab &&
               donnees->tabAbsence[j].numJourTab <= numJourCourant){
                totalAbs++; //incrémente le total d'absence si on trouve une absence (pour l'étudiant) pour le num de jour courant supérieur ou égal au num de jour de l'absence
            }
        }
        printf("(%u) %-30s %4u %4u\n", donnees->tabEtudiant[i].idEtuTab, donnees->tabEtudiant[i].nomEtuTab, donnees->tabEtudiant[i].numGrpTab, totalAbs); //printf pour chaque étudiant, ligne par ligne
    }
    return 1;
}

int compareEtu(const void *a, const void *b){
    const Etudiant *etudiantA = (Etudiant *)a;
    const Etudiant *etudiantB = (Etudiant *)b;
    if (etudiantA->numGrpTab < etudiantB->numGrpTab) { //si le numGrp de l'étudiant A < numGrp de l'étudiant B
        return -1;
    } //numGrp de l'étudiant A est bien inférieur à celui de l'étudiant B
    else if(etudiantA->numGrpTab > etudiantB->numGrpTab) { //si le numGrp de l'étudiant A < numGrp de l'étudiant B
        return 1; //id de l'étudiant A est bien inférieur à celui de l'étudiant B
    }
    else{
        return strcmp(etudiantA->nomEtuTab, etudiantB->nomEtuTab); //Ordre alphabétique (les numGrp de l'étudiant A et B sont identiques)
    }
}

int justificatif(unsigned int tempIdAbs, unsigned int numJour, char justificatifTxt[MAX_JUSTIFICATIF], Donnees *donnees){

    if(!absenceExistance(donnees, tempIdAbs)){
        printf("Identifiant incorrect\n");
        return 0;
    }
    if(numJour < donnees->tabAbsence[tempIdAbs].numJourTab){
        printf("Date incorrecte\n");
        return 0;
    }
    for(int i = 1; i < donnees->idAbsInc; ++i){
        if(tempIdAbs == donnees->tabAbsence[i].idAbsJustifieeTab || tempIdAbs == donnees->tabAbsence[i].idAbsNonJustifeeTab){
            printf("Justificatif deja connu\n");
            return 0;
        }
    }
    if(numJour - donnees->tabAbsence[tempIdAbs].numJourTab > JOUR_JUST){ //si le numJour dépasse 3 jours au numJour de l'absence, on enregistre le justificatif et on classe l'absence comme étant non justifiée
        donnees->tabAbsence[tempIdAbs].idAbsNonJustifeeTab = tempIdAbs;
        strcpy(donnees->tabAbsence[tempIdAbs].justificatifTxtNonJustTab, justificatifTxt);
        printf("Justificatif enregistre\n");
        return 0;
    }
    strcpy(donnees->tabAbsence[tempIdAbs].justificatifTxtTab, justificatifTxt); //copie le texte justificatif de l'absence
    donnees->tabAbsence[tempIdAbs].idAbsJustifieeTab = tempIdAbs; //enregistre l'id de l'absence justifiée dans un tableau ayant le meme Id que son absence (on différencie celles non justifiées et celles non justifiées)
    printf("Justificatif enregistre\n");
    return 1;
}

int validations(Donnees *donnees) {
    if (!validationAttente(donnees)) {
        printf("Aucune validation en attente\n");
        return 0;
    }

    copierAbsences(donnees);  //copie des absences
    qsort(donnees->tabAbsenceCopie + 1, donnees->idAbsInc - 1, sizeof(Absence), comparerAbsences);

    for (int i = 1; i < donnees->idAbsInc; ++i){
        if (donnees->tabAbsenceCopie[i].idAbsJustifieeTab != 0 && donnees->tabAbsenceCopie[i].idValidation == 0){
            unsigned int etuId = donnees->tabAbsenceCopie[i].idAbsEtuTab;
            char *etuNom = ""; //on initialise la variable etuNom
            unsigned int etuGrp = 0; //on initialise la variable etuGrp
            for (int j = 1; j < donnees->idEtuInc; ++j){
                if(donnees->tabEtudiant[j].idEtuTab == etuId){ //on vérifie qu'il y a une correspondance entre les idEtu (on vérifie à différents index du fait de l'enregistrement de données)
                    etuNom = donnees->tabEtudiant[j].nomEtuTab; //on prend etuNom au bon index
                    etuGrp = donnees->tabEtudiant[j].numGrpTab; //on prend etuGrp au bon index
                    break; //on sort de la boucle une fois qu'on a trouvé une correspondance pour ne pas avoir à faire des tours inutiles
                }
            }
            printf("[%u] (%-u) %-30s %4u %3u/%2s (%s)\n",
                   donnees->tabAbsenceCopie[i].idAbsTab,
                   etuId,
                   etuNom,
                   etuGrp,
                   donnees->tabAbsenceCopie[i].numJourTab,
                   donnees->tabAbsenceCopie[i].demiJourneeTab,
                   donnees->tabAbsenceCopie[i].justificatifTxtTab);
        }
    }
    return 1;
}

int comparerAbsences(const void *a, const void *b) {
    const Absence *absenceA = (const Absence *)a;
    const Absence *absenceB = (const Absence *)b;
    if (absenceA->idAbsEtuTab != absenceB->idAbsEtuTab) {
        return absenceA->idAbsEtuTab - absenceB->idAbsEtuTab; //Tri croissant par IdEtu
    }
    if (absenceA->numJourTab != absenceB->numJourTab) {
        return absenceA->numJourTab - absenceB->numJourTab; //Tri croissant par jour
    }
    return strcmp(absenceA->demiJourneeTab, absenceB->demiJourneeTab); //Si numJourTab est identique on compare par demiJourneeTab
}

int validation(unsigned int tempIdAbsJust, char code[3], Donnees* donnees){
    if(!absenceJustExistance(donnees, tempIdAbsJust) || tempIdAbsJust == 0){ //si l'id n'existe pas ou est égal à 0
        printf("Identifiant incorrect\n");
        return 0;
    }
    if((strcmp(code, "ok") !=  0) && (strcmp(code, "ko") != 0)){
        printf("Code incorrect\n");
        return 0;
    }
    for(int i = 1; i < donnees->idAbsInc; ++i){
        if(tempIdAbsJust == donnees->tabAbsence[i].idValidation){
            printf("Validation deja connue\n");
            return 0;
        }
    }
    if(strcmp(code, "ko") == 0){
        donnees->tabAbsence[tempIdAbsJust].idValidation = tempIdAbsJust;
        donnees->tabAbsence[tempIdAbsJust].idAbsNonJustifeeTab = donnees->tabAbsence[tempIdAbsJust].idAbsJustifieeTab;
        strcpy(donnees->tabAbsence[tempIdAbsJust].justificatifTxtNonJustTab, donnees->tabAbsence[tempIdAbsJust].justificatifTxtTab);
        printf("Validation enregistree\n");
        return 0;
    }
    if(strcmp(code, "ok") == 0){
        donnees->tabAbsence[tempIdAbsJust].idValidation = tempIdAbsJust;
        printf("Validation enregistree\n");
        return 1;
    }
}

int comparerChronos(const void *a, const void *b) {
    const Absence *absenceA = (const Absence *)a;
    const Absence *absenceB = (const Absence *)b;
    if (absenceA->numJourTab != absenceB->numJourTab) {
        return absenceA->numJourTab - absenceB->numJourTab; //Tri croissant par jour
    }
    return strcmp(absenceA->demiJourneeTab, absenceB->demiJourneeTab); //Si numJourTab est identique on compare par demiJourneeTab
}

int etudiant(unsigned int tempIdEtu, unsigned int numJourCourant, Donnees *donnees) {
    if (!etudiantExistance(donnees, tempIdEtu)) {
        printf("Identifiant incorrect\n");
        return 0;
    }

    // Affichage des infos de l'étudiant
    for (int i = 1; i < donnees->idEtuInc; ++i) {
        if (donnees->tabEtudiant[i].idEtuTab == tempIdEtu) {
            unsigned int totalAbs = 0;
            for (int j = 1; j < donnees->idAbsInc; ++j) {
                if (donnees->tabAbsence[j].idAbsEtuTab == tempIdEtu) {
                    totalAbs++;
                }
            }
            printf("(%u) %-30s %4u %4u\n", donnees->tabEtudiant[i].idEtuTab, donnees->tabEtudiant[i].nomEtuTab, donnees->tabEtudiant[i].numGrpTab, totalAbs);
            break;
        }
    }

    // Variables pour gérer l'état des absences
    bool enAttenteJustificatif = false, enAttenteValidation = false, justifiees = false, nonJustifiees = false;

    // Gestion des absences
    for (int j = 1; j < donnees->idAbsInc; ++j) {
        if (donnees->tabAbsence[j].idAbsEtuTab == tempIdEtu) {
            // En attente de justificatif
            if (strlen(donnees->tabAbsence[j].justificatifTxtTab) == 0 && numJourCourant - donnees->tabAbsence[j].numJourTab <= JOUR_JUST) {
                if (!enAttenteJustificatif) {
                    printf("- En attente justificatif\n");
                    enAttenteJustificatif = true;
                }
                printf("  [%u] %u/%s\n", donnees->tabAbsence[j].idAbsTab, donnees->tabAbsence[j].numJourTab, donnees->tabAbsence[j].demiJourneeTab);
            }

            // Absence non justifiée mais avec justificatif enregistré
            if (strlen(donnees->tabAbsence[j].justificatifTxtTab) > 0) {
                if (!nonJustifiees) {
                    printf("- Non-justifiees\n");
                    nonJustifiees = true;
                }
                printf("  [%u] %u/%s (%s)\n", donnees->tabAbsence[j].idAbsTab, donnees->tabAbsence[j].numJourTab, donnees->tabAbsence[j].demiJourneeTab, donnees->tabAbsence[j].justificatifTxtTab);
            }

            // Absence avec justificatif en attente de validation
            else if (strlen(donnees->tabAbsence[j].justificatifTxtTab) > 0 && donnees->tabAbsence[j].idValidation == 0) {
                if (!enAttenteValidation) {
                    printf("- En attente validation\n");
                    enAttenteValidation = true;
                }
                printf("  [%u] %u/%s (%s)\n", donnees->tabAbsence[j].idAbsTab, donnees->tabAbsence[j].numJourTab, donnees->tabAbsence[j].demiJourneeTab, donnees->tabAbsence[j].justificatifTxtTab);
            }

            // Absence justifiée si la validation est "ok"
            else if (donnees->tabAbsence[j].idValidation == donnees->tabAbsence[j].idAbsTab && strcmp(donnees->tabAbsence[j].justificatifTxtTab, "") != 0) {
                if (!justifiees) {
                    printf("- Justifiees\n");
                    justifiees = true;
                }
                printf("  [%u] %u/%s (%s)\n", donnees->tabAbsence[j].idAbsTab, donnees->tabAbsence[j].numJourTab, donnees->tabAbsence[j].demiJourneeTab, donnees->tabAbsence[j].justificatifTxtTab);
            }

            // Absence non justifiée si la validation est refusée (validation "ko")
            else if (donnees->tabAbsence[j].idValidation == donnees->tabAbsence[j].idAbsTab && strcmp(donnees->tabAbsence[j].justificatifTxtTab, "") != 0) {
                if (!nonJustifiees) {
                    printf("- Non-justifiees\n");
                    nonJustifiees = true;
                }
                printf("  [%u] %u/%s (%s)\n", donnees->tabAbsence[j].idAbsTab, donnees->tabAbsence[j].numJourTab, donnees->tabAbsence[j].demiJourneeTab, donnees->tabAbsence[j].justificatifTxtTab);
            }
        }
    }

    return 1;
}




int defaillants(unsigned int numJourCourant, Donnees *donnees) {

    if (numJourCourant < MIN_JOUR) {
        printf("Date incorrecte\n");
        return 0;
    }

    bool aucunDefaillant = true;  //on vérifie s'il y a des défaillants


    for (int i = 1; i < donnees->idEtuInc; ++i) {
        unsigned int totalAbs = 0;            //total des absences de l'étudiant
        unsigned int absNonJustifiees = 0;    //total des absences non justifiées

        for (int j = 1; j < donnees->idAbsInc; ++j) {
            //jour <= au jour courant
            if (donnees->tabAbsence[j].idAbsEtuTab == donnees->tabEtudiant[i].idEtuTab &&
                donnees->tabAbsence[j].numJourTab <= numJourCourant) {

                //Compter le total des absences
                totalAbs++;

                //Vérifier si l'absence n'est pas justifiée
                if (donnees->tabAbsence[j].idAbsJustifieeTab == 0 &&
                    strlen(donnees->tabAbsence[j].justificatifTxtTab) == 0) {
                    absNonJustifiees++;  // on compte les absences non justifiées
                    }
                }
        }


        if (absNonJustifiees >= 5) { //on vérifie si l'étudiant est défaillant (5 absences non justifiées ou plus)

            aucunDefaillant = false; //si on trouve au moins un défaillant, on désactive notre condition


            printf("(%u) %-30s %4u %4u\n", donnees->tabEtudiant[i].idEtuTab, //on affiche les informations de l'étudiant : id, nom, groupe, total absences
                   donnees->tabEtudiant[i].nomEtuTab, donnees->tabEtudiant[i].numGrpTab, totalAbs);
        }
    }


    if (aucunDefaillant) {
        printf("Aucun defaillant\n");
    }

    return 1;
}

void help(void){ //Commande supplémentaire affichant toutes les commandes
    printf("********************************************************************************************************************************************\n"); //esthétique
    printf("%80s", "Commandes disponibles\n");
    printf("********************************************************************************************************************************************\n"); //esthétique
    printf("%-70s %70s", "help", "affiche la liste des commandes disponibles\n"); //Cpersonnalisée
    printf("%-70s %70s", "exit", "arrete le programme\n"); //C0
    printf("%-70s %70s", "inscription <nom etu> <nom grp>", "inscription de l'etudiant\n"); //C1
    printf("%-70s %70s", "absence <id etu> <Num jour> <am/pm>", "enregistrer une absence\n"); //C2
    printf("%-70s %70s", "etudiants <Num jour courant>", "liste des etudiants\n"); //C3
    printf("%-70s %70s", "justificatif <Id absence> <Num jour> <justificatif>", "Depot de justificatif\n"); //C4
    printf("%-70s %70s", "validations", "liste des justifications a valider\n"); //C5
    printf("%-70s %70s", "validation <id absence> <ok/ko>", "validation d'une justification d'absence\n"); //C6
    printf("%-70s %70s", "etudiant <id etu> <Num jour courant>", "situation d'un etudiant\n"); //C7
    printf("%-70s %70s", "defaillants <Num jour courant>", "liste des étudiants défaillants\n"); //C8
}