#include <fcntl.h> // for open()
#include <unistd.h> // for read(), write(), and close()
#include <stdio.h>
#include <assert.h>

int main() {
  // Get the file descriptor of existing input file
  const char inputName[] = "input";
  int inputFile = open(inputName, O_RDONLY);
  printf("The input file's file descriptor is %d\n", inputFile);
  assert(inputFile > 0);

  // Get the file descriptor of an output file that may not exist
  const char outputName[] = "output";
  int outputFile = open(outputName, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  printf("The output file's file descriptor is %d\n\n", outputFile);
  assert(outputFile > 0);
  char buffer[128];

  // Begin reading bytes into buffer, and writing bytes into output file
  int bytesRead, bytesWritten;
  while ( (bytesRead = read(inputFile, buffer, sizeof(buffer))) > 0 ) {
    printf("Read %d bytes into buffer\n", bytesRead);
    bytesWritten = write(outputFile, buffer, bytesRead);
    if (bytesWritten != bytesRead) {
      fprintf(stderr, "There was an error writing to the output file. Wrote %d bytes instead of %d\n", bytesWritten, bytesRead);
      return 1;
    }
    printf("Wrote %d bytes into output file from buffer\n", bytesWritten);
  }

  if (bytesRead < 0) {
    fprintf(stderr, "There was an error reading from the input file!\n");
  }

  // Close file descriptors
  close(inputFile);
  close(outputFile);
  return 0;
}
