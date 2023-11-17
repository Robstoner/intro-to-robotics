
// Input variables
char incomingByte = '0';

// Menu settings
// 0 - Main, 1,2,3,4 - The 4 submenus
char currentMenu = '0';
bool menuShown = false;

void setup() {

  Serial.begin(9600);
}

void loop() {
  if (!menuShown) {
    printMenus(currentMenu);
    menuShown = true;
  }

  if (Serial.available()) {
    incomingByte = (char)Serial.read();

    if (menuShown) {
      chooseOption(incomingByte);
    }
  }
}

// Used a switch case to choose which menu to print, instead of 5 different functions
// 0 - main, 1,2,3,4 - each submenu respectively
void printMenus(char menu) {

  switch (menu) {
    case '0':
      Serial.println(F("1. Sensor Settings."));
      Serial.println(F("2. Reset Logger Data."));
      Serial.println(F("3. System Status."));
      Serial.println(F("4. RGB LED Control."));
      currentMenu = '0';

      break;
    case '1':
      Serial.println(F("1. Sensor Settings."));
      Serial.println(F("  1. Sensors Sampling Interval."));
      Serial.println(F("  2. Ultrasonic Alert Threshold."));
      Serial.println(F("  3. LDR Alert Threshold."));
      Serial.println(F("  4. Back."));
      currentMenu = '1';

      break;
    case '2':
      Serial.println(F("2. Reset Logger Data."));
      Serial.println(F("  1. Yes."));
      Serial.println(F("  2. No."));
      currentMenu = '2';

      break;
    case '3':
      Serial.println(F("3. System Status."));
      Serial.println(F("  1. Current Sensor Readings."));
      Serial.println(F("  2. Current Sensor Settings."));
      Serial.println(F("  3. Display Logged Data."));
      Serial.println(F("  4. Back."));
      currentMenu = '3';

      break;
    case '4':
      Serial.println(F("4. RGB LED Control."));
      Serial.println(F("  1. Manual Color Control."));
      Serial.println(F("  2. LED: Toggle Automatic ON/OFF."));
      Serial.println(F("  3. Back."));
      currentMenu = '4';

      break;
    default:
      printMenus('0');

      break;
  }
}

void chooseOption(char option) {
  if (option == '\n') {
    return;
  }

  switch (option) {
    case '1':
      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('1');
        break;
      }

      break;
    case '2':
      // Back from submenus
      if (currentMenu == '2') {
        printMenus('0');
        break;
      }

      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('2');
        break;
      }


      break;
    case '3':
      // Check if current submenu has this option
      if (currentMenu == '2') {
        defaultCaseInvalidOption();
        break;
      }

      // Back from submenus
      if (currentMenu == '4') {
        printMenus('0');
        break;
      }

      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('3');
        break;
      }

      break;
    case '4':
      // Check if current submenu has this option
      if (currentMenu == '2' || currentMenu == '4') {
        defaultCaseInvalidOption();
        break;
      }

      // Back from submenus
      if (currentMenu == '1' || currentMenu == '3') {
        printMenus('0');
        break;
      }

      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('4');
        break;
      }

      break;
    default:
      defaultCaseInvalidOption();

      break;
  }
}

void defaultCaseInvalidOption() {
  Serial.println(F("Invalid option, please choose one of the options listed below."));
  menuShown = false;
}