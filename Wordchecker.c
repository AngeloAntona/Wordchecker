#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 65 //numero di caratteri ammissibili
typedef struct node* pNode;

struct node {
    char* word; //contiene una parola.
    pNode r; //puntatore al figlio destro dell'albero complessivo.
    pNode l; //puntatore al figlio sinistro dell'albero complessivo.
    pNode succ;
};

typedef struct{
    int* minNumCh; //contiene il numero di occorrenze minime per ogni carattere presente nella stringa.
    int*  numCh;   //quante volte ogni carattere è presente.
    char* corrCh;  //lettere di cui il giocatore ha scoperto la posizione.
    char* wrongCh; //lettere che è certo non appartengano a parolaTarget.
    int k;
} clues;


/*====================================================UTILITA'========================================================*/

int compare (const char word1[], const char word2[], int k){
    for(int i=0; i < k ; i++){
        if(word1[i] > word2[i]) {return 1;}  // 1 se è maggiore la 1°.
        if(word1[i] < word2[i]) {return -1;} //-1 se è maggiore la 2°.
    }
    return 0;
}

int scanWord(char container[], int k) {
    char curChar;
    for (int i=0; i < k; i++){
        curChar=(char)getchar();
        if (curChar == '+'){return -1;} //stringa successiva è un comando.
        container[i]=curChar;
    }
    getchar();
    return 1; //è stata letta una stringa.
}

char commandChar(){
    char com[17]; //contiene il comando inserito dall'utente.
    char nextChar; //serve per sapere se il carattere successivo è ' ' o '\n'.
    nextChar=(char)getchar();
    int i=0;
    while(nextChar!=' '&&nextChar!='\n'&&nextChar!=EOF){
        com[i]=nextChar;
        nextChar=(char)getchar();
        i++;
    }
    //inserisci_inizio
    if(com[0]=='i'&&com[1]=='n'&&com[2]=='s'&&com[3]=='e'&&com[4]=='r'&&com[5]=='i'&&com[6]=='s'&&
       com[7]=='c'&&com[8]=='i'&&com[9]=='_'&&com[10]=='i'){
        return 'i';
    }
        //inserisci_fine
    else if(com[0]=='i'&&com[1]=='n'&&com[2]=='s'&&com[3]=='e'&&com[4]=='r'&&com[5]=='i'&&com[6]=='s'&&
            com[7]=='c'&&com[8]=='i'&&com[9]=='_'&&com[10]=='f'){
        return 'f';
    }
        //nuova_partita
    else if(com[0]=='n') {
        return 'n';
    }
        //stampa_filtrate
    else if(com[0]=='s'){
        return 's';
    }
    return 'e';
}

void deleClues(clues* info){
    free(info->wrongCh);
    free(info->minNumCh);
    free(info->corrCh);
    free(info->numCh);
}


//=========================================================================================================ALBERO&LISTA.


void deleTree(pNode tree){
    if(tree == NULL){ return; }
    deleTree(tree->l);
    deleTree(tree->r);
    free(tree->word);
    free (tree);
}

int doesItExists(pNode tree, char word[], int k){
    while (tree != NULL) {
        if (compare(word, tree->word, k)==-1){tree = tree->l;}
        else if (compare(word, tree->word, k)==1){tree = tree->r;}
        else{return 1;}
    }
    return 0;
}

pNode putInList(pNode list, pNode newNode, int k){
    //unico elemento.
    if(list==NULL) {
        return newNode;
    }
    //inserimento in testa.
    if(compare(list->word, newNode->word, k)==1){
        newNode->succ=list;
        return newNode;
    }
    //inserimento in mezzo.
    pNode pre=list;
    pNode curr=pre->succ;
    while(curr!=NULL){
        if(compare(curr->word, newNode->word,k)==1){ //se l'elemento corrente è maggiore del newNode, allora sarà il nodo successivo al newNode.
            pre->succ=newNode;
            newNode->succ=curr;
            return list;
        }
        pre=curr;
        curr=curr->succ;
    }
    //inserimento in coda.
    pre->succ=newNode;
    return list;
    //la funzione dovrebbe chiamare puInList scorrendo l'albero dall'elemento più grande al più piccolo,
    // quindi in teoria si avrà sempre che il newNode andrà inserito in testa.

}

