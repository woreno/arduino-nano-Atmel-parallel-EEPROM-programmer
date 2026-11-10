// definir pinos 
  #define W0 void
  #define W1 bool
  #define W8 uint8_t
  #define W32 uint32_t
  #define SW16 int16_t
  #define W16 uint16_t
  #define SHIFT_DELAY_US       delayMicroseconds(200);
  #define CLOCK_SHIFT_DELAY_US delayMicroseconds(100);
  #define WRITE_DELAY_US       delayMicroseconds(100);
  #define PIN_SERIALDATA   2
  #define PIN_CLOCK_SHIFT  3
  #define PIN_CLOCK_STORE  4
  #define PIN_DATA         5
  #define PIN_WE           13
  //#define SERIALLOG(X)           Serial.printf(X);
  #define SERIALLOG(X)           
  W8 dev_id[] = "prom-moreno";
  W8 buf[4];
  #define DEV_ID_SZ        11 

void setupDataAsOutput()
{
    for (int i=0; i< 8; i++)
      pinMode (PIN_DATA+i, OUTPUT);
}
void setupDataAsInput()
{
    for (int i=0; i< 8; i++)
      pinMode (PIN_DATA+i, INPUT);
}
void led_inform(W8 amount_)
{
    // 5 piscas ao iniciar
    for (W8 i=0; i< amount_; i++)  
    {
        digitalWrite(13, LOW);
        delay(100);  
        digitalWrite(13, HIGH);
        delay(100);  
    }
}
void setup() 
{
  // configurar pinos
  pinMode (PIN_WE,  OUTPUT);
  digitalWrite(PIN_WE, HIGH);
  pinMode (PIN_SERIALDATA,  OUTPUT);
  pinMode (PIN_CLOCK_SHIFT, OUTPUT);
  pinMode (PIN_CLOCK_STORE,  OUTPUT); 
  digitalWrite(PIN_CLOCK_SHIFT,LOW);
  digitalWrite(PIN_CLOCK_STORE,LOW);
  Serial.begin(9600, SERIAL_8N1);
  delay (200); //ms
  led_inform(5);
  SERIALLOG("\nLOG: iniciado")
}
W8 tx8(W8 byte_)
{
  Serial.write(byte_);
}
W8 rx8()
{
  //Serial.readBytes(buf,1);
  //return buf[0];
  while(1)
  {
      if (Serial.available() > 0)
        return Serial.read();
  }
}
W32 rx32()
{
   return (((W32)rx8())<<24)|(((W32)rx8())<<16)|(((W32)(rx8()))<<8)|rx8();
}

W0 setupShiftRegBit(W1 bit_)
{
    digitalWrite(PIN_SERIALDATA, bit_? HIGH:LOW);
    SHIFT_DELAY_US
    digitalWrite(PIN_CLOCK_SHIFT,LOW);
    CLOCK_SHIFT_DELAY_US
    digitalWrite(PIN_CLOCK_SHIFT,HIGH);
}

W0 setupShiftRegisters(W16 address_, W1 isWrite_)
{
    setupShiftRegBit(isWrite_); 
    for (SW16 i=14; i>-1; i--)
    {
      setupShiftRegBit((address_>>i)&1);
    }
    // colocar valores nas saídas Q
    digitalWrite(PIN_CLOCK_STORE,LOW);
    CLOCK_SHIFT_DELAY_US
    digitalWrite(PIN_CLOCK_STORE,HIGH);
}

uint8_t op;
uint32_t addr_st;
uint32_t op_size;

W0 prom_read()
{
  // em principio já está assim
  digitalWrite(PIN_WE, HIGH);
  // colocar os pinos de dados para input
  setupDataAsInput();
  for (W32 addr=addr_st; addr< op_size; addr++)
  { 
      // preparar endereço nos shift-register
      setupShiftRegisters(addr, 1);
      WRITE_DELAY_US
      // transferir databus
      W8 data = 0;
      for (W8 j=0; j<8; j++)
      {   
        data = data|(((digitalRead(PIN_DATA+j)==HIGH)?1:0)<<j);
      }
      tx8(data);
  }
  // receber um byte e
  // colocar os pinos de databus
 
  
}

W0 prom_write()
{
  // colocar os pinos de dados para output
  setupDataAsOutput();
  for (W32 addr=addr_st; addr< op_size; addr++)
  {
      // receber um byte e
      // colocar os pinos de databus
      W8 data = rx8();
      for (W8 j=0; j<8; j++)
      {   
        digitalWrite(PIN_DATA+j, (1&(data>>j))?HIGH:LOW);
      }
      // preparar endereço nos shift-register
      setupShiftRegisters(addr, 1);
      // colocar a eeprom em escrita
      digitalWrite(PIN_WE, LOW);
      WRITE_DELAY_US
      // colocar a eeprom em leitura
      digitalWrite(PIN_WE, HIGH);
  }
}

W0 loop() 
{
    Serial.flush();
    // esperar por opção
    op =  rx8();
    led_inform(2);
    Serial.write(dev_id,DEV_ID_SZ);
    // esperar inicio em bytes
     addr_st   = rx32();
    // esperar tamanho em bytes
     op_size = rx32();
    switch(op)
    {
      case 1: led_inform(20); prom_write(); break;
      case 2: case 0: led_inform(10); prom_read(); break;
    }
    Serial.write(dev_id,DEV_ID_SZ);
    led_inform(10);
}
