/*
Epson Italia s.p.a.
via M. Vigano' De Vizzi, 93/95
20092 Cinisello Balsamo (MI), Italia
www.epson.it

Tutti i diritti di questo documento sono riservati alla Epson Italia, 
nessuna parte di esso puo` essere riprodotta, trascritta o tradotta in 
qualsiasi forma, mezzo elettronico o meccanico, senza il permesso scritto 
della Epson Italia.

nome:           "FP_driver.h"

identificatore: EPSON_CASH_PRJ-SRC-EPSON-SHARED_LIBRARY-FP_DRIVER.H

autore:         Giorgio Meini
note:           interfaccia "C" della componente di basso livello del driver
data v0.0:      8 Luglio 2006

versione:       0.0
				0.1 (05-09-2006):	+ funzioni RecItemSurcharge(), RecSubtotalSurcharge()
				0.2 (01-10-2006):   ++ CRadaelli aggiunto 57600
				0.3 (19-02-2007):   ++ CRadaelli  added GetNotPaidAmount
  
*/


    
/* APERTURA DEVICE-DRIVER */
// port: 0, 1, 2, 3, 4, 5 (-> /dev/ttyS0, /dev/ttyS1, ...)
// baudrate: 4800, 9600, 19200, 38400, 57600
// parity: 0 (none), 1 (odd), 2 (even)
// databit: 7, 8
// stopbit: 1, 2
// return: -1: ERROR, >=0: OK 

// sovrascrittura stampante (0 = no, 1 = yes)
int OpenDisplayDriver(int port, int baudrate, int parity, int databit, int stopbit, int overwrite); // test GM 08-07-2006

// sensore cassetto aperto (0 = no, 1 = yes)
int OpenDrawerDriver(int port, int baudrate, int parity, int databit, int stopbit, int sensor);

// POSID: identificatore POS (max 3 car.),
// totali giornalieri in chiusura fiscale (0 = no, 1 = yes),
// righe pagamento predeterminate (0 = no, 1 = yes)
// restituisce le capability del firmware (0 -> NO subtotale, 1 -> subtotale e verifica totale, 2 -> subtotale, verifica totale e messaggi aggiuntivi, 3-> subtotale, verifica totale, messaggi aggiuntivi e maggiorazioni)
int OpenPrinterDriver(int port, int baudrate, int parity, int databit, int stopbit, char POSID[], int Z_day_total, int fixed_payment_line); // test GM 08-07-2006
    
/* CHIUSURA DEVICE-DRIVER */
void CloseDisplayDriver(void); // test GM 08-07-2006
void CloseDrawerDriver(void);
void ClosePrinterDriver(void); // test GM 08-07-2006
    
    
/* DISPLAY LCD */

// visualizza le 2 linee del display LCD (max 20 car. per linea)
int WriteDisplay(char line0[], char line1[]); // test GM 08-07-2006


/* CASSETTO VALUTA */

// acquisisce lo stato del cassetto
 // restituisce: 1 = OPEN, 0 = CLOSE, -1 = ERROR
int GetDrawerState(void); // test GM 09-07-2006

// apre il cassetto
 // restituisce: 0 = OK, -1 = ERROR
int OpenDrawer(void); // test GM 09-07-2006

// attende la chiusura del cassetto per un massimo periodo di tempo espresso in millisecondi
// restituisce: 0 = OK, -1 = TIMEOUT/ERROR
int WaitDrawerClose(int millisec); // test GM 09-07-2006

    
/* PROPRIETA' STAMPANTE FISCALE (SET/GET) */

// impostazione intestazione aggiuntiva (Epson 1.078/1)
// restituisce: -1 = ERROR, 0 = OK
int SetAdditionalHeader(char text[]);

// impostazione messaggio aggiuntivo (Epson: 1.078/3)
// restituisce: -1 = ERROR, 0 = OK
int SetAdditionalTrailer(char text[]);

// apertura giornata fiscale (Epson: 1.070)
// restituisce: 1 = OPEN, 0 = CLOSE, -1 = ERROR
int GetFiscalDayState(void); // test GM 08-07-2006

