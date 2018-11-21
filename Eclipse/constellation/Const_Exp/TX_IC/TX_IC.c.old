#include <TX_IC.h>
#INCLUDE <stdlib.h>
#define RAND_MAX 255
#define BIT_PERIOD 250

void LEDInit (void)
{
   i2c_start();
   i2c_write(I2C_PORT, 0x00); // Adress
   i2c_write(I2C_PORT, 0x42); // Powerup Option
   i2c_write(I2C_PORT, 0x00); // Disable Scripts
   i2c_write(I2C_PORT, 0x00);
   i2c_write(I2C_PORT, 0x00);
   i2c_write(I2C_PORT, 0x00);
   i2c_write(I2C_PORT, 0x00);
   i2c_stop();
   delay_ms (25);
   i2c_start();
   i2c_write(I2C_PORT, 0x00); // Address
   i2c_write(I2C_PORT, 0x6E); // Change Col Immediately
   i2c_write(I2C_PORT, 0x00);
   i2c_write(I2C_PORT, 0x00);
   i2c_write(I2C_PORT, 0x00);
   i2c_stop();
   delay_ms (25);
}
void LEDSetCol (unsigned int8 RedCH, unsigned int8 GreenCH, unsigned int8 BlueCH)
{
   i2c_start();
   i2c_write(I2C_PORT, 0x00); // Address
   i2c_write(I2C_PORT, 0x6E); // Change Color Immediately
   i2c_write(I2C_PORT, RedCH);
   i2c_write(I2C_PORT, GreenCH);
   i2c_write(I2C_PORT, BlueCH);
   i2c_stop();
}
void ByteToLED (unsigned int8 Data)
{
   unsigned int8 RedCH, GreenCH, BlueCH, Parity;
   RedCH = Data & 0b11100000;
   GreenCH = (Data << 3) & 0b11100000;
   BlueCH = (Data << 6) & 0b11000000;
   Parity = RedCH ^ GreenCH ^ BlueCH;
   Parity = (Parity ^ (Parity >> 1) ^ (Parity >> 2)) & 0b00100000;
   BlueCH = BlueCH | Parity;
   
   LEDSetCol (0xFF, 0xFF, 0xFF);
   delay_ms (BIT_PERIOD);
   LEDSetCol (RedCH, GreenCH, BlueCH);
   delay_ms (BIT_PERIOD);
   putc (Data, COM_PORT);
}
void ColCycle ()
{
   LEDSetCol (0xFF, 0x00, 0x00);
   delay_ms (BIT_PERIOD);
   LEDSetCol (0x00, 0xFF, 0x00);
   delay_ms (BIT_PERIOD);
   LEDSetCol (0x00, 0x00, 0xFF);
   delay_ms (BIT_PERIOD);
   LEDSetCol (0x00, 0x00, 0x00);
   delay_ms (BIT_PERIOD);
}
void main()
{
   unsigned int8 Data, Command, Status;
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   port_b_pullups (true);
   LEDInit ();
   
   fprintf (COM_PORT, "\n\rTransmitter Started. Enter Command...\n\r");
   while (!kbhit());
   Command = fgetc (COM_PORT);
   if (Command == 'd')
   {
      Status = 1;
      fprintf (COM_PORT, "\n\rData Mode. Enter Data...\n\r");
   }
   else if (Command == 'r')
   {
      Status = 2;
      fprintf (COM_PORT, "\n\rRandom Mode. Generating Data...\n\r");
   }
   else if (Command == 'c')
   {
      Status = 3;
      fprintf (COM_PORT, "\n\rCycle Mode. Rotating Colors...\n\r");
   }
   else
      Status = 0;
      
   while (true)
   {
      if (Status == 1)
      {
         Data = getc (COM_PORT);
         ByteToLED (Data);
      }
      else if (Status == 2)
         ByteToLED (rand ());
      else if (Status == 3)
         ColCycle ();
      else
      {
         fprintf (COM_PORT, "\n\rUndefined State. Reset TX.\n\r");
         break;
      }
   }
}