pNode putNode(int k, pNode tree, pNode newNode)
{
    pNode curr = tree;
    pNode pre = NULL;
    //scorrimento albero.
    while (curr != NULL)
    {
        pre = curr;
        if (compare(newNode->word, curr->word, k) == -1){curr = curr->l;}
        else{curr = curr->r;}
    }
    //inserimento nodo.
    if (pre == NULL){
        return newNode;
    }
    else if (compare(newNode->word, pre->word, k) == -1){pre->l = newNode;}
    else{pre->r = newNode;}
    return tree;
}

pNode initialInsert(int k, pNode tree, char* command){
    pNode newNode;
    char* newWord;
    while (1){
        newWord = (char*)malloc(k * sizeof (char)+1);
        if(scanWord(newWord, k) == -1){ //_______________________________________________________letturaComando.
            free(newWord);
            *command= commandChar(); //restituzione command.
            return tree;
        }
        else { //____________________________________________________________________________________inserimentoNewWord.
            newNode= (pNode)malloc(sizeof(struct node)+1);
            newNode->r=NULL;
            newNode->l=NULL;
            newNode->succ=NULL;
            newNode->word=newWord; //inserimento newWord in newNodo.
            tree= putNode(k, tree, newNode); //inseriamo newNodo in tree.
        }
    }
    return tree;
}

void insert(pNode tree, pNode toReturn[], pNode list, char* command, clues* info, const char targetW[], int wrongPos[][info->k]){
    pNode newNode;
    char* newWord;
    while (1){
        newWord = (char*)malloc(info->k * sizeof (char));
        //_________________________________________________________________________________________________letturaInput.
        if(scanWord(newWord, info->k) == -1){
            free(newWord);
            *command= commandChar(); //restituzione command.
            return;
        }
        else { //___________________________________________________________________________________________inserimento.
            newNode= (pNode)malloc(sizeof(struct node));
            newNode->r=NULL;
            newNode->l=NULL;
            newNode->succ=NULL;
            newNode->word=newWord; //inserimento newWord in newNodo.
            toReturn[0]= putNode(info->k, tree, newNode); // Inseriamo newNodo in tree.
            //_______________________________________________________________________________________________filtraggio.
            int boolean=1;
            for(int i=0; i<info->k; i++){//wronglettertest.
                for(int j=0; j<N && info->wrongCh[j] != '.'; j++){
                    if(info->wrongCh[j] == newWord[i]){boolean= 0;}
                }
            }//reservedPositionTest.
            if (boolean==1){
                for(int i=0; i<info->k; i++){
                    if(info->corrCh[i] != '.' && newWord[i] != info->corrCh[i]){boolean= 0;}
                }
            }//wrongPositionTest.
            if(boolean==1){
                for(int i=0; i<info->k; i++){
                    for(int h=0; h<info->k && wrongPos[i][h] != -1; h++){
                        //se il carCurr si trova in una posizione vietata ritorna 0:
                        if(targetW[i] == newWord[wrongPos[i][h]]){boolean= 0;}
                    }
                }
            }//wrongNumberTest.
            if(boolean==1){
                for(int i=0; i<info->k; i++){ //l'indice i indicherà uno a uno gli elementi del vettore targetW.
                    int numInWord=0; //num ricorrenze di charCurr in word.
                    int index=0;
                    //trova la posizione della 1° occorrenza di carCurr:
                    while(index < info->k && targetW[i] != targetW[index]){index++;}
                    //conta quante volte il carattere compare nella word contenuta nel nodo attuale:
                    for(int s=0; s<info->k; s++){ if(newWord[s] == targetW[i]){numInWord++;} }
                    //_____________________________________________________________________________________numeroEsatto.
                    //Se conosce numCh, se il carCur non appare esattamente numCh volte ritorna 0:
                    if(info->numCh[i] != -1 ){ if(numInWord != info->numCh[i]){boolean= 0;} }
                        //_____________________________________________________________________________almenoTanteVolte.
                        //non non conosce numCh, ma ha un numero minimo di occorrenze che devono apparire:
                    else if(info->minNumCh[i] != -1){ if(numInWord < info->minNumCh[index]){boolean= 0;}
                    }
                }
            }
            if(boolean==1){toReturn[1]= putInList(list, newNode, info->k);}
            else{toReturn[1]=list;}
        }
    }
    return;
}

