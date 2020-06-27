### Pizza delivery
> 🍕 🍕 

Si vuole implementare un'applicazione basata sulle socket per l'ordine e la consegna a domicilio di pizze.

Il server implementa le seguenti funzioni:

- **acquisto**: la funzione riceve in ingresso la lista di pizze, l'indirizzo di consegna, l'ora di consegna e restituisce in uscita codice prenotazione di tipo intero;
- **cancellazione**: la funzione riceve in ingresso il codice della prenotazione e restituisce in uscita OK se l'annullamento viene effettuato un'ora prima della consegna, FAIL altrimenti.  

Si richiede di fornire lo pseudocodice del server iterativo che usa socket TCP.  
Il server riceve in ingresso il nome della funzione da eseguire e i parametri, esegue la funzione e ritorna il risultato al client.