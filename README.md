SISTEMA PARCHEGGIO

  Il sistema si compone di due programmi da eseguire su terminali diversi.
  1.Sistema parcheggio con file di test (src/"".txt).
  2.Client di connessione al sistema parcheggio.


PER IL CORRETTO FUNZIONAMENTO

  Entrambi i programmi richiedono all'avvio un input (PORT) che rappresenta la
  porta di connessione per le socket.
  Devono essere uguali.

  Errori conosciuti:

 1. Nel caso di terminazione forzata del programma da terminale
    un nuovo avvio del programma non può essere fatto sulla stessa PORT senza riavviare
    il terminale.
    ERR: "server: failed to bind"

 2. Il Client riesce a connettersi ad una PORT errata nel caso di input PORT inserite
    in precedenza e che hanno portato ad una terminazione forzata da terminale.

 3. Il Programma parcheggio necessita di una richiesta di connessione dal client per riuscire 
    a terminare in modo corretto. La thread di connessione rimane bloccata nella fase di accept().

COMPILAZIONE

  Il sistema utilizza il CMAKE. Da terminale nella giusta locazione basta scrivere:

->   cmake ./

  per creare i MakeFile e poi

->  make

  per compilare il programma e creare l'eseguibile.


ESECUZIONE
  Il programma va eseguito nella directory src scrivendo:

-> ./main PORT
o
-> ./client PORT

PORT è un numero di 4 cifre (esempio 3490) relativo alla porta di connessione della socket
evitare porte occupate dal sistema