pNode initialFilter(int* sum, pNode tree, pNode list, clues* info, char targetW[], int wrongPos[][info->k]){
    if(tree == NULL){return list;}
    //_______________________________________________________________________________________________________ricorsione.
    list= initialFilter(sum, tree->r, list, info, targetW, wrongPos);
    //_______________________________________________________________________________________________________filtraggio.
    int boolean=1;
    for(int i=0; i<info->k; i++){//wronglettertest.
        for(int j=0; j<N && info->wrongCh[j] != '.'; j++){
            if(info->wrongCh[j] == tree->word[i]){ boolean= 0;}
        }
    }//reservedPositionTest.
    if (boolean==1){
        for(int i=0; i<info->k; i++){
            if(info->corrCh[i] != '.' && tree->word[i] != info->corrCh[i]){ boolean= 0;}
        }
    }//wrongPositionTest.
    if(boolean==1){
        for(int i=0; i<info->k; i++){
            for(int h=0; h<info->k && wrongPos[i][h] != -1; h++){
                //se il carCurr si trova in una posizione vietata ritorna 0:
                if(targetW[i] == tree->word[wrongPos[i][h]]){ boolean= 0;}
            }
        }
    }//wrongNumberTest.
    if(boolean==1){
        for(int i=0; i<info->k; i++){ //l'indice i indicherà uno a uno gli elementi del vettore targetW.
            int numInWord=0; //num ricorrenze di charCurr in word.
            int index=0;
            //trova la posizione della 1° occorrenza di carCurr:
            while(index < info->k && targetW[i] != targetW[index]){index++;}
            //conta quante volte il carattere compare nella word contenuta nel nodo attuale:
            for(int s=0; s<info->k; s++){ if(tree->word[s] == targetW[i]){numInWord++;} }
            //_____________________________________________________________________________________________numeroEsatto.
            //Se conosce numCh, se il carCur non appare esattamente numCh volte ritorna 0:
            if(info->numCh[i] != -1 ){ if(numInWord != info->numCh[i]){boolean= 0;} }
                //_____________________________________________________________________________________almenoTanteVolte.
                //non conosce numCh, ma ha un numero minimo di occorrenze che devono apparire:
            else if(info->minNumCh[i] != -1){ if(numInWord < info->minNumCh[index]){boolean= 0;}
            }
        }
    }
    //______________________________________________________________________________________________________inserimento.
    if(boolean==1) {
        list = putInList(list, tree, info->k);
        *sum=*sum+1;
    }
    list= initialFilter(sum, tree->l, list, info, targetW, wrongPos);
    return list;
}

