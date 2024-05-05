# Progetto di Algoritmi e Principi dell'Informatica

## Descrizione del Progetto
Il programma principale confronta due parole di ugual lunghezza e fornisce feedback basati sulla correttezza e posizione delle lettere. Il gioco include diverse fasi, dalla lettura delle parole ammissibili all'elaborazione dei tentativi dell'utente per indovinare la parola corretta.

### Schema del progetto
[![Schema](ReadmeFiles/ReadmeFiles/UMLProgettoApi.png)](ReadmeFiles/ReadmeFiles/UMLProgettoApi.pdf)

### Caratteristiche del Gioco
- **Input delle Parole**: Accetta parole formate da lettere maiuscole e minuscole, cifre numeriche, e i simboli '-' e '_'.
- **Modalità di Gioco**: L'utente può inserire nuove parole ammissibili, iniziare una nuova partita, e ricevere suggerimenti basati sui tentativi precedenti.
- **Output del Risultato**: Il programma fornisce un feedback sul numero di lettere corrette al posto giusto, al posto sbagliato, e completamente sbagliate.

### Comandi del Gioco
* inserisci_inizio: Inizia l'inserimento di nuove parole ammissibili.
* inserisci_fine: Termina l'inserimento di nuove parole.
* nuova_partita: Inizia una nuova partita.
* stampa_filtrate: Stampa tutte le parole ammissibili basate sul feedback dei tentativi.