// stato giornale elettronico (Epson: 1.074)
// restituisce: -1 = ERROR, 0 = EMPTY, 1 = NEAR_END, 2 = OK
int GetJournalState(void); // test GM 08-07-2006

// stato carta scontrini (Epson: 1.074)
// restituisce: -1 = ERROR, 0 = EMPTY, 1 = NEAR_END, 2 = OK
int GetReceiptState(void); // test GM 08-07-2006

// memoria fiscale rimanente (Epson: 2.052)
// restituisce: -1 = ERROR, >=0 = OK (value)
int GetFreeFiscalMemory(void); // test GM 08-07-2006

// stato attivazione training-mode (Epson: 4.214)
// restituisce: -1 = ERROR, 0 = NO_TRAINING_MODE, 1 = TRAINING_MODE 
int GetTrainingModeState(void);

// valore tabella IVA (Epson: 4.205)
// parametri:  0 < VAT_id <= 4
// restituisce: %*100
int getVATval(int VAT_id);
        
/* METODI STAMPANTE FISCALE, restituiscono:

       <0 = ERROR:
       
        -1 = ILLEGAL
        -2 = OFF_LINE
        -3 = FAILURE
        -4 = TIME_OUT
        -5 = BUSY
       -11 = COVER_OPEN  
       -12 = JRN_EMPTY 
       -13 = REC_EMPTY
       -14 = WRONG_STATE
       -15 = FISCAL_MEMORY_FULL
       -16 = FISCAL_MEMORY_DISCONNECTED
       -17 = BAD_QUANTITY
       -18 = BAD_LENGHT
       -19 = BAD_AMOUNT
       -20 = BAD_DESCRIPTION
       -21 = BAD_VAT
       -22 = BAD_PRICE
       -23 = BAD_DATE
       -24 = REC_TOTAL_OVERFLOW
       -25 = REC_NEGATIVE_TOTAL
       -26 = WORD_NOT_ALLOWED
       -27 = TECHNICAL_ASSISTANCE
*/
    
// impostazione valore tabella IVA
// parametri: 0 < VAT_id <= 4, VAT_val: %*100
int setVATval(int VAT_id, int VAT_val); 

// memorizzazione tabella IVA (Epson: 4.005)
int setVATtable(void); 

// impostazione data-ora (Epson: 4.001)
// parametri: date_time: "YYYYMMDDhhmm"
int SetPrinterDate(char date_time[]);

// impostazione intestazione scontrino (Epson: 3.016 + 4.016)
// parametri: line: 1-6; type = 1 -> normal, type = 2 -> double width
int SetHeaderLine(int line, char text[], int type);

// impostazione chiusura scontrino (Epson: 1.078/2)
// parametri: line: 1-12; type = 1 -> normal, type = 2 -> double width
int SetTrailerLine(int line, char text[], int type);

// inizio scontrino fiscale (Epson: 1.085)
int BeginFiscalRec(void); // test GM 08-07-2006

// fine scontrino fiscale (Epson: 1.074)
int EndFiscalRec(void); // test GM 08-07-2006

// duplicazione scontrino (Epson: 1.047)
int DuplicateRec(void);

// vendita (Epson: 1.080)
// parametri: quantity*1000, unit_price: Euro*10000, 0 <= VAT_id <= 4
int RecItem(char description[], int quantity, long unit_price, int VAT_id, char PreLine[], char PostLine[]); // test GM 08-07-2006 

// sconto (Epson: 1.083)
// parametri: price: Euro*10000, 0 <= VAT_id <= 4 
int RecItemDiscount(char description[], long price, int VAT_id, char PreLine[]);

// maggiorazione (Epson: 1.083)
// parametri: price: Euro*10000, 0 <= VAT_id <= 4 
int RecItemSurcharge(char description[], long price, int VAT_id, char PreLine[]);

// sconto su subtotale (Epson: 1.083)
// parametri: amount: Euro*10000
int RecSubtotalDiscount(char description[], long amount, char PreLine[]);

// maggioreazione su subtotale (Epson: 1.083)
// parametri: amount: Euro*10000
int RecSubtotalSurcharge(char description[], long amount, char PreLine[]);