pNode listFilter(pNode list, clues* info, const char targetW[], int wrongPos[][info->k], int* sum){
    //_______________________________________________________________________________________________________filtraggio.
    pNode pre=NULL;
    pNode curr=list;
    while(curr!=NULL){
        int boolean=1;
        for(int i=0; i<info->k; i++){//wronglettertest.
            for(int j=0; j<N && info->wrongCh[j] != '.'; j++){
                if(info->wrongCh[j] == curr->word[i]){ boolean= 0;}
            }
        }//reservedPositionTest.
        if (boolean==1){
            for(int i=0; i<info->k; i++){
                if(info->corrCh[i] != '.' && curr->word[i] != info->corrCh[i]){ boolean= 0;}
            }
        }//wrongPositionTest.
        if(boolean==1){
            for(int i=0; i<info->k; i++){
                for(int h=0; h<info->k && wrongPos[i][h] != -1; h++){
                    //se il carCurr si trova in una posizione vietata ritorna 0:
                    if(targetW[i] == curr->word[wrongPos[i][h]]){ boolean= 0;}
                }
            }
        }//wrongNumberTest.
        if(boolean==1){
            for(int i=0; i<info->k; i++){ //l'indice i indicherà uno a uno gli elementi del vettore targetW.
                int numInWord=0; //num ricorrenze di charCurr in word.
                int index=0;
                //trova la posizione della 1° occorrenza di carCurr:
                while(index < info->k && targetW[i] != targetW[index]){index++;}
                //conta quante volte il carattere compare nella word contenuta nel nodo attuale:
                for(int s=0; s<info->k; s++){ if(curr->word[s] == targetW[i]){numInWord++;} }
                //_________________________________________________________________________________________numeroEsatto.
                //Se conosce numCh, se il carCur non appare esattamente numCh volte ritorna 0:
                if(info->numCh[i] != -1 ){ if(numInWord != info->numCh[i]){boolean= 0;} }
                    //_________________________________________________________________________________almenoTanteVolte.
                    //non non conosce numCh, ma ha un numero minimo di occorrenze che devono apparire:
                else if(info->minNumCh[i] != -1){ if(numInWord < info->minNumCh[index]){boolean= 0;}
                }
            }
        }
        if(boolean==0){//_________________________________________________________________________________cancellazione.
            if(pre==NULL) { //se l'elemento da eliminare è il primo della lista
                list = list->succ; //la nuova testa sarà il secondo elemento.
                curr->succ = NULL;
                curr=list;
            }
            else{
                pre->succ=curr->succ; //altrimenti il nodo dovrà essere saltato.
                curr->succ=NULL;
                curr=pre->succ;
            }
        }
        else{
            pre=curr;
            curr=curr->succ;
            *sum=*sum+1;
        }
        //____________________________________________________________________________________________________cancellazione.
    }
    return list;
}

void finalListFilter(pNode list, clues* info, const char targetW[], int wrongPos[][info->k], int* sum){
    //_______________________________________________________________________________________________________filtraggio.
    pNode pre=NULL;
    pNode curr=list;
    while(curr!=NULL){
        int boolean=1;
        for(int i=0; i<info->k; i++){//wronglettertest.
            for(int j=0; j<N && info->wrongCh[j] != '.'; j++){
                if(info->wrongCh[j] == curr->word[i]){ boolean= 0;}
            }
        }//reservedPositionTest.
        if (boolean==1){
            for(int i=0; i<info->k; i++){
                if(info->corrCh[i] != '.' && curr->word[i] != info->corrCh[i]){ boolean= 0;}
            }
        }//wrongPositionTest.
        if(boolean==1){
            for(int i=0; i<info->k; i++){
                for(int h=0; h<info->k && wrongPos[i][h] != -1; h++){
                    //se il carCurr si trova in una posizione vietata ritorna 0:
                    if(targetW[i] == curr->word[wrongPos[i][h]]){ boolean= 0;}
                }
            }
        }//wrongNumberTest.
        if(boolean==1){
            for(int i=0; i<info->k; i++){ //l'indice i indicherà uno a uno gli elementi del vettore targetW.
                int numInWord=0; //num ricorrenze di charCurr in word.
                int index=0;
                //trova la posizione della 1° occorrenza di carCurr:
                while(index < info->k && targetW[i] != targetW[index]){index++;}
                //conta quante volte il carattere compare nella word contenuta nel nodo attuale:
                for(int s=0; s<info->k; s++){ if(curr->word[s] == targetW[i]){numInWord++;} }
                //_________________________________________________________________________________________numeroEsatto.
                //Se conosce numCh, se il carCur non appare esattamente numCh volte ritorna 0:
                if(info->numCh[i] != -1 ){ if(numInWord != info->numCh[i]){boolean= 0;} }
                    //_________________________________________________________________________________almenoTanteVolte.
                    //non non conosce numCh, ma ha un numero minimo di occorrenze che devono apparire:
                else if(info->minNumCh[i] != -1){ if(numInWord < info->minNumCh[index]){boolean= 0;}
                }
            }
        }
        if(boolean==1){*sum=*sum+1;}
        //____________________________________________________________________________________________________cancellazione.
        //dato che si elimineranno tutti gli elementi della lista, l'elemento da eliminare sarà sempre il primo elemento.
        pre=curr;
        curr=curr->succ;
        pre->succ=NULL;
    }
    return;
}

