# WordChecker

## Scopo del Progetto

Il progetto **WordChecker** è stato sviluppato per realizzare un sistema che confronta parole di uguale lunghezza, determinando la corrispondenza tra i caratteri rispetto a una parola di riferimento. L’obiettivo principale è fornire un feedback dettagliato per identificare la posizione corretta delle lettere, eventuali lettere fuori posto e lettere assenti.

Il programma gestisce una lista di parole ammissibili e consente all’utente di confrontarle con una parola target, rispettando regole e vincoli appresi durante il gioco. Le operazioni vengono eseguite tramite input da tastiera, utilizzando comandi specifici.

[![Schema](ReadmeFiles/ReadmeFiles/UMLProgettoApi.png)](ReadmeFiles/ReadmeFiles/UMLProgettoApi.pdf)

## Funzionalità Principali

Il sistema consente la gestione delle parole ammissibili tramite la lettura iniziale di un insieme di parole di lunghezza fissa senza duplicati, con la possibilità di inserire nuove parole dinamicamente. Durante le partite, l’utente può confrontare parole con la parola target ricevendo un feedback basato su tre simboli: `+` per lettere nella posizione corretta, `|` per lettere presenti ma in posizione errata e `/` per lettere non presenti. La partita termina con un esito positivo (`ok`) se la parola è indovinata o negativo (`ko`) dopo l'esaurimento dei tentativi.

Durante i tentativi, il sistema apprende vincoli come le lettere confermate in posizioni specifiche, le lettere assenti dalla parola target e il numero minimo ed esatto di occorrenze. È possibile filtrare dinamicamente le parole ammissibili in base ai vincoli appresi, ordinandole in ordine lessicografico.

## Implementazione

L’implementazione si basa su un’architettura efficiente e scalabile, utilizzando **alberi binari di ricerca (BST)** per la gestione delle parole ammissibili e **liste ordinate** per le parole filtrate. 

Il BST garantisce inserimenti ordinati e ricerche rapide, mentre la lista ordinata facilita la visualizzazione delle parole compatibili. La gestione dei vincoli avviene attraverso array e matrici che memorizzano informazioni su lettere scoperte, escluse e le loro posizioni corrette o errate.

## Logica di Gioco

Il programma segue un flusso di esecuzione che include il caricamento iniziale delle parole ammissibili, la gestione dei comandi per l'inizio di nuove partite, la stampa delle parole filtrate e l'inserimento di nuove parole. Dopo ogni tentativo di confronto, i vincoli vengono aggiornati e le parole compatibili sono ristampate in ordine lessicografico. La partita termina con successo se la parola è indovinata o con esito negativo se i tentativi vengono esauriti.

## Ottimizzazione e Debugging

Per garantire un’esecuzione efficiente e priva di errori, il progetto è stato testato con strumenti di analisi della memoria e delle prestazioni come **Valgrind** per il rilevamento di memory leaks, **Address Sanitizer** per individuare accessi non validi alla memoria e **Callgrind** per l'analisi delle prestazioni e l'identificazione di colli di bottiglia.
