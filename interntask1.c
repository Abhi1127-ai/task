#include <stdio.h>

// Function prototypes
void convertFromCelsius(float temp);
void convertFromFahrenheit(float temp);
void convertFromKelvin(float temp);

int main() {
    float temperature;
    char unit;

    // Prompt user for input
    printf("Enter temperature value: ");
    scanf("%f", &temperature);

    printf("Enter the unit of temperature (C for Celsius, F for Fahrenheit, K for Kelvin): ");
    scanf(" %c", &unit);

    // Convert and display the temperature
    switch (unit) {
        case 'C':
        case 'c':
            convertFromCelsius(temperature);
            break;
        case 'F':
        case 'f':
            convertFromFahrenheit(temperature);
            break;
        case 'K':
        case 'k':
            convertFromKelvin(temperature);
            break;
        default:
            printf("Invalid unit entered.\n");
    }

    return 0;
}

// Function to convert from Celsius to Fahrenheit and Kelvin
void convertFromCelsius(float temp) {
    float fahrenheit = (temp * 9 / 5) + 32;
    float kelvin = temp + 273.15;
    printf("%.2f Celsius is %.2f Fahrenheit and %.2f Kelvin.\n", temp, fahrenheit, kelvin);
}

// Function to convert from Fahrenheit to Celsius and Kelvin
void convertFromFahrenheit(float temp) {
    float celsius = (temp - 32) * 5 / 9;
    float kelvin = (temp - 32) * 5 / 9 + 273.15;
    printf("%.2f Fahrenheit is %.2f Celsius and %.2f Kelvin.\n", temp, celsius, kelvin);
}

// Function to convert from Kelvin to Celsius and Fahrenheit
void convertFromKelvin(float temp) {
    float celsius = temp - 273.15;
    float fahrenheit = (temp - 273.15) * 9 / 5 + 32;
    printf("%.2f Kelvin is %.2f Celsius and %.2f Fahrenheit.\n", temp, celsius, fahrenheit);
}
