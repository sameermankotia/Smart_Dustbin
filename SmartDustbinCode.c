//Tx data to PC
#include<string.h>

void usart_initialize()
{
 UCSRB=0x08; //Tx Enable
 UCSRC=0x86; // Data Size : 8-bit, Stop Bit:1,No parity
 UBRRL=0x67; // X= (Fosc/(16(Desired Baud Rate)))-1
            //     =(8*10^6/(16 *9600))-1
            //     =52.08-1
            //     =51 (Dec)
            //Here, URSEl=0, so Fosc is divided by 16 if it was 1
  UCSRA.B1=1;
}

//USART Send
void usart_send(unsigned char ch)
{
  while(UCSRA.B5==0); // Wait till UDR is empty
  UDR=ch; //Write the value to be Tx
}

void send_SMS(unsigned char str[], int str_len)
 {
      int i=0;
      int cnt = 1;

      while(cnt != 5)
      {
        usart_send(str[i++]);  //Send data
        if (i>=str_len)  // Till string Length
        {
          i=0;
          usart_send(26);
          delay_ms(2000);
          cnt = 5;
          //break;
        }
        delay_ms(300);
      }
 }

void main() {
int j=1;
int k=0;
// unsigned char str[]="AT\r\nAT+CSMINS?\r\nAT+CREG?\r\nATE1\r\nAT+CMGF=1\r\nAT+CSCS=\"GSM\"\r\nAT+CSMP=17,167,0,16\r\nAT+CMGS=\"+91**********\"\rALERT!\nDustbin: 001 is filled...\n";

 unsigned char str[]="AT\r\nATE1\r\nAT+CMGF=1\r\nAT+CSCS=\"GSM\"\r\nAT+CSMP=17,167,0,16\r\nAT+CMGS=\"+91**********\"\rALERT!\nDustbin: 001 is filled...\n";
 int str_len=strlen(str);

unsigned char str1[]="AT\r\nATE1\r\nAT+CMGF=1\r\nAT+CSCS=\"GSM\"\r\nAT+CSMP=17,167,0,16\r\nAT+CMGS=\"+91**********\"\rGarbage collected from Dustbin 001.\n";

//int str_len=strlen(str);
int str_len1=strlen(str1);

  DDRB = 0x00;
  DDRC = 0x03;
  DDRA= 0x03;
  DDRD= 0x00;

  while(1)
  {
      if(PINB.B0 == 1)
      {
            PORTA= 0x00;
            PORTC.B0 = 1;     //buzzer ON
            PORTC.B1 = 1;     //red led ON
            PORTC.B2 = 0;     //green led off
            Delay_ms(4000);
            PORTC.B0 = 0x00;      ///buzzer off

            if(j == 1)
            {
               usart_initialize();
               send_SMS(str, str_len);
               j=0;
               k=1;
            }
      }
      else
      {
          if(k == 1)
          {
             usart_initialize();
             send_SMS(str1, str_len1);
             k=0;
          }
            
          PORTC.B0 = 0;    //buzzer off
          PORTC.B1 = 0;    //red led off
          PORTC.B2 = 1;       //green ON
          j=1;
      }

      if(PINB.B1 == 1){
          PORTA= 0x02;
          delay_ms(1300);
          PORTA= 0x00;
          delay_ms(1300);
          PORTA= 0x01;
          delay_ms(1300);
          PORTA= 0x00;
      }
      else{
           PORTA= 0x00;
      }
  }
}