// annullo sconto su subtotale (Epson: 1.027)
// parametri: amount: Euro*10000
int RecSubtotalDiscountVoid(long amount, char PreLine[]);

// reso (Epson: 1.081)
// parametri: quantity*1000, unit_price: Euro*10000, 0 <= VAT_id <= 4
int RecRefund(char description[], int quantity, long unit_price, int VAT_id, char PreLine[]);

// annullo reso (Epson: 1.080)
// parametri: quantity*1000, unit_price: Euro*10000, 0 <= VAT_id <= 4
int RecRefundVoid(char description[], int quantity, long unit_price, int VAT_id);

// subtotale (Epson: 1.086)
// restituisce: 0 = OK, 1 = CHECK-ERR -> scontrino annullato
// parametri: amount: Euro*10000; checktot: 0/1 (false/tre)
int RecSubtotal(char PostLine[], long amount, int checktot);

// pagamento (Epson: 1.084)
// restituisce: 0 = payment < total, return 1 = payment >= total, return 2 = CHECK-ERR -> scontrino annullato
// parametri: description: "0" (cash), "1" (check), "2" (credit-card), "3" (ticket), payment, amount: Euro*10000, checktot: 0/1 (false/tre)
int RecTotal(char description[], long payment, char PostLine[], long amount, int checktot); // test GM 08-07-2006

// corrispettivo non pagato (Epson: 1.084)
// restituisce: 0 = payment < total, return 1 = payment >= total
// parametri: nopayment: Euro*10000
int RecNotPaid(char description[], long nopayment);

// annullo scontrino (Epson: 1.028)
int RecVoid(void);

// annullo vendita (Epson: 1.082)
// parametri: quantity*1000, unit_price: Euro*10000, 1 <= type <= 4, adjustment: Euro*10000, 0 <= VAT_id <= 4
int RecVoidItem(char description[], int quantity, long unit_price, int type, long adjustment, int VAT_id);

// stampa totali periodici (Epson: 3.014)
// parametri: date_time: "YYYYMMDDhhmm" 
int PeriodicTotalsReport(char date_time0[], char date_time1[]);

// stampa rapporto tra indici (Epson: 3.012/3.013)
int OrdinalReport(int n0, int n1);

// stampa rapporto tra date
// parametri: date_time: "YYYYMMDDhhmm
int DateReport(char date_time0[], char date_time1[]);

// stampa rapporto di tipo X (Epson: 2.001)
int X_report(void); // test GM 08-07-2006

// chiusura fiscale (Epson: 3.001)
int Z_report(void); // test GM 08-07-2006

// inizio socntrino non fiscale (Epson: 1.063)
int BeginNonFiscal(void); // test GM 08-07-2006

// inizio modalita' training (Epson: 4.014/1)
int BeginTraining(void); //test GM 08-07-2006

// fine scontrino non fiscale (Epson: 1.065)
int EndNonFiscal(void); // test GM 08-07-2006

// fine modalita' training
int EndTraining(void); // test GM 08-07-2006

// stampa riga non fiscale (Epson: 1.064)
int PrintNonFiscal(char text[]); // test GM 08-07-2006
// stampa riga non fiscale in scontrino fiscale
int PrintFiscal(char text[]);

// stampa messaggi chiusura scontrino (Epson: 1.078/2)
int RecMessage(char text[]);
    
/*
  metodi GET-DATA
  restitusicono una stringa ("" = ERROR)
  [test GM 08-07-2006]
*/
  
