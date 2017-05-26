# EPSON fiscal printer driver
Questo driver permette di comunicare con stampanti fiscali Epson, è ottenuto da codice liberamente distribuito da Epson Italia snc
e modificato per lavorare correttamente con stampanti fiscalizzate come la Epson FP90 e Epson FP81

### Introduzione ###

 


 
### Installazione ###

Il file fp_81_driver.c va compilato con il compilatore gc, l'esecuzione avviene tramite script "epson_print.sh"

### Funzionamento ###

Il driver C è sviluppato in maniera da poter essere utilizzato da applicativo web come POS Touch, quest'ultimo richiama lo script sh il quale richiama l'applicativo fp_81_driver.out passandogli come parametro il path completo di accesso ad un file xml di comunicazione.
Questo file è chiamato receipt.xml e contiene l'importo di uno scontrino oppure dei comandi da eseguire da parte del registratore fiscale.

Ad esempio per fare aprire il cassetto del misuratore fiscale l'applicativo web creerà un nuovo file "receipt.xml" con il seguente contenuto:

```xml
<?xml version="1.0"?>
<action>
  <open_drawer>
    <empty/>
  </open_drawer>
</action>
```


### Versioning ###

Il versionamento è ottenuto con git, non è disponibile la storia originale dello sviluppo ma è ripubblicato come un nuovo archivio git.

### Authors ###

