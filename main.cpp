#include <SoftwareSerial.h>
#include <Arduino.h>
#include <GIS_V2_Flex_18G_5_Pumps_parameters.h>
SoftwareSerial Pump(Recieve_From_Pump, Transmit_To_Pump);  //Defining pins for pump serial input and ouput
//int Volume_counter = 0;

void Clean_Pump_Serial()   // function which reads buffer and by that empties it
{
  for (int i = 0; i < 65; i++){
    Pump.read();
  }
}

void All_Pumps_Unusable()
{
  digitalWrite(Green_LED_A, LOW);
  digitalWrite(Red_LED_A, HIGH);
  digitalWrite(Green_LED_B, LOW);
  digitalWrite(Red_LED_B, HIGH);
}

void gen_function_0(const byte command[], int size_t)// function verifies each pump command is only given once previous command is completed
{
  All_Pumps_Unusable();
  int Task_Executed_0 = 1;
  do {
    Clean_Pump_Serial();
    Pump.write(command, size_t);
    delay(500);
    byte response[8];
    Pump.readBytes(response, 8);
    Task_Executed_0 = (memcmp(response, Pump_0_Executed, size_t) == 0 || memcmp(response, Pump_0_Idle, size_t) == 0) ? 0 : 1;
  } while (Task_Executed_0);
}

void gen_function_1(const byte command[], int size_t)// function verifies each pump command is only given once previous command is completed
{
  All_Pumps_Unusable();
  int Task_Executed_1 = 1;
  do {
    Clean_Pump_Serial();
    Pump.write(command, size_t);
    delay(500);
    byte response[8];
    Pump.readBytes(response, 8);
    Task_Executed_1 = (memcmp(response, Pump_1_Executed, size_t) == 0 || memcmp(response, Pump_1_Idle, size_t) == 0) ? 0 : 1;
  } while (Task_Executed_1);
}

/*void Pump_0_Working()
{
  digitalWrite(Green_LED_A, LOW);
  digitalWrite(Red_LED_A, HIGH);  
}
void Pump_0_Done()
{
  digitalWrite(Red_LED_A, LOW);
  digitalWrite(Green_LED_A, HIGH);
}
void Pump_1_Working()
{
  digitalWrite(Green_LED_B, LOW);
  digitalWrite(Red_LED_B, HIGH);  
}
void Pump_1_Done()
{
  digitalWrite(Red_LED_B, LOW);
  digitalWrite(Green_LED_B, HIGH);
}*/



void Reset_All_Pumps()
{
  gen_function_0(Pump_0_Speed_2_mms, 8);
  gen_function_0(Pump_0_Valve_to_1, 8);
  gen_function_0(Pump_0_Reset, 8);
  gen_function_1(Pump_1_Speed_2_mms, 8);
  gen_function_1(Pump_1_Valve_to_1, 8);
  gen_function_1(Pump_1_Reset, 8);

}

void Fill_Applicator_A(byte intake[], byte dispense_valve[], byte dispense[])
{
  //Pump_0_Working();
  gen_function_0(Pump_0_Valve_to_1, 8);
  gen_function_0(Pump_0_Speed_2_mms, 8);
  gen_function_0(intake, 8);
  gen_function_0(dispense_valve, 8);
  gen_function_0(Pump_0_Speed_013_mms, 8);
  gen_function_0(dispense, 8); //here i want to get out of the function when the dispense start in order to continue with other pumps
}
void Fill_Applicator_B(byte intake[], byte dispense_valve[], byte dispense[])
{
  //Pump_1_Working();
  gen_function_1(Pump_1_Valve_to_1, 8);
  gen_function_1(Pump_1_Speed_2_mms, 8);
  gen_function_1(intake, 8);
  gen_function_1(dispense_valve, 8); //depending on top or bottom valve
  gen_function_1(Pump_1_Speed_013_mms, 8); //setting speed to 0.13 mms
  gen_function_1(dispense, 8); //here i want to get out of the function when the dispense start in order to continue with other pumps
}
/*void Fill_Applicator_C(byte intake[], byte dispense_valve[], byte dispense[])
{
  Pump_2_Working();
  gen_function(Pump_2_Valve_to_1, 8, 2);
  gen_function(intake, 8, 2);
  gen_function(dispense_valve, 8, 2); //depending on top or bottom valve
  Pump.write(dispense, 8); //here i want to get out of the function when the dispense start in order to continue with other pumps
}
void Fill_Applicator_D(byte intake[], byte dispense_valve[], byte dispense[])
{
  Pump_3_Working();
  gen_function(Pump_3_Valve_to_1, 8, 3);
  gen_function(intake, 8, 3);
  gen_function(dispense_valve, 8, 3); //depending on top or bottom valve
  Pump.write(dispense, 8); //here i want to get out of the function when the dispense start in order to continue with other pumps
}
void Fill_Applicator_E(byte intake[], byte dispense_valve[], byte dispense[])
{
 // Pump_4_Working();
  gen_function(Pump_4_Valve_to_1, 8, 4);
  gen_function(intake, 8, 4);
  gen_function(dispense_valve, 8, 4); //depending on top or bottom valve
  Pump.write(dispense, 8); //here i want to get out of the function when the dispense start in order to continue with other pumps
}*/
void Query_Pump_0_Status(byte Query_Pump[], byte Pump_Response[])
{
  Clean_Pump_Serial();
  Pump.write(Pump_0_Query, 8);
  byte response[8];
  Pump.readBytes(response, 8);
  int busy_A = memcmp(response, Pump_0_Busy,8) == 0 ? 1 : 0;
  if (busy_A)
  {
    digitalWrite(Green_LED_A, LOW);
    digitalWrite(Red_LED_A, HIGH);
  }
  else
  {
    digitalWrite(Green_LED_A, HIGH);
    digitalWrite(Red_LED_A, LOW);
  }
}
void Query_Pump_1_Status(byte Query_Pump[], byte Pump_Response[])
{
  Clean_Pump_Serial();
  Pump.write(Pump_1_Query, 8);
  byte response[8];
  Pump.readBytes(response, 8);
  int busy_B = memcmp(response, Pump_1_Busy,8) == 0 ? 1 : 0;
  if (busy_B)
  {
    digitalWrite(Green_LED_B, LOW);
    digitalWrite(Red_LED_B, HIGH);
  }
  else
  {
    digitalWrite(Green_LED_B, HIGH);
    digitalWrite(Red_LED_B, LOW);
  }
}


void setup() {
Serial.begin(9600);
  Pump.begin(9600);
  pinMode(Go_Button_A, INPUT_PULLUP);              // 2
  pinMode(Go_Button_B, INPUT_PULLUP);              // 3
  pinMode(Go_Button_C, INPUT_PULLUP);              // 4
  pinMode(Go_Button_D, INPUT_PULLUP);              // 5
  pinMode(Go_Button_E, INPUT_PULLUP);              // 6
  pinMode(Red_LED_A, OUTPUT);                      // 7
  pinMode(Red_LED_B, OUTPUT);                      // 8
  //pinMode(Red_LED_C, OUTPUT);              // 9
  pinMode(Red_LED_D, OUTPUT);              // 20
  pinMode(Red_LED_E, OUTPUT);              // 21
  pinMode(Green_LED_A, OUTPUT);      // 12
  //pinMode(Green_LED_B, OUTPUT);      // 13
  //pinMode(Green_LED_C, OUTPUT);      // 14
  //pinMode(Green_LED_D, OUTPUT);      // 15
  pinMode(Recieve_From_Pump, INPUT);     // 16
  pinMode(Transmit_To_Pump, OUTPUT);     // 17
  pinMode(Applicator_Switch_1, INPUT_PULLUP); // 18
  pinMode(Applicator_Switch_2, INPUT_PULLUP); // 19
  pinMode(Program_Switch_1, INPUT_PULLUP); // 10
  pinMode(Program_Switch_2, INPUT_PULLUP); // 11
  //Pump.write(Pump_0_Reset, 8);
  Reset_All_Pumps();
  //Pump_0_Done();
  //Pump_1_Done();

}

void loop() {
int Run_Pump_0 = digitalRead(Go_Button_A);
int Run_Pump_1 = digitalRead(Go_Button_B);

if (Run_Pump_0 == LOW)
{
Fill_Applicator_A(Pump_0_Intake_40_ul, Pump_0_Valve_to_2, Pump_0_Dispense_40_ul);
}

if (Run_Pump_1 == LOW)
{
Fill_Applicator_B(Pump_1_Intake_40_ul, Pump_1_Valve_to_2, Pump_1_Dispense_40_ul);
}

Query_Pump_0_Status(Pump_0_Query, Pump_0_Busy);
Query_Pump_1_Status(Pump_1_Query, Pump_1_Busy);
}