void GetFirmware(char* string); // FIRMWARE (1.074)
void GetFiscal_ID(char* string); // PRINTER_ID (3.217)
void GetDailyTotal(char* string); // DAILY_TOTAL (2.050/28): Euro*100
void GetGrandTotal(char* string); // GRAND_TOTAL (2.052): Euro*100
void GetFiscalRecVoidNumber(char* string); // MID_VOID (2.051/05)
void GetFiscalRecNumber(char* string); // RECEIPT_NUMBER (2.051/24)
void GetRefundTotal(char* string); // REFUND (2.051/02): Euro*100
void GetDailyFiscalRecNumber(char* string); // FISCAL_REC (1.070)
void GetDailyFiscalRecVoidNumber(char* string); // FISCAL_REC_VOID (2.050/05)
void GetDailyNonFiscalRecNumber(char* string); // NONFISCAL_REC (2.050/26)
void GetZreportNumber(char* string); // Z_REPORT (2.052)
void GetCurrentTotal(char* string); // CURRENT_TOTAL (1.051): Euro*100
void GetNotPaidAmount(char* string); // CURRENT_TOTAL (1.051): Euro*100

void GetPrinterDate(char* date); // DATE (4.201): "YYYYMMDDhhmm"

/*
  metodi GET-TOTALIZER (Epson: 2.050/2.051)
  restituiscono una valuta (Euro*100) in formato ASCII ("" = ERROR)
  [test GM 08-07-2006]
*/  
void GetDayTotalizer(int VAT_id, char* tot); // 0 < VAT_id <= 4
void GetGrandTotalizer(int VAT_id, char* tot); // 0 < VAT_id <= 4
void GetDayDiscountTotalizer(char* tot);
void GetGrandDiscountTotalizer(char* tot);
void GetDayRefundTotalizer(char* tot);
void GetGrandRefundTotalizer(char* tot);
void GetDayVoidTotalizer(char* tot);
void GetGrandVoidTotalizer(char* tot);
    
// reset della stampante
int resetPrinter(void); // test GM 08-07-2006
    
// accesso diretto ai comandi della stampante
// il parametro header e' la forma ASCII di un numero intero >1000 e <9999 (codifica Epson del comando da eseguire)
// il parametro text e' una stringa ASCII (parametri del comando Epson da eseguire)
// la funzione restituisce una stringa:
// i primi 4 caratteri rappresentano in formato ASCII la codifica Epson del comando eseguito ("" in caso di errore, "0000" in caso di errore Epson)
// i caratteri successivi rappresentano i dati restituiti come risultato del comando
void Direct_IO(char header[], char text[], char* result); // test GM 08-07-2006

// invio diretto di dati alla stampante
// invia sulla porta seriale esattamente DIM byte ricevuti come argomento DATA
// restituisce: -1 = ERROR
int Raw_IO(int dim, unsigned char data[]);

// stato della stampante
// restituisce: -1 = ERROR,
//               0 = OFF-LINE (BUSY/COVER-OPEN?),
//               1 = MONITOR,
//               2 = FISCAL-REC,
//               3 = FISCAL-REC_TOTAL,
//               4 = FISCAL-REC-END,
//               5 = NON-FISCAL
int GetPrinterState(void); // test GM 08-07-2006	

// AM270110
int SetDebugFileName(char text[], long size);

int GetDrawerStateThread(void);
int SetTrailerFileName(char* text);
int SetPOSID(int pos, int op);
int GetPrinterStateThread(void);
int OpenElectronicJournalDriver(int port, int baudrate, int parity, int databit, int stopbit, int notused);
int CloseElectronicJournalDriver(void);
int InitializeMedium(void);
void GetStatus(char* string);
void GetStatusThread(char* string);
int PrintContentByDate(char* fromMarker, char* toMarker);
int PrintContentByNumber(char* fromMarker, char* toMarker);
int QueryContentByDate(char* result, char* fromMarker, char* toMarker, int inc);
int QueryContentByNumber(char* result, char* fromMarker, char* toMarker, int inc);
int RecItemVoid(char* description, int quantity, long unit_price, int VAT_id, char* PreLine, char* PostLine);
int GetEJData(char* result, char* Zreport, int inc);
int GetEJDownloadState(void);
void GetPrinterInformation(char* string);
int BeginFiscalDocument(long amount, int kind);
int BeginInsertion(long timeout);
int EndInsertion(void);
int PrintFiscalDocumentLine(char* documentLine);
int BeginRemoval(long timeout);
int EndRemoval(void);
int EndFiscalDocument(void);
void GetFiscalDocNumber(char* string);
void GetFiscalDocVoidNumber(char* string);