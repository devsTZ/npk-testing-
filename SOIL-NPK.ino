
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// Create object named SIM900 of the class SoftwareSerial 
SoftwareSerial SIM900(8, 7);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//
//
void setup() {
  SIM900.begin(9600);  // Define baud rate for software serial communication 
  Serial.begin(9600); // Define baud rate for serial communication 
Serial.println("setup complete!");
  lcd.begin(20,4 ); //initialize the interface of the lcd screen
  lcd.print("Initializing...");
  lcd.setCursor(0, 1);
  lcd.print("NPK CHECKING");
  delay(6000);
    lcd.clear();

}

//end of setup

//start void loop

void loop()
{
  int sensorPin = analogRead(A0); //this pin will be used to support the power connection
//  inquiry frame for getting soil NPK nutrient values
  float nitrogen = map(sensorPin, 0, 1023, 1, 250);// NITROGEN Sensor 
  float phosphorus = map(sensorPin, 0, 1023, 1, 30);// PHOSPHORUS Sensor 
  float potassium = map(sensorPin, 0, 1023, 1, 200);// POTASSIUM Sensor 
  delay(100);
  
  
  lcd.setCursor(0, 0);
  lcd.print("----NPK SENSING----");
  delay(100);  

  //clear lcd data so as to print NPK nutrient that will depend on variable resistors
  lcd.clear();

  lcd.setCursor(0,0 );
  lcd.print("----NPK NUTRIENTS----");
  
  lcd.setCursor(0,1 );
  lcd.print("N: ");
  lcd.print(nitrogen);
  lcd.println("ppm");

 lcd.println(); 
 lcd.setCursor(0,2);
 lcd.print("P: ");
 lcd.print(phosphorus);
 lcd.println("ppm");

 lcd.println();  
 lcd.setCursor(0,3);
 lcd.print("K: ");
 lcd.print(potassium);
 lcd.println("ppm ");
  delay(100);
 
  Serial.print("NITROGEN = ");
  Serial.print(nitrogen);
  Serial.println(" ppm");
  
  Serial.print("PHOSPHORUS = ");
  Serial.print(phosphorus);
  Serial.println(" ppm");
  
  Serial.print("POTASSIUM = ");
  Serial.print(potassium);
  Serial.println(" ppm");
  Serial.println("----------------");
  
  delay(100);

if(SIM900.available())
Serial.write(SIM900.read());

  Serial.println("...HTTP POST METHOD...");
  SIM900.println("AT"); // Check Communication  
  delay(100);
  ShowSerialData(); // Print response on the serial monitor 
  delay(100);

  SIM900.println("AT+CPIN?");//Pin code of the sim is accepted or not
  delay(100);

  SIM900.println("AT+CREG?"); //Gives information about access technology
  delay(100);

  SIM900.println("AT+CGATT?");//Attach or deattach pacakge domain service
  delay(100);

  SIM900.println("AT+CIPSHUT");//GPRS tcp contact
  delay(100);

   SIM900.println("AT+CIPSTATUS");//Return the connection status
  delay(100);
  
  SIM900.println("AT+CIPMUX=0");//Device single or multi ip address
  delay(100);
  ShowSerialData();
  
   SIM900.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN,
  delay(100);
  ShowSerialData();

  SIM900.println("AT+CIICR");//bring up wireless connection
  delay(100);
  ShowSerialData();

  SIM900.println("AT+CIFSR");//get local IP adress
  delay(100);
  ShowSerialData();

  SIM900.println("AT+CIPSPRT=0"); //echo if is prompt or not
  delay(100);
  ShowSerialData();

  SIM900.println("AT+CIPSTART=\"TCP\",\"localhost/soil-npk-test/nutrient.php\",\"3306\"");//start up the connection to database
  delay(100);
  ShowSerialData();

  SIM900.println("AT+CIPSEND");  //begin send data to remote server
  delay(100);
  ShowSerialData();

   //Sending data to server
  String data = "GET soil-npk-test/nutrient.php";
  Serial.println(data);
  SIM900.println(data);   //begin send data to remote server
  delay(100);
  ShowSerialData();

  SIM900.println((char)26);  //sending collected data to database
  delay(200);  //waitting for reply, important! the time is base on the condition of internet
  SIM900.println();
  ShowSerialData();

  SIM900.println("AT+CIPSHUT"); //close the connection
  delay(100);
  ShowSerialData();
  Serial.println("Data Submitted Successful");
  delay(100);
  
  Serial.println("");
  Serial.println("next soil nutrients test");
  delay(1000);
  
  Serial.println("");
  Serial.println("..........................");
  Serial.println("");
  Serial.println("..........................");
  
}

  void ShowSerialData()
{
  while (SIM900.available() != 0)
    Serial.write(SIM900.read());
  delay(1000);

}
