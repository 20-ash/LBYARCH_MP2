#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern void imgCvtGrayInttoFloat(
    unsigned char* input,
    float* output,
    int width,
    int height
);

int main(void)
{
    int width, height;

    printf("Enter height and width: ");
    scanf("%d %d", &height, &width);

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

    for (int i = 0; i < width * height; i++)
    {
        int temp;
        scanf("%d", &temp);

        // Clamp values to the valid uint8 range
        if (temp < 0)
            temp = 0;
        if (temp > 255)
            temp = 255;

        input[i] = (unsigned char)temp;
    }

    // Call the assembly function
    imgCvtGrayInttoFloat(input, output, width, height);

    // Print the converted image
    printf("\nFloat Image:\n");

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            printf("%.6f ", output[r * width + c]);
        }
        printf("\n");
    }

    // Correctness check
    printf("\nCorrectness Check:\n");

    int correct = 1;

    for (int i = 0; i < width * height; i++)
    {
        float expected = input[i] / 255.0f;

        if (fabsf(output[i] - expected) > 0.0001f)
        {
            printf("Mismatch at pixel %d\n", i);
            printf("Expected: %.6f\n", expected);
            printf("Actual:   %.6f\n", output[i]);

            correct = 0;
            break;
        }
    }

    if (correct)
        printf("Conversion correct!\n");
    else
        printf("Conversion failed!\n");

    free(input);
    free(output);

    return 0;
}