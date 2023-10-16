#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <stdlib.h>

// Define GPIO pin where the PIR motion sensor is connected
#define PIR_PIN 17

void takePicture() {
    char filename[80];
    time_t now;
    struct tm *t;
    char buf[80];

    // Get the current date and time
    time(&now);
    t = localtime(&now);

    // Create a filename with the current date and time
    strftime(buf, sizeof(buf), "%Y%m%d-%H%M%S", t);
    snprintf(filename, sizeof(filename), "/path/to/save/directory/%s.jpg", buf);

    // Use raspistill to take a picture
    char cmd[100];
    snprintf(cmd, sizeof(cmd), "raspistill -o %s -w 640 -h 480", filename);
    system(cmd);
}

void motionDetected() {
    printf("Motion detected! Taking picture.\n");
    takePicture();
}

int main() {
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize GPIO. Running as root?\n");
        return 1;
    }

    pinMode(PIR_PIN, INPUT);

    for (;;) {
        // Check for motion
        if (digitalRead(PIR_PIN) == HIGH) {
            motionDetected();

            // Delay to avoid multiple triggers
            delay(5000);
        }

        // Poll every 1 second
        delay(1000);
    }

    return 0;
}
