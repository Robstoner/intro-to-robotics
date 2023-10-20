### Nu duceti rezistorii la limita de putere, pentru ca pot sa arda

#### 1 / R_paralel = 1 / R_1 + 1/R_2 + ... + 1/R_n

## Curs 2

Daca lipseste ceva din kit ul arduino putem veni sa intrebam in alta zi la laborator, in special LED ul RGB, pentru ca ne trebuie la tema.

Alimentatorul o sa lipseasca la multe, nu e problema, alimentam cu USB.

### Memorie pe ATMEGA

Flash - unde stocam codul
SRAM - unde stocam variabilele
EEPROM - unde stocam variabile persistente (high score, etc)


### NU FOLOSIM WHILE PE ARDUINO

### Functii pe care trebuie sa le stim pe de rost

analogRead() - read the value from an analog pin    <br>
analogWrite() - output a PWM signal   <br>
digitalRead() - read the value from a digital pin   <br>
digitalWrite() - set a digital pin to HIGH or LOW   <br>
pinMode() - configure the pin as either input or output

In rest e doar programare C.

### Color coding

Black - GND<br>
Red (sau culoare deschisa dac anu avem rosu) - Power (5V, 3.3V)

### Semnale

Avem doua interpretari de semnale:
- Input
- Output

Si poate fi de doua tipuri:
- Analog (temperatura, sunet, etc)
- Digital (On/Off)

#### Folosim PWM pentru a simula un output analog

Este un pic innaccurate, nu e bun pentru lucruri de finete.
Trecerea de la high la low nu e instanta si de fapt poate fi un pic mai mult voltajul mediu decat ne asteptam.

### TTL Gate (input, output high/low margins)

Input: 
- 0 - 0.8V - LOW
- 2.0 - 5.0V - HIGH

Output:
- 0 - 0.5V - LOW
- 2.7 - 5.0V - HIGH

#### Trebuie sa evitam sa stam in zonele dintre (ZONA INTERZISA)

#### Doar pinii digitali cu ~ pot fi folositi pentru PWM pentru a simula un output analog

### Coding style

Foarte important, o sa fim depunctati daca nu scriem cod frumos.

[Github](https://github.com/irikos/arduino-style-guide)