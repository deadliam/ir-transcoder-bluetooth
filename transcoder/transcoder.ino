/* Decode the IR Remote Control signal and display 
the button/key name on the serial monitor using the 
IRremote: IRrecvDemo https://www.pjrc.com/teensy/td_libs_IRremote.html. 
www.maxphi.com April, 2017  
*/

#include <IRremote.h>

int RECV_PIN = 11;
int BLUETOOTH_PLAY_PIN = 6;
int BLUETOOTH_NEXT_PIN = 4;
int BLUETOOTH_PREV_PIN = 5;
int BLUETOOTH_VOLUP_PIN = 2;
int BLUETOOTH_VOLDOWN_PIN = 7;

bool phonePressShort = false;
bool phonePressLong = false;
bool phoneUpPressLong = false;
bool phoneDownPressLong = false;

int keyNumber = 0;
unsigned long keys[6] = {0x9D62807F, 0x9D6240BF, 0x9D62C03F, 0x9D6220DF, 0x9D62A05F, 0x9D62609F};

bool device = true; // Default Bluetooth == true

unsigned long lastCode;

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(BLUETOOTH_PLAY_PIN, OUTPUT);
  pinMode(BLUETOOTH_NEXT_PIN, OUTPUT);
  pinMode(BLUETOOTH_PREV_PIN, OUTPUT);
  pinMode(BLUETOOTH_VOLUP_PIN, OUTPUT);
  pinMode(BLUETOOTH_VOLDOWN_PIN, OUTPUT);

  digitalWrite(BLUETOOTH_PLAY_PIN, LOW);
  digitalWrite(BLUETOOTH_NEXT_PIN, LOW);
  digitalWrite(BLUETOOTH_PREV_PIN, LOW);
  digitalWrite(BLUETOOTH_VOLUP_PIN, LOW);
  digitalWrite(BLUETOOTH_VOLDOWN_PIN, LOW);
}

