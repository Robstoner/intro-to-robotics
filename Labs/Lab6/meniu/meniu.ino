char incomingByte = "";
bool menuShown = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // TODO: Check if Serial input is available
  // TODO: Read the input and store it in a variable
  // TODO: Call a function to print the message based on the user's choice
  // TODO: Display the menu again
  if (!menuShown) {
    printMenu();
    menuShown = true;
  }

  if (Serial.available()) {
    incomingByte = (char)Serial.read();

    if (menuShown) {
      printMessage(incomingByte);
    }
  }
}

void printMenu() {
  // TODO: Print the main menu options to the Serial Monitor
  // The menu should list at least three options for the user to choose from
  Serial.println("1. Print \"Hello, world\".");
  Serial.println("2. Print an \"Option 2 selected\" message.");
  Serial.println("3. Print a joke.");
}

void printMessage(char option) {
  if (option == '\n') {
    return;
  }
  Serial.print("\nPrinted message: ");
  // TODO: Use a switch-case structure to handle different options and print corresponding messages
  menuShown = false;

  switch (option) {
    case '1':
      {
        Serial.println("Hello, world");
        break;
      }
    case '2':
      {
        Serial.println("Option 2 selected");
        break;
      }
    case '3':
      {
        Serial.println("There is no joke");
        
        break;
      }
    default:
      {
        menuShown = true;
        break;
      }
  }
}