## POP3 📫 📧

Si vuole implementare un client di posta basato sui socket che supporti il protocollo POP3.  
Il client si collega al server POP3 che usa socket TCP, e spedisce al server tutti i comandi necessari per la visualizzazione dei messaggi
della mailbox e per l'accesso alle/cancellazione delle mail.  
Si richiede di:

1. Discutere brevemente il protocollo POP3
2. Fornire lo pseudocodice del client POP3 in modo che: 
	* L'utente si colleghi alla sua mailbox tramite username e password
    * Ad ogni accesso venga inviato un comando di `list` per visualizzare tutte le mail nella mailbox
    * L'utente possa eseguire un'operazione di `retry` o `delete` su un determinato messaggio. Il messaggio e l'operazione richiesti sono passati in ingresso dall'utente.

N.B. Si ricorda di mostrare nello pseudocodice tutti i comandi POP3 inviati dal client al server e di presentare tutte
le risposte inviate dal server al client.