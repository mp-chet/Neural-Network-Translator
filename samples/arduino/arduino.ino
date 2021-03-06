extern "C"
{
#include "nn_model.h"
}

void setup() {

  /* Opens the serial port and sets the data rate to 9600 bps. */
  Serial.begin(9600);
  Serial.println("Enter input values:");
}

void loop()
{
  /* Get the number of bytes (characters) available for reading from the serial port.
  This is data that’s already arrived and stored in the serial receive buffer (which holds 64 bytes). */
  if (Serial.available() > 0)
  {
    String inputString;

    /* Reads characters from the serial buffer into a String. */
    inputString = Serial.readString();

    /* Print the received input string to allow verification. */
    Serial.println("Input is: " + inputString);

    /* Inits the input float array with zeros.
    The length of the array is determined through the number of units in layer 0 (input layer). */
    float * input = calloc(LAYER_OUTPUT_WIDTH[0]*LAYER_OUTPUT_HEIGHT[0], sizeof(float));
    char *tmp;
    int i = 0;

    /* Tokenize the string on "," characters. */
    tmp = strtok(&inputString[0], ",");
    while (tmp) {

      /* Convert a string token to a float and store it in the input array. */
      input[i++] = atof(tmp);
      tmp = strtok(NULL, ",");
    }

    /* Call the predict function with the input array. */
    uint32_t startTime = micros();
    float * result = predict(input);
    uint32_t endTime = micros();

    for(int i = 0; i < LAYER_OUTPUT_HEIGHT[NUMBER_OF_LAYERS-1]; i++)
    {
      for(int j = 0; j < LAYER_OUTPUT_WIDTH[NUMBER_OF_LAYERS-1]; j++)
      {
        Serial.print(String(*(result+i*LAYER_OUTPUT_WIDTH[NUMBER_OF_LAYERS-1]+j),8)+ "   ");
      }
      Serial.println();
    }

    Serial.println(String(endTime-startTime)+" microseconds");
    Serial.println("\nEnter input values:");
  }
}
