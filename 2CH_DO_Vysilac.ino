// 433 MHz vysílač

// připojení knihovny
#include <VirtualWire.h>

#define CH1 0
#define CH2 1

int Ch1Pin = A0;
int Ch2Pin = A1;
int Btn1Pin = 8;
int Btn2Pin = 9;

int Ch1Value = 0;
int Ch2Value = 0;

int OldCh1Value = 0;
int OldCh2Value = 0;

void setup()
{
  //nastaveni vstupnich pinu
  pinMode(Btn1Pin,INPUT);
  pinMode(Btn2Pin,INPUT);
 
   Serial.begin(9600);
   
  // nastavení typu bezdrátové komunikace
  vw_set_ptt_inverted(true);
  // nastavení čísla datového pinu pro vysílač
  vw_set_tx_pin(4);
  // nastavení rychlosti přenosu v bitech za sekundu
  vw_setup(1000);
}

void send_message(int channel, int value)
{
    char znaky [4];
    if(channel == CH1)
        snprintf(znaky, sizeof(znaky), "A%d", value);
    else
        snprintf(znaky, sizeof(znaky), "B%d", value);
    char *casZnaky = znaky;
    digitalWrite(13, true);
    vw_send((uint8_t *)casZnaky, strlen(casZnaky));
    vw_wait_tx();
    digitalWrite(13, false);
}

void loop() {

  //nacti hodnotu kanalu 1
  Ch1Value = analogRead(Ch1Pin);
Serial.print("sensor = ");
  Serial.print(Ch1Pin);
  Serial.print(" old = ");
  Serial.println(OldCh1Value);
  //pokud se hodnota lisi, odesli ji do prijimace
  if ((Ch1Value < (OldCh1Value-1)) ||(Ch1Value > (OldCh1Value+1)) )
  {
    OldCh1Value = Ch1Value;
    send_message(CH1, Ch1Value);
    //cas prijimaci na zpracovani zpravy
    delay(10);
  }
  
    //nacti hodnotu kanalu 2
  Ch2Value = analogRead(Ch2Pin);
  //pokud se hodnota lisi, odesli ji do prijimace
  if ((Ch2Value < (OldCh2Value-1)) ||(Ch2Value > (OldCh2Value+1)) )
  {
    OldCh2Value = Ch2Value;
    send_message(CH2, Ch2Value);
    //cas prijimaci na zpracovani zpravy
    delay(10);
  }
  //cas pro AD converter
  delay(20);
}
