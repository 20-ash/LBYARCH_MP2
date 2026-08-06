#include <stdio.h>     // Input and output functions (printf, scanf)
#include <stdlib.h>    // Memory allocation functions (malloc, free)

// Declaration of the external assembly function
extern void imgCvtGrayInttoFloat(
    unsigned char* input,   // Pointer to input image pixels(uint8 grayscale values)
    float* output,          // Pointer to output image pixels (float values)
    int width,              // Image width (the number of pixels per row)
    int height              // Image height (the number of rows)
);

// Main function
int main(void)
{
    int width, height;

    // Ask the user to enter the image dimensions; then read
    printf("Enter height and width: ");
    scanf_s("%d %d", &height, &width);

    // Allocate memory
    unsigned char* input = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    float* output = (float*)malloc(width * height * sizeof(float));

    if (input == NULL || output == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Read pixel values
    printf("Enter pixel values:\n");

    // Read pixel values and store them as unsigned 8-bit integers
    for (int i = 0; i < width * height; i++)
    {
        int temp;
        if (scanf_s("%d", &temp) != 1) // Check if the input is a valid integer
        {
            printf("INVALID: Please enter valid pixel values! \n");
            free(input);
            free(output);
            return 1;
        }

        // Clamp values to the valid uint8 range (0 - 255)
        if (temp < 0)
            temp = 0;
        if (temp > 255)
            temp = 255;

        // Convert the integer value to unsigned char and store it in the input array
        input[i] = (unsigned char)temp;
    }

    // Call the assembly function
    imgCvtGrayInttoFloat(input, output, width, height);

    // Print the converted image
    printf("\nFloat Image:\n");

    // Print the converted float image row by row
    for (int r = 0; r < height; r++)
    {
        // Print each pixel value in the current row
        for (int c = 0; c < width; c++)
        {
            printf("%.2f ", output[r * width + c]);
        }
        printf("\n");  // Move to the next row 
    }

    //release allocated memory
    free(input);
    free(output);

    return 0;
}