void showTree(pNode tree, int k){
    if(tree == NULL){return;} // Se arrivo alla fine ritorno.
    //_______________________________________________________________________________________________ricorsioneSinistra.
    showTree(tree->l, k);
    //___________________________________________________________________________________________________________stampa.
    for(int i=0; i<k; i++){putchar(tree->word[i]);}
    putchar('\n');
    //_________________________________________________________________________________________________ricorsioneDestra.
    showTree(tree->r, k);
    return;
}

void showFilteredList (pNode list, int k){
    pNode curr= list;
    while(curr!=NULL){
        for(int i=0; i<k; i++){
            putchar(curr->word[i]);
        }
        putchar('\n');
        curr=curr->succ;
    }
    return;
}

__attribute__((unused)) void resetList( pNode list){
    pNode pre=NULL;
    pNode curr=NULL;
    while(curr!=NULL){
        pre=curr;
        curr=curr->succ;
        pre->succ=NULL;
    }
}


//=====================================================================================================creazioneVincoli.


char compareWords (const char targetW[], const char attemptW[], clues* info, int wrongPos[][info->k]){
    //_________________________________________________________________________________________________ControlloVincita.
    int boolean=1;
    for (int i=0; i<info->k && boolean==1; i++){
        if(targetW[i] != attemptW[i]){ boolean=0;}
    }
    if (boolean==1){return 'w';}
    else{//____________________________________________________________________________________________Inizializzazione.
        char toPrint[info->k]; //contiene +, | e / da stampare dopo ogni tentativo.
        char copyAttemptW[info->k]; //conterrà un '.' per ogni lettera già considerata.
        char copyTargetW[info->k]; //conterrà un '.' per ogni lettera già considerata.
        for(int j=0; j<info->k; j++){
            copyAttemptW[j]=attemptW[j];
            copyTargetW[j]=targetW[j];
        }
        for(int i=0; i<info->k; i++) {
            if (targetW[i] == attemptW[i]) {
                //__________________________________________________________________________________________lettPosCorr.
                if(info->corrCh[i] == '.'){info->corrCh[i] =targetW[i];} //memorizzo il carattere corretto.

                int index=0;
                //trova la posizione della prima occorrenza del carCurr:
                while(index < info->k && copyAttemptW[i] != targetW[index]){index++;}
                if(info->numCh[index]==-1){ //se non ha ancora trovato numCh del carCurr...
                    int countMinNum=0;
                    for(int p=0; p<info->k; p++){
                        //conta quante volte il carattere compare nella attemptW:
                        if(copyAttemptW[i] == attemptW[p]){countMinNum++;}
                    }
                    //conserva il newMinNum se è più stringente del precedente.
                    if(info->minNumCh[index] < countMinNum){ info->minNumCh[index]=countMinNum;}
                }
                copyAttemptW[i] = '.';// segniamo l'elemento come "considerato".
                copyTargetW[i] = '.';// segniamo l'elemento come "considerato".
                toPrint[i] = '+';//per la visualizzazione.

            } else {
                //____________________________________________________________________________________________lettSbagl.
                int bool = 1; //1 solo se il carCurr è un carattere errato;
                for (int j = 0; j < info->k; j++) { if (copyAttemptW[i] == targetW[j]) { bool = 0;} }
                if (bool ==1) {
                    // inseriamo il carattere errato nell'opportuno vettore (nel caso non ci fosse già):
                    int s;
                    for(s=0;s<N && info->wrongCh[s] != '.' && info->wrongCh[s] != attemptW[i]; s++){}
                    if (info->wrongCh[s] == '.') { info->wrongCh[s] = attemptW[i];}
                    copyAttemptW[i] = '.'; // segniamo l'elemento come "considerato".
                    toPrint[i] = '/'; //per la visualizzazione.
                }
                else{ //se il carattere non è "proibito"
                    int index=0;
                    //trova la posizione della prima occorrenza del carCurr:
                    while(index < info->k && copyAttemptW[i] != targetW[index]){index++;}
                    if(info->numCh[index]==-1){ //se non ha ancora trovato numCh del carCurr...
                        int count=0;
                        for(int p=0; p<info->k; p++){
                            //conta quante volte il carattere compare nella parolaTentativo:
                            if(copyAttemptW[i] == attemptW[p]){count++;}
                        }
                        //conserva il newMinNum se è più stringente del precedente.
                        if(info->minNumCh[index] < count){ info->minNumCh[index]=count;}
                    }
                }
            }
        }
        int bool=1; //controlla se tutte le lettere sono state incluse nei casi precedenti
        for(int t=0; t<info-> k; t++){
            if(copyAttemptW[t] != '.')
                bool=0;
        }
        if(bool == 0){
            //________________________________________________________________________________________________lettDeall.
            // Il vettoreProva non contiene corrCh e wrongCh. I rimanenti caratteri in copyTargetW sono:
            //      - deallCh (no soprannumero) -> va salvato il carattere e la sua posizione errata.
            //      - deallCh (soprannumero) -> come prima ma si inserisce il numero di istanze in numCh;
            for(int i=0; i < info->k; i++){
                for (int j=0; j < info->k; j++){
                    if(copyAttemptW[i] != '.' && copyAttemptW[i] == copyTargetW[j]){
                        //____________________________________________________________________________________noLettNum.
                        //memorizzazione wrongPos.
                        int h=0;
                        while(wrongPos[j][h] != -1 && wrongPos[j][h] != i && h < (info->k)){h++;}
                        if(wrongPos[j][h] == -1){ wrongPos[j][h] = i;}

                        copyAttemptW[i]='.'; //segniamo l'elemento come "considerato".
                        copyTargetW[j]='.'; //segniamo l'elemento come "considerato".
                        toPrint[i]='|';//per la visualizzazione.
                    }
                }
            }
            //__________________________________________________________________________________________________numLett.
            for(int i=0; i < info->k; i++){
                if(copyAttemptW[i] != '.') //vero solo se è un carattere in soprannumero.
                {
                    int countNum=0; //contatore numCh.
                    for(int p=0; p<info->k; p++){
                        if(copyAttemptW[i] == targetW[p]){countNum++;}
                    }
                    //memorizza "countNum" nella prima cella in cui compare il carattere.
                    int index=0;
                    while(index < info->k && copyAttemptW[i] != targetW[index]){index++;}
                    if(info->numCh[index] == -1){ info->numCh[index]=countNum;}

                    //memorizza la wrongPos.
                    int p=0;
                    while(wrongPos[index][p] != -1 && wrongPos[index][p] != i && p < info->k){p++;}
                    if(wrongPos[index][p] == -1){ wrongPos[index][p] = i;}

                    toPrint[i]='/';//per la visualizzazione.
                }
            }
        }
        //______________________________________________________________________________________________visualizzazione.
        for(int i=0; i<info->k; i++){putchar(toPrint[i]);}
        printf("\n");

        return 'o';
    }
}


