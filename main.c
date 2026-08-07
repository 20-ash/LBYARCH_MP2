#include <stdio.h>     // Input and output functions (printf, scanf)
#include <stdlib.h>    // Memory allocation functions (malloc, free)
#include <time.h>      // Timing functions (clock)
#include <math.h>      // Floating point comparison (fabs)


// Declaration of the external assembly function
extern void imgCvtGrayInttoFloat(
    unsigned char* input,   // Pointer to input image pixels(uint8 grayscale values)
    float* output,          // Pointer to output image pixels (float values)
    int width,              // Image width (the number of pixels per row)
    int height              // Image height (the number of rows)
);

// Function to check if the assembly output is correct
int checkCorrectness(unsigned char* input, float* output, int size)
{
    // Check every converted pixel
    for (int i = 0; i < size; i++)
    {
        // Expected conversion: integer pixel / 255
        float expected = input[i] / 255.0f;


        // Compare expected and actual output (allowing small floating point error)
        if (fabs(output[i] - expected) > 0.001f)
        {
            printf("Incorrect output at pixel %d\n", i);
            printf("Expected: %.2f | Got: %.2f\n",expected, output[i]);
            return 0;   // Output is incorrect
        }
    }
    return 1;   // Output is correct
}

// C implementation of grayscale integer to float conversion
void imgCvtGrayInttoFloat_C(
    unsigned char* input,
    float* output,
    int width,
    int height
)
{
    int size = width * height;       // Calculate total number of pixels
    for (int i = 0; i < size; i++)  // Convert every pixel from integer to normalized float
    {
        output[i] = input[i] / 255.0f;
    }
}

// Function to perform required performance testing
// 0 for C Testing, 1 for ASM Testing
void runBenchmark(int useASM)
{
    // Indicated image sizes
    int imageSizes[3][2] = {
        {10, 10},
        {100, 100},
        {1000, 1000}
    };

    int runs = 30; // Number of executions required
    srand((unsigned int)time(NULL));  // Initialize random number generator


    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    // Display which implementation is being tested
    if (useASM)
        printf("ASM Performance Testing\n");
    else
        printf("C Performance Testing\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

    // Test each image size
    for (int test = 0; test < 3; test++)
    {
        // Get width and height
        int width = imageSizes[test][0];
        int height = imageSizes[test][1];

        // Calculate total pixels
        int totalPixels = width * height;
        printf("\nImage Size: %dx%d\n", width, height);

        // Allocate memory
        unsigned char* input = (unsigned char*)malloc(totalPixels * sizeof(unsigned char));
        float* output = (float*)malloc(totalPixels * sizeof(float));

        // Check memory allocation
        if (input == NULL || output == NULL)
        {
            printf("Memory allocation failed.\n");
            return;
        }

        // Generate random pixel values from 0-255
        for (int i = 0; i < totalPixels; i++)
            input[i] = rand() % 256;
       
        double totalTime = 0;  // Store total execution time

        // Execute assembly function 30 times
        for (int i = 0; i < runs; i++)
        {
            clock_t start = clock();  // Start timer
            // Run Assembly implementation
            if (useASM)
            {
                imgCvtGrayInttoFloat(
                    input,
                    output,
                    width,
                    height
                );
            }

            // Run C implementation
            else
            {
                imgCvtGrayInttoFloat_C(
                    input,
                    output,
                    width,
                    height
                );
            }
            clock_t end = clock();  // Stop timer

            // Calculate elapsed time
            double elapsed =(double)(end - start) / CLOCKS_PER_SEC;
            totalTime += elapsed;   // Add elapsed time
        }

        // Compute and print average execution time
        double avgTime = totalTime / runs;  
        printf("Average Execution Time (%d runs): %.6f seconds\n", runs, avgTime);

        // Check correctness
        if (checkCorrectness(input, output, totalPixels))
            printf("Correctness Status: PASSED\n");
        else
            printf("Correctness Status: FAILED\n");
        
        // Free allocated memory
        free(input);
        free(output);
    }
}

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
        if (scanf_s("%d%*[,]", &temp) != 1) // Check if the input is a valid integer (disregarding , after)
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

    runBenchmark(0);  // Run C implementation benchmark
    runBenchmark(1);  // Run ASM implementation benchmark
    return 0;
}
