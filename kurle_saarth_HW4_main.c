#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COLUMNS 50
#define MAX_BUFFER_SIZE 4096 // Increase if you have very long lines

typedef struct {
    int thread_id;
    // ... other thread-specific fields ...
} thread_data_t;

typedef struct {
    int count; // Just an example field, you'll need to expand this
    pthread_mutex_t mutex;
} shared_data_t;

FILE *csvFile = NULL;
pthread_mutex_t csvMutex;
shared_data_t sharedData;

// This will parse a CSV line into fields considering quotes and commas.
char **parseCSVLine(char *line) {
    char **record = (char **)malloc(MAX_COLUMNS * sizeof(char *));
    char *p = line;
    char *field_start = p;
    int field_count = 0;

    bool in_quotes = false;
    while (*p) {
        if (*p == '"') {
            in_quotes = !in_quotes;
            p++;
        } else if (*p == ',' && !in_quotes) {
            *p = '\0';
            record[field_count++] = strdup(field_start);
            field_start = p + 1;
        } else if (*p == '\\' && *(p + 1) == '"') {
            // Handle escaped quotes
            strcpy(p, p + 1); // Shift left to override the escape char
        }
        p++;
    }

    // Capture the last field
    if (field_start != p) {
        record[field_count++] = strdup(field_start);
    }

    record[field_count] = NULL; // Null terminate the record
    return record;
}

// Free parsed CSV line.
void freeCSVLine(char **line) {
    int i = 0;
    while (line[i] != NULL) {
        free(line[i]);
        i++;
    }
    free(line);
}

// Get the next CSV line and parse it.
char **csvnext(void) {
    pthread_mutex_lock(&csvMutex);
    
    char buffer[MAX_BUFFER_SIZE];
    char **row = NULL;

    if (fgets(buffer, sizeof(buffer), csvFile) != NULL) {
        buffer[strcspn(buffer, "\n\r")] = 0; // Strip newline character
        row = parseCSVLine(buffer);
        pthread_mutex_lock(&sharedData.mutex);
        sharedData.count++;
        pthread_mutex_unlock(&sharedData.mutex);
    }

    pthread_mutex_unlock(&csvMutex);
    return row;
}

void *process_csv_data(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    char **row;
    while ((row = csvnext()) != NULL) {
        // TODO: Process the row
        // For example, analyze the data and update the sharedData structure

        freeCSVLine(row); // Free each row after processing
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        fprintf(stderr, "Usage: %s <csv filename> <threads> <subfield> <subfield value 1> <subfield value 2> <subfield value 3>\n", argv[0]);
        return 1;
    }

    csvFile = fopen(argv[1], "r");
    if (!csvFile) {
        perror("Error opening CSV file");
        return 1;
    }

    pthread_mutex_init(&csvMutex, NULL);
    pthread_mutex_init(&sharedData.mutex, NULL);
    sharedData.count = 0;

    int num_threads = atoi(argv[2]);
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    struct timespec startTime, endTime;
    clock_gettime(CLOCK_REALTIME, &startTime);

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], NULL, process_csv_data, (void *)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec) {
        --sec;
        n_sec += 1000000000L;
    }

    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);

    // TODO: Print the results from the shared data
    // This will depend on how you've structured sharedData to store the results
    // For now, just printing the count of processed rows.
    printf("Processed %d rows.\n", sharedData.count);

    // Cleanup
    fclose(csvFile);
    pthread_mutex_destroy(&csvMutex);
    pthread_mutex_destroy(&sharedData.mutex);

    return 0;
}