//========================================================================================================logicaDiGioco.


char attempt(pNode tree, clues* info, int wrongPos[][info->k]){
    char targetW[info->k]; //conterrà la word da indovinare.

    //_________________________________________________________________________________________________inizializzazione.
    for(int i=0; i<info->k; i++){ //Inizializzo tutti i vettori come vuoti con dei simboli convenzionali (. e -1).
        info->numCh[i]=-1;
        info->corrCh[i]='.';
        info->minNumCh[i]=-1;
        for(int j=0; j<info->k; j++){ wrongPos[i][j]=-1;}
    }
    for(int i=0; i<N; i++)
        info->wrongCh[i]='.';
    pNode list=NULL;
    //_____________________________________________________________________________________letturaValoriInizialiPartita.
    // lettura targetW:
    for(int i=0; i<info->k; i++){targetW[i]=(char)getchar();}
    if (getchar() == EOF) {return 'e'; } //Elimino lo spazio.
    //lettura countdown:
    int countdown;
    int scan;
    scan=scanf("%d", &countdown);
    if( scan !=1){return 'e';}
    if (getchar() == EOF) {return 'e'; } //Elimino lo spazio.
    char attemptW[info->k]; // Salviamo la attemptW in un array...
    //____________________________________________________________________________________________________inizioPartita.
    for(int j=0; j < countdown; j++){ // Finché non finiscono i tentativi
        attemptW[0]=(char)getchar();
        char command;
        if(attemptW[0] == '+'){ //se il 1° carattere è '+' la stringa è un comando.
            command= commandChar();
            if(command=='s'){
                if(j==0){ showTree(tree, info->k);}
                else{ showFilteredList(list, info->k);}
                j--; //ripristino iterazione.
            }
            else if(command=='i'){
                if(j==0){ initialInsert(info->k, tree, &command);}
                else{
                    pNode toReturn[2]; //toReturn[0]=newTree, toReturn[1]=newFilteredTree.
                    insert(tree, toReturn, list, &command, info, targetW, wrongPos);
                    tree=toReturn[0];
                    list=toReturn[1];
                }
                j--; //ripristino iterazione.
            }
            else if(command=='n'){
                return 'n';
            }
        }
        else{ //la stringa non è un comando.
            for (int i=1; i<info->k; i++){ attemptW[i]=(char)getchar(); } //lettura
            if (getchar() == EOF) { return 'e'; }
            if(doesItExists(tree, &attemptW[0], info->k) == 0){ //word non ammissibile.
                putchar('n');
                putchar('o');
                putchar('t');
                putchar('_');
                putchar('e');
                putchar('x');
                putchar('i');
                putchar('s');
                putchar('t');
                putchar('s');
                putchar('\n');
                j--; //ripristino iterazione.
            }
            else{ //word ammissibile.
                char result=compareWords(targetW, attemptW,  info, wrongPos);
                if( result!= 'w') //il giocatore non ha vinto.
                {
                    int sum=0;
                    if(j==0){list= initialFilter(&sum, tree, list, info, targetW, wrongPos); }
                    else if(j==countdown-1){ finalListFilter(list, info, targetW, wrongPos, &sum);}
                    else{ list= listFilter(list, info, targetW, wrongPos, &sum);}
                    printf("%d\n", sum);
                }
                else{return 'w';} //il giocatore ha vinto.
            }
        }
    }
    return 'o'; //noVincita.
}


