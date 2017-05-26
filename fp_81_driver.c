#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WINDOWS
#include <windows.h>
#endif

#include "FP_driver.h"
#include "ezxml.h"
/**
 ********************************************************************
  EPSON FP90 PRINTER DRIVER, Version 11.04 
  Copyright (c) 2011  PLARZ systems www.plarz.net, 
  All Rights Reserved.

  Use and copying of this software and preparation of derivative works
  based upon this software are permitted. Any copy of this software or
  of any derivative work must include the above copyright notice, this
  paragraph and the one after it.  Any distribution of this software or
  derivative works must comply with all aplicable laws.

  This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS
  ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
  LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
  EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
  NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGES.
  ********************************************************************
*/

/*
    Per informazioni info@postouch.it

    Software sviluppato per casse digitali POS Touch
 */

int main (int argc, char* argv[])
{
    int i, n, err;
    int comport;
    int vat_memory_length = 5; //number of predefined vat values the printer can memorize
    int flag_reset = 0;
    int flag_training = 0;
    int flag_training_end = 0;
    int flag_set_vat_value = 0;
    const int obj_length = 18;
    char debugfile[10]= "debug.txt";
    char result[57]= "12345678901234567890123456789012345678901234567890123456\0";
    
    if (argc != 3)
    {
	printf("Use: %s COM N\r\n", argv[0]);
	return -1;
    }
     
    comport = atoi(argv[1]);
    if ((comport < 0) || (comport > 9))
    {
	printf("COM error!");
	return -1;
    }
    
    n = atoi(argv[2]);
    if (n <= 0)
    {
	printf("Nothing to do!");
	return 0;
    }
       
    // Open
    if ((err = OpenPrinterDriver(comport,19200,0,8,1,"abc",0,1)) < 0)
    {
	printf("ERROR [OpenPrinterDriver]: %i\r\n", err);
 	return -1;
    }
   if ((err = OpenDisplayDriver(comport,19200,0,8,1,1)) < 0)
    {
	printf("ERROR [OpenDisplayDriver]: %i\r\n", err);
 	return -1;
    }
    if ((err = OpenDrawerDriver(comport,19200,0,8,1,0)) < 0)
    {
	printf("ERROR [OpenDrawerDriver]: %i\r\n", err);
 	return -1;
    }
    printf("Open Done!, debugfile = <%s>\n", debugfile);
	       
    // Debug
    if ((err = SetDebugFileName(debugfile, 50000)) < 0)
    {
	printf("ERROR [Setdebug]: %i\r\n", err);
	return -1;
    }
    
    // stato della stampante
// restituisce: -1 = ERROR,
//               0 = OFF-LINE (BUSY/COVER-OPEN?),
//               1 = MONITOR,
//               2 = FISCAL-REC,
//               3 = FISCAL-REC_TOTAL,
//               4 = FISCAL-REC-END,
//               5 = NON-FISCAL
    // GetPrinterState
    printf("GetPrinterState(): %i\r\n", GetPrinterState());
    
    if (flag_reset > 0)
    {
	if ((err = resetPrinter()) < 0)
	{
		printf("ERROR [resetPrinter]: %i\r\n", err);
	}
	else
	{
		printf("resetPrinter(): %i\r\n", err);
	}
	return -1;
    }
    
    if (flag_set_vat_value)
    {
		if ((err = Z_report()) < 0)
		{
			printf("ERROR [Z_report ()]: %i\r\n", err);
			return -1;
		}
		int tmp_vat_val = 22*100;
		//int setVATval(int VAT_id, int VAT_val);
		if ((err = setVATval(1, tmp_vat_val)) < 0)
		{
			printf("ERROR [setVATval (%i)]: %i\r\n", tmp_vat_val, err);
			return -1;
		}
		tmp_vat_val = 0*100;
		//int setVATval(int VAT_id, int VAT_val);
		if ((err = setVATval(2, tmp_vat_val)) < 0)
		{
			printf("ERROR [setVATval (%i)]: %i\r\n", tmp_vat_val, err);
			return -1;
		}
		if ((err = setVATtable()) < 0)
		{
			printf("ERROR [setVATtable ()]: %i\r\n", err);
			return -1;
		}
		printf("Correctly updated vat table\r\n");
		return -1;
    }
     
    ezxml_t f1 = ezxml_parse_file("/var/www/postouch/receipt.xml"), open_drawer, reset_printer, fiscal, fiscal_close_report, fiscal_report, fiscal_print_display, recitem, rectotal;
    
    //open drawer, notice that if you enter here you won't enter the other loops
    for ( open_drawer = ezxml_child(f1, "open_drawer"); open_drawer; open_drawer = open_drawer->next) 
    {
	   	if ((err = OpenDrawer()) < 0)
		{
		    printf("ERROR [OpenDrawer]: %i\r\n", err);
		    return -1;
		}
		else
		{
		    printf("correctly opened drawer\r\n");
		}
    }
    
    //reset printer, notice that if you enter here you won't enter the other loops
    for ( reset_printer = ezxml_child(f1, "reset_printer"); reset_printer; reset_printer = reset_printer->next) 
    {
	if ((err = resetPrinter()) < 0)
	{
		printf("ERROR [resetPrinter]: %i\r\n", err);
	}
	else
	{
		printf("resetPrinter(): %i\r\n", err);
	}
	return -1;
    }
    
    //close the fiscal day, notice that if you enter here you won't enter the other loops
    for ( fiscal_close_report = ezxml_child(f1, "fiscal_close_report"); fiscal_close_report; fiscal_close_report = fiscal_close_report->next) 
    {
	     	if ((err = Z_report()) < 0)
 		{
 		    printf("ERROR [Z_report ()]\r\n");
 		    return -1;
 		}
 		if ((err = WriteDisplay("Chiusura Fiscale","")) < 0)
		{
			printf("ERROR [WriteDisplay]: %i\r\n", err);
			return -1;
		}
    }
    
    //print a fiscal report, notice that if you enter here you won't enter the other loops
    for ( fiscal_report = ezxml_child(f1, "fiscal_report"); fiscal_report; fiscal_report = fiscal_report->next) 
    {
	     	if ((err = X_report()) < 0)
 		{
 		    printf("ERROR [X_report ()]\r\n");
 		    return -1;
 		}
 		if ((err = WriteDisplay("Rapporto Fiscale","")) < 0)
		{
			printf("ERROR [WriteDisplay]: %i\r\n", err);
			return -1;
		}
    }
    
    //print on the display an item which has just been swiped, notice that if you enter here you won't enter the other loops
    for ( fiscal_print_display = ezxml_child(f1, "fiscal_print_display"); fiscal_print_display; fiscal_print_display = fiscal_print_display -> next) 
    {
	for ( recitem = ezxml_child(fiscal_print_display, "recitem"); recitem; recitem = recitem -> next)
	{
	        const char* xml_obj = ezxml_attr(recitem, "obj");
		char obj[24]={};
		int j = 0;
		while(*xml_obj != '\0' && j < obj_length-1)
		{
		  obj[j] = *xml_obj;
		  xml_obj++;
		  j++;
		}
		
		const char* xml_quant = ezxml_attr(recitem, "quant");
		int quant = atoi(xml_quant);
		const char* xml_price = ezxml_attr(recitem, "price_vat"); //on the display show vat included
		long price = atol(xml_price);
		printf("%s %i %ld\n", obj, quant, price);
		double print_price = price/10000.0;
		char print_price_s[10];
		//ltoa(print_tot, print_tot_s, 10); //ltoa deprecated
		snprintf(print_price_s, sizeof(print_price_s), "%2.2f", print_price);
		
  		if ((err = WriteDisplay(obj, print_price_s)) < 0)
  		{
  		    printf("ERROR [WriteDisplay]: %i\r\n", err);
  		    return -1;
  		}
	}
	for ( rectotal = ezxml_child(fiscal_print_display, "total"); rectotal; rectotal = rectotal->next)
	{
 		const char* xml_total = ezxml_attr(rectotal, "total");
 		long total = atol(xml_total);
 		printf("rectotal: %ld\n", total);
		double print_tot = total / 10000.0;
		char print_tot_s[10];
		//ltoa(print_tot, print_tot_s, 10); //ltoa deprecated
		snprintf(print_tot_s, sizeof(print_tot_s), "%2.2f", print_tot);
		
  		if ((err = WriteDisplay("Totale Euro:",print_tot_s)) < 0)
  		{
  		    printf("ERROR [WriteDisplay]: %i\r\n", err);
  		    return -1;
  		}
	}
    }

    /* print a fiscal receipt of all items, notice that if you enter here you haven't entered the previous loops
       below an example of correct xml file
       <?xml version="1.0"?>
	<receipt>
		<fiscal>
		<recitem obj="7 NANI 25 cm DOPEY" quant="1000" price_unvat="140800" discount="20" vat="20.00" price_vat="168960" uid="4de9f881b1533"/>
		<total total="135200" payments="135200"/>
		</fiscal>
	</receipt> 
     */
    for ( fiscal = ezxml_child(f1, "fiscal"); fiscal; fiscal = fiscal->next) 
    {
	    if (flag_training == 1)
	    {
		if ((err = BeginTraining()) < 0)
		{
			printf("ERROR [BeginTraining]: %i\r\n", err);
			return -1;
		}
		else
		{
			printf("correctly entered training mode");
		}
	    }
	    if ((err = BeginFiscalRec()) < 0)
	    {
			printf("ERROR [BeginFiscalRec]: %i\r\n", err);
			return -1;
	    }
	    for ( recitem = ezxml_child(fiscal, "recitem"); recitem; recitem = recitem->next)
	    {
	        const char* xml_obj = ezxml_attr(recitem, "obj");
		char obj[24]={};
		int j = 0;
		while(*xml_obj != '\0' && j < obj_length-1)
		{
		  obj[j] = *xml_obj;
		  xml_obj++;
		  j++;
		}
		
		const char* xml_quant = ezxml_attr(recitem, "quant");
		int quant = atoi(xml_quant);
		const char* xml_price = ezxml_attr(recitem, "price_vat"); //price with vat!
		long price = atol(xml_price);
		const char* xml_vat = ezxml_attr(recitem, "vat");
		int vat = atoi(xml_vat);
		if (vat != 0)
			vat = 22;
		printf("Vat set for now at %i\r\n", vat);
		//discount is a percentage, for instance 25% corresponds to discount=25
		const char* xml_discount = ezxml_attr(recitem, "discount");
		int discount = atoi(xml_discount);
		long discounted_price = 0.0; //amount of discount, if price_vat=10000 and discount=20 discounted_price=2000
		if (discount > 0.0)
		{
			//prices are always multiplied by 10000 for precision, hence the strange formula!
			discounted_price = (price / (1000000.0))*(discount*10000);
			printf("discounted_price %ld\r\n", discounted_price);
		}
		
		int vat_id = -1;
		for (i=1; i<vat_memory_length; i++)
		{
			if (getVATval(i) == vat*100)
			{
				vat_id = i; //ok we found to which id corresponds the given vat, the fp90 supports ids from 1 to 4
				printf("vat found vat_id= %i\r\n", vat_id);
				break;
			}
		}
		
		if (vat_id == -1)
		{
			printf("ERROR [getVATval ()], couldn't find the given vat %i!\r\n", vat);
			return -1;
		}
		
		printf("%s quant: %i price: %ld discount: %i vat: %i vat_id: %i discounted_price: %ld\n", obj, quant, price, discount, vat, vat_id, discounted_price);
		
 		if (discount > 0)
		{
			if ((err = RecItem(obj, quant, price, vat_id, "", "")) < 0)
			{
				printf("ERROR [RecItem (%s)]: %i\r\n", obj, err);
				return -1;
			}
			//int RecItemDiscount(char description[], long price, int VAT_id, char PreLine[]);
			if ((err = RecItemDiscount("- sconto", discounted_price, vat_id,"")) < 0)
			{
				printf("ERROR [RecItemDiscount (%s)]: %i\r\n", obj, err);
				return -1;
			}
		}
		else
		{
			if ((err = RecItem(obj, quant, price, vat_id, "", "")) < 0)
			{
				printf("ERROR [RecItem (%s)]: %i\r\n", obj, err);
				return -1;
			}
		}
	    }
	    
	    for ( rectotal = ezxml_child(fiscal, "total"); rectotal; rectotal = rectotal->next)
	    {
		const char* xml_payments = ezxml_attr(rectotal, "payments");
 		long payments = atol(xml_payments);
		const char* xml_total = ezxml_attr(rectotal, "total");
 		long total = atol(xml_total);
 		printf("rectotal: %ld\n", total);
		
// 		if ((err = RecSubtotal("",34800,1)) < 0)
//   		{
//   		    printf("ERROR [RecSubtotal]: %i\r\n", err);
//   		    return -1;
//   		}
  		if ((err = RecTotal("0",payments,"",total,0)) <= 0)
  		{
  		    printf("ERROR [RecTotal]: %i\r\n", err);
  		    return -1;
  		}
		if ((err = PrintFiscal("           ")) < 0)
  		{
  		    printf("ERROR [PrintFiscal]: %i\r\n", err);
  		    return -1;
  		}
  		if ((err = PrintFiscal("powered by PLARZ POS 12.01")) < 0)
  		{
  		    printf("ERROR [PrintFiscal]: %i\r\n", err);
  		    return -1;
  		}
  		if ((err = PrintFiscal("running on Ubuntu Linux 11.10")) < 0)
  		{
  		    printf("ERROR [PrintFiscal]: %i\r\n", err);
  		    return -1;
  		}
  		if ((err = OpenDrawer()) < 0)
		{
		    printf("ERROR [OpenDrawer]: %i\r\n", err);
		    return -1;
		}
		double print_tot = total /10000.0;
		char print_tot_s[10];
		//ltoa(print_tot, print_tot_s, 10); //ltoa deprecated
		snprintf(print_tot_s, sizeof(print_tot_s), "%2.2f", print_tot);
		
  		if ((err = WriteDisplay("Totale Euro:",print_tot_s)) < 0)
  		{
  		    printf("ERROR [WriteDisplay]: %i\r\n", err);
  		    return -1;
  		}
	    }
	//we exit the fiscal state and thus close the receipt    
	if ((err = EndFiscalRec()) < 0)
	{
		printf("ERROR [EndFiscalRec]: %i\r\n", err);
		return -1;
	}
    }
    
    if (flag_training_end == 1)
    {
	if ((err = EndTraining()) < 0)
	{
		printf("ERROR [EndTraining]: %i\r\n", err);
		return -1;
	}
    }
    
    // GetPrinterState
    printf("GetPrinterState(): %i\r\n", GetPrinterState());
    CloseDrawerDriver();
  
    ezxml_free(f1);
    ClosePrinterDriver();  
    return 1; 
}
