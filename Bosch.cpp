#include <iostream>

uint8_t value[8] = { 0, 0, 0, 0, 0, 0, 0, 0};  //the storage value
uint8_t dotPosition = 0;

uint8_t input[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //the input value

void binaryRepresentation(uint16_t num)
{
    uint16_t i;
    for (i = 1 << 15; i > 0; i = i / 2)
    {
        if ((num & i) != 0)
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << "\n";
}

uint16_t convertTheDigit(uint8_t value) {
    if (value == 0)      {return 0b00111111; /*zero; */ }
    else if (value == 1) {return 0b00000110; /*one*/ }
    else if (value == 2) {return 0b01011011; /*two*/ }
    else if (value == 3) {return 0b01001111; /*three*/ }
    else if (value == 4) {return 0b01100110; /*four*/ }
    else if (value == 5) {return 0b01101101; /*five*/ }
    else if (value == 6) {return 0b01111101; /*six*/ }
    else if (value == 7) {return 0b00000111; /*seven*/ }
    else if (value == 8) {return 0b01111111; /*eight*/ }
    else if (value == 9) {return 0b01101111; /*nine*/ }
    else { return 0; }
}   

uint16_t draw_digit(uint8_t position, uint8_t digit_value) {

    uint16_t positionSegment = position;
    
    uint16_t litTheDiodes = 0;

    uint8_t positionOftheDot = dotPosition;

    if (positionOftheDot == position && position != 0) {
        litTheDiodes = value[dotPosition - 1];
        litTheDiodes |= ((uint16_t)1U << 7);//this will lit the dot on the segment
        positionSegment -= 1;
        value[dotPosition-1] = litTheDiodes;
    } 
    else {
        litTheDiodes = convertTheDigit(digit_value);
        value[position] = digit_value;
    }

    //first half will contain the position where to be displayed and the second half - the digital output to lit the diodes
    uint16_t connectionToTheDisplay = ((positionSegment << 8) | (litTheDiodes & 0xFF));
    
    return connectionToTheDisplay; //sending on the display
}

void draw_value(uint8_t* value) {

    for (int16_t i = 0; i < 8; i++) {   //this is for us to see the result atm
        printf("%d", value[i]);
    }

    std::cout << '\n';

    for (int16_t i = 7; i >= 0; i--) {  //printing the bits
         binaryRepresentation((draw_digit(i, value[i])));
    }
}

void clearValue() {
    //clearing the storage value
    for(uint8_t i = 0; i < 8; i++){
        value[i] = 0;
    }
}

void addValue(uint8_t* inputNumbers) {
    uint8_t oneInMind = 0;
    uint8_t tempVar = 0;
    for (int16_t i = 7; i >= 0 ; i--)
    {
        tempVar = value[i] - '0' + inputNumbers[i] + oneInMind;
        oneInMind = 0;
        if (tempVar >= 10) {
            oneInMind++;
            value[i] %= 10;
            if (i == 0) {   //in case we overflow
                clearValue();
                break;
            }
        }
        value[i] = tempVar;
    }
}

void subtractValue(uint8_t* inputNumbers) {
    uint8_t minusOne = 0;
    int8_t tempVar = 0;
    int16_t k = 0;
    for (int16_t i = 7;(i >= 0) && (k <8); i--, k++)
    {
        tempVar = value[i] - inputNumbers[i] + '0' - minusOne;
        minusOne = 0;
        if (tempVar < 0) {
            minusOne++;
            tempVar = tempVar + 10;

            if (i == 0) //overflow
            {
                clearValue();
                break;
            }
        }
        value[i] = tempVar;
        tempVar = 0;
    }
}

int main()
{
    while (true) { //program always running 
        for (int16_t i = 0; i < 8; i++)     //simulating an input
        {
            uint8_t tempVariable = 0;
            std::cin >> tempVariable;
            if (!(isdigit(tempVariable))) {
                if ((tempVariable == '.' || tempVariable == ',') && i != 0) {
                    dotPosition = i;
                }
                i--;
                continue;
            }
            input[i] = tempVariable;
        }
        std::cin.clear();
        char symbol = '0';
        std::cin >> symbol;

        if (symbol == '+') {
            addValue(input);
        }
        else if (symbol == '-') {
            subtractValue(input);
        }
        draw_value(value); //printing out the output for the display
                            //first 8 bits are locating which segment to light up
                            //the second 8 bits are gonna light up the diodes of the segment
    }
}