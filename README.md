# EPSON fiscal printer driver
Questo driver permette di comunicare con stampanti fiscali Epson, è ottenuto da codice liberamente distribuito da Epson Italia snc
e modificato per lavorare correttamente con stampanti fiscalizzate come la Epson FP90 e Epson FP81

### Introduzione ###

Il progetto completo POS Touch sarà disponibile in futuro in GitHub, per ora è disponibile il driver per stampanti epson modificato per funzionare con la Epson FPXX ed il codice che permette all'applicativo Web scritto in codeigniter di richiamare il driver.
 
### Installazione ###

Il file fp_81_driver.c va compilato con il compilatore gc, l'esecuzione avviene tramite script "epson_print.sh"

### Funzionamento ###

Il driver C è sviluppato in maniera da poter essere utilizzato da applicativo web come POS Touch (progetto svilupatto in CodeIgniter), quest'ultimo richiama lo script "epson_print.sh" il quale richiama l'applicativo fp_81_driver.out passandogli come parametro il path completo di accesso ad un file xml di comunicazione.
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

per ottenere il file xml sopra in CodeIgniter utilizzo il seguente codice:

```php
function open_drawer()
	{
		/* output receipt to xml file for external thermal receipt printers*/
		$doc = new DOMDocument();
		$doc->formatOutput = true;
		
		$r = $doc->createElement( "action" );
		$doc->appendChild( $r );
		$f = $doc->createElement( "open_drawer" );
		$g = $doc->createElement( "empty" );
		$f->appendChild( $g );
		$r->appendChild( $f );
		$doc->saveXML();
		$filename = APPPATH.'../receipt.xml';
		if (is_writable($filename)) 
		{
			if (!$handle = fopen($filename, 'w')) 
			{
				$data['error']= "Cannot open ($filename)";
				exit;
			}
			if (fwrite($handle, $doc->saveXML()) === FALSE) 
			{
				$data['error']= "Cannot write ($filename)";
				exit;
			}
			fclose($handle);
		} 
		else
		{
			$data['error']= "Cannot write ($filename)";
		}
		if ($this->Appconfig->get('printer_model')=="fp90")
		{
			$fp_90_print = `/bin/bash /var/www/postouch/application/libraries/fp_90/epson_print.sh`;
		}
		$this->_reload();
	}
```

### Versioning ###

Il versionamento è ottenuto con git, non è disponibile la storia originale dello sviluppo ma è ripubblicato come un nuovo archivio git.

### Authors ###