void loop() {
  if (irrecv.decode(&results)) {
//    Serial.println(results.value, HEX);

    // ----------------------------------
    if(results.value == 0xB54AE916 && phonePressShort == false)
    {
//      phonePressShort = true;
      Serial.println("phonePressShort = true");
      delay(150);
    }
    if(lastCode != results.value && results.value == 0xF50A6C93) {
      delay(100);
      phonePressLong = true;
      Serial.println("phonePressLong = true");
      delay(150);
    }
    if(lastCode != results.value && results.value == 0xF50A4CB3) {
      delay(100);
      phoneUpPressLong = true;
      Serial.println("phoneUpPressLong = true");
      delay(150);
    }
    if(lastCode != results.value && results.value == 0xF50ACC33) {
      delay(100);
      phoneDownPressLong = true;
      Serial.println("phoneDownPressLong = true");
      delay(150);
    }
    // ----------------------------------
    
    // SWITCH DVICE - "BAND" BUTTON
    if(results.value == 0xB54A48B7)
    {
      if(device)
      {
        device = false;  
        Serial.println("Device: Kenwood");
      } else {
        device = true;
        Serial.println("Device: Bluetooth");
      }
    }

    // ----------------------------
    // KENWOOD SOURCE
    if(results.value == 0xB54A58A7)
    {
      delay(100);
      irsend.sendNEC(0x9D62C837, 32);
      Serial.print("KENWOOD SOURCE: ");
      Serial.println(results.value, HEX);
    }

    // KENWOOD VOL UP
    if(results.value == 0xB54A50AF)
    {
      delay(100);
      irsend.sendNEC(0x9D6228D7, 32);
      Serial.print("KENWOOD VOL UP: ");
      Serial.println(results.value, HEX);
    }

    // KENWOOD VOL DOWN
    if(results.value == 0xB54AD02F)
    {
      delay(100);
      irsend.sendNEC(0x9D62A857, 32);
      Serial.print("KENWOOD VOL DOWN: ");
      Serial.println(results.value, HEX);
    }
    // KENWOOD ATT -- Phone Down Long remote
    if(phoneDownPressLong)
    {
      delay(100);
      irsend.sendNEC(0x9D626897, 32);
      Serial.print("KENWOOD ATT -- Phone Down remote: ");
      Serial.println(results.value, HEX);
      phoneDownPressLong = false;
      phonePressShort = false;
    }
  
    // ----------------------------
    // Switch between bluetooth and kenwood

    // BLUETOOTH
    if(device)
    {
      // PLAY -- Phone UP remote
      if(phoneUpPressLong)
      {
        delay(100);
        digitalWrite(BLUETOOTH_PLAY_PIN, HIGH);
        delay(100);
        digitalWrite(BLUETOOTH_PLAY_PIN, LOW);
        delay(100);
        Serial.print("PLAY -- Phone UP remote: ");
        Serial.println(results.value, HEX);
        phoneUpPressLong = false;
        phonePressShort = false;
      }

      // NEXT TRACK
      if(results.value == 0xB54AC23D)
      {
        delay(100);
        digitalWrite(BLUETOOTH_NEXT_PIN, HIGH);
        delay(100);
        digitalWrite(BLUETOOTH_NEXT_PIN, LOW);
        delay(100);
        Serial.print("NEXT TRACK: ");
        Serial.println(results.value, HEX);
      }
  
      // PREV TRACK
      if(results.value == 0xB54A42BD)
      {
        delay(100);
        digitalWrite(BLUETOOTH_PREV_PIN, HIGH);
        delay(100);
        digitalWrite(BLUETOOTH_PREV_PIN, LOW);
        delay(100);
        Serial.print("PREV TRACK: ");
        Serial.println(results.value, HEX);
      }
  
      // VOL UP -- UP on remote
      if(results.value == 0xB54A02FD)
      {
        delay(100);
        digitalWrite(BLUETOOTH_VOLUP_PIN, HIGH);
        delay(200);
        digitalWrite(BLUETOOTH_VOLUP_PIN, LOW);
        delay(100);
        Serial.print("VOL UP -- UP on remote: ");
        Serial.println(results.value, HEX);
      }
  
      // VOL DOWN -- DOWN on remote
      if(results.value == 0xB54A827D)
      {
        delay(100);
        digitalWrite(BLUETOOTH_VOLDOWN_PIN, HIGH);
        delay(200);
        digitalWrite(BLUETOOTH_VOLDOWN_PIN, LOW);
        delay(100);
        Serial.print("VOL DOWN -- DOWN on remote: ");
        Serial.println(results.value, HEX);
      }

    // ==================================
    // KENWOOD
    } else {

      // KENWOOD AUDIO -- Phone Long press remote
      if(phonePressLong)
      {
        delay(100);
        irsend.sendNEC(0x9D62E817, 32);
        Serial.print("KENWOOD AUDIO -- Phone remote: ");
        Serial.println(results.value, HEX);
        delay(1000);
        phonePressLong = false;
        phonePressShort = false;
      }
//-------------------------------------------
//       KENWOOD PLAY/PAUSE -- Phone Short remote / Phone Up short / Phone Down Short
//      if(phonePressShort)
//      {
//        delay(100);
//        irsend.sendNEC(0x9D62708F, 32);
//        Serial.print("KENWOOD PLAY/PAUSE -- Phone Short remote: ");
//        Serial.println(results.value, HEX);
//        phonePressShort = false;
//      }
//-------------------------------------------
      // Phone UP button
      // KENWOOD SWITCH NUMBERS - Phone UP remote
      if(phoneUpPressLong)
      {
        if (keyNumber > 5)
        {
          keyNumber = 0;
        }
        delay(100);
        irsend.sendNEC(keys[keyNumber], 32);
        Serial.print("KENWOOD SWITCH NUMBERS - Phone UP remote: ");
        Serial.println(keys[keyNumber], HEX);
        keyNumber += 1;
        delay(1500);
        phoneUpPressLong = false;
        phonePressShort = false;
      }
      // KENWOOD RIGHT
      if(results.value == 0xB54AC23D)
      {
        delay(100);
        irsend.sendNEC(0x9D62D02F, 32);
        Serial.print("KENWOOD RIGHT: ");
        Serial.println(results.value, HEX);
      }
      // KENWOOD LEFT
      if(results.value == 0xB54A42BD)
      {
        delay(100);
        irsend.sendNEC(0x9D6250AF, 32);
        Serial.print("KENWOOD LEFT: ");
        Serial.println(results.value, HEX);
      }
      // KENWOOD UP
      if(results.value == 0xB54A02FD)
      {
        delay(100);
        irsend.sendNEC(0x9D62B04F, 32);
        Serial.print("KENWOOD UP: ");
        Serial.println(results.value, HEX);
      }
      // KENWOOD DOWN
      if(results.value == 0xB54A827D)
      {
        delay(100);
        irsend.sendNEC(0x9D6230CF, 32);
        Serial.print("KENWOOD DOWN: ");
        Serial.println(results.value, HEX);
      }
    }

    lastCode = results.value;
    
    delay(200);
    irrecv.enableIRIn();
    irrecv.resume(); // Receive the next value
  }
}

// Pioneer CD-SR1110
// B54AD02F --- VOL DOWN
// B54A50AF --- VOL UP
// B54AE916, F50A6C93 --- PHONE
// B54AE916, F50A4CB3 --- PHONE UP
// B54AE916, F50ACC33 --- PHONE DOWN
// B54A02FD --- UP
// B54A827D --- DOWN
// B54A42BD --- LEFT
// B54AC23D --- RIGHT
// B54A48B7 --- BAND
// B54A58A7 --- SOURCE

// Kenwood RC-DV430
// AUDIO SWITCH
// 9D62C837 --- SOURCE
// 9D6228D7, FFFFFFFF --- VOL UP
// 9D62A857, FFFFFFFF --- VOL DOWN
// 9D62807F --- 1
// 9D6240BF --- 2
// 9D62C03F --- 3
// 9D6220DF --- 4
// 9D62A05F --- 5
// 9D62609F --- 6
// 9D62E01F --- 7
// 9D6210EF --- 8
// 9D62906F --- 9
// 9D6200FF --- 0
// 9D62F00F --- DIRECT
// 9DF258A7 --- CLEAR
// 9D6248B7, FFFFFFFF --- MENU
// 9D626897 --- ATT
// 9D62708F --- PLAY/PAUSE
// 9DF2C837 --- STOP
// 9D6250AF, FFFFFFFF --- REW (MENU SWITCH LEFT)
// 9D62D02F, FFFFFFFF --- FF (MENU SWITCH RIGHT)
// 9D62E817, FFFFFFFF --- AUDIO
// 9D62B04F, FFFFFFFF --- FM/+ (MENU UP)
// 9D6230CF, FFFFFFFF --- AM/- (MENU DOWN)
// 