int main(int argc, char*argv[]) {
    //____________________________________________________________________________________________lettura&dichiarazione.
    clues info;
    //lettura dimensione parole:
    int scan;
    scan = scanf("%d", &info.k);
    if(scan!=1){
        printf("Errore nella lettura di k.\n");
        return 0;
    }
    getchar();
    pNode tree=NULL; //albero delle parole.
    info.numCh= malloc(info.k * sizeof (int) + 1);
    info.corrCh= malloc(info.k * sizeof (char) + 1);
    info.wrongCh=malloc(N * sizeof(char) + 1);
    info.minNumCh= malloc(info.k * sizeof (int) + 1);
    int wrongPos[info.k][info.k]; //posizioni vietate per specifici caratteri.
    char command='.';//contiene il command dato dall'utente.
    //inserimento delle prime parole.
    tree = initialInsert(info.k, tree, &command);
    //_____________________________________________________________________________________________________cicloComandi.
    while(command != 'e') {
        if(command == 'n'){//command=='n' => attempt: valoreRitorno='w' -> vincita; valoreRitorno='o' -> noVincita.
            command= attempt(tree, &info, wrongPos);
            if(command == 'o'){
                putchar('k');
                putchar('o');
                putchar('\n');
                if (getchar() == EOF) { command='e';} //consumo '+'.
                if(command != 'e'){ command=commandChar();}
            }
            if(command == 'w'){
                putchar('o');
                putchar('k');
                putchar('\n');
                if (getchar() == EOF) { command='e';} //consumo '+'.
                if(command != 'e'){ command=commandChar();}
            }
        }
        if(command == 'i'){//command='i'=> Inserisci.
            tree= initialInsert(info.k, &tree[0], &command);
        }
        if(command == 'f'){ //command='f'=>si aspetta un nuovo command.
            if (getchar() == EOF) { command='e';} //consumo '+'.
            if(command != 'e'){ command=commandChar();}
        }
    }
    deleTree(tree);
    deleClues(&info);
    return 0;
}
