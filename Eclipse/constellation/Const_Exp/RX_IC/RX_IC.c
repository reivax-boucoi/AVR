#include <RX_IC.h>
#INCLUDE <stdlib.h>
#define RAND_MAX 255
#define BIT_DELAY 250

unsigned int16 ReadR, ReadG, ReadB;
unsigned int8 RedCH, GreenCH, BlueCH, Data, Parity;

void PDInit ()
{
   output_low (GR0);
   output_low (GR1);
   
   output_low (GG0);
   output_low (GG1);
   
   output_high (GB0);
   output_high (GB1);
   
   output_low (WLED);
}
void ReadChannels ()
{
   set_adc_channel (0);
   // delay_us (10); 
   ReadR = read_adc ();
   
   set_adc_channel (1);
   // delay_us (10); 
   ReadG = read_adc ();

   set_adc_channel (2);
   // delay_us (10); 
   ReadB = read_adc ();
}   
unsigned int8 MakeDesc (unsigned int16 RawAn)
{
   unsigned int8 DescDi;
   if ((RawAn > 0x0F00) & (RawAn <= 0x2E00)) { DescDi = 0b11100000; }
   else if ((RawAn > 0x2E00) & (RawAn <= 0x4D00)) { DescDi = 0b11000000; }
   else if ((RawAn > 0x4D00) & (RawAn <= 0x6BC0)) { DescDi = 0b10100000; }
   else if ((RawAn > 0x6BC0) & (RawAn <= 0x8A60)) { DescDi = 0b10000000; }
   else if ((RawAn > 0x8A60) & (RawAn <= 0xA960)) { DescDi = 0b01100000; }
   else if ((RawAn > 0xA960) & (RawAn <= 0xC840)) { DescDi = 0b01000000; }
   else if ((RawAn > 0xC840) & (RawAn <= 0xE6A0)) { DescDi = 0b00100000; }
   else if ((RawAn > 0xE6A0) & (RawAn <= 0xFFC0)) { DescDi = 0b00000000; }
   else { DescDi = rand() & 0b11100000; }
   return DescDi;
}
void RecSymbol ()
{
   RedCH = MakeDesc (ReadR);
   GreenCH = MakeDesc (ReadG);
   BlueCH = MakeDesc (ReadB);
   
   Data = (RedCH | (GreenCH >> 3) | ((BlueCH & 0b11000000) >> 6));
   Parity = RedCH ^ GreenCH ^ (BlueCH & 0b11000000);
   Parity = (Parity ^ (Parity >> 1) ^ (Parity >> 2)) & 0b00100000;
   if (Parity != (BlueCH & 0b00100000))
   {
      output_high (WLED);
      delay_ms (25);
      output_low (WLED);
   }
}
void main()
{
   unsigned int8 Status, Command;
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_adc (ADC_CLOCK_INTERNAL);
   setup_adc_ports(ALL_ANALOG);
   set_tris_c (0b10000000);
   set_tris_e (0b000);
   PDInit ();
   Status = 0;
     
   while(true)
   {
      while (!kbhit ())
      {
         if (Status > 0)
         {
            ReadChannels ();
            if ((ReadR <= 0x0200) & (ReadG <= 0x0200) & (ReadB <= 0x0200))
            {
               output_low (PIN_E0);
               delay_ms (BIT_DELAY);
               ReadChannels ();
               output_high (PIN_E0);
               RecSymbol ();
               if (Status == 1)
                  fprintf (COM_PORT, "%c", Data);
               else if (Status == 2)
                  fprintf (COM_PORT, "(%LX,%LX,%LX,%X)\n\r", ReadR, ReadG, ReadB, Data);
               else if (Status == 3)
                  fprintf (COM_PORT, "[\\/\\]");
               else
                  fprintf (COM_PORT, "");
            }
         }
         else if (Status == 0)
         { 
            output_high (WLED);
            delay_ms (125);
            output_low (WLED);
            delay_ms (125);
         }
      }
      Command = getc ();
      fprintf (COM_PORT, "\n\r");
      if (Command == 's')
         Status = 0;
      else if (Command == 'd')
         Status = 1;
      else if (Command == 'r')
         Status = 2;
      else if (Command == 'c')
         Status = 3;
      else
         Status = 4;
   }
}
