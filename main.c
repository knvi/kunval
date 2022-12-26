#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_SIZE 256   // in chars
#define MAX_VALUE_SIZE 256 // in chars

typedef char *KeyType;
typedef char *ValueType;

// Create a struct to represent the entire database
typedef struct KeyValueDb
{
    FILE *fp;
} KeyValueDb;

// Open the database file for reading and writing
KeyValueDb *open_db(const char *filename)
{
    KeyValueDb *db = (KeyValueDb *)malloc(sizeof(KeyValueDb));
    db->fp = fopen(filename, "r+");
    if (db->fp == NULL)
    {
        db->fp = fopen(filename, "w+");
    }
    return db;
}

// Create a new key-value pair and add it to the database
void add(KeyValueDb *db, KeyType key, ValueType value)
{
    // note: lmao
    fprintf(db->fp, "%s %s\n", key, value);
}

// Search the database for a specific key and return the corresponding value
ValueType get(KeyValueDb *db, KeyType key)
{
    rewind(db->fp); // go back to the start of the file
    KeyType readKey = (ValueType)malloc(sizeof(char) * MAX_KEY_SIZE);
    ValueType value = (ValueType)malloc(sizeof(char) * MAX_VALUE_SIZE);
    while (fscanf(db->fp, "%s %s\n", readKey, value) == 2)
    {
        if (strcmp(readKey, key) == 0)
        {
            return value;
        }
    }

    free(readKey);
    free(value);
    return NULL;
}

// Update the value for a specific key in the database
void update(KeyValueDb *db, KeyType key, ValueType value)
{
    char tempFilename[256];
    sprintf(tempFilename, "%s.tmp", "database.txt");

    FILE *tempfp = fopen(tempFilename, "w");
    if (tempfp == NULL)
    {
        // Error creating temporary file
        return;
    }

    rewind(db->fp);
    KeyType readKey = (ValueType)malloc(sizeof(char) * MAX_KEY_SIZE);
    ValueType readValue = (ValueType)malloc(sizeof(char) * MAX_VALUE_SIZE);
    int keyFound = 0;
    while (fscanf(db->fp, "%s %s\n", readKey, readValue) == 2)
    { // Read key-value pair from the original file
        if (strcmp(readKey, key) == 0)
        {                                               // Check if the key matches the target key
            fprintf(tempfp, "%s %s\n", readKey, value); // Write the updated key-value pair to the temporary file
            keyFound = 1;
        }
        else
        {
            fprintf(tempfp, "%s %s\n", readKey, readValue); // Write the original key-value pair to the temporary file
        }
    }
    free(readKey);
    free(readValue);

    if (!keyFound)
    {
        fprintf(tempfp, "%s %s\n", key, value);
    }

    fclose(tempfp);
    fclose(db->fp);

    rename(tempFilename, "database.txt");

    // Open the file again for reading and writing
    db->fp = fopen("database.txt", "r+");
}

// Delete a key-value pair from the database
void delete(KeyValueDb *db, KeyType key)
{
    FILE *tempfp = tmpfile();
    if (tempfp == NULL)
    {
        return;
    }

    rewind(db->fp);
    KeyType readKey = (ValueType)malloc(sizeof(char) * MAX_KEY_SIZE);
    ValueType readValue = (ValueType)malloc(sizeof(char) * MAX_VALUE_SIZE);
    while (fscanf(db->fp, "%s %s\n", readKey, readValue) == 2)
    { // Read key-value pair from the original file
        if (strcmp(readKey, key) == 0)
        {
            fprintf(tempfp, "%s %s\n", readKey, readValue); // Write the key-value pair to the temporary file
        }
    }
    free(readKey);
    free(readValue);

    fclose(tempfp);
    fclose(db->fp);

    rename("tmpfileXXXXXX", "database.txt");

    // Open the file again for reading and writing
    db->fp = fopen("database.txt", "r+");
}

// Iterate through all key-value pairs in the database and print them
void printAll(KeyValueDb *db)
{
    rewind(db->fp);
    KeyType readKey = (ValueType)malloc(sizeof(char) * MAX_KEY_SIZE);
    ValueType readValue = (ValueType)malloc(sizeof(char) * MAX_VALUE_SIZE);
    while (fscanf(db->fp, "%s %s\n", readKey, readValue) == 2)
    { // Read key-value pair from the original file
        printf("Key: %s, value: %s\n", readKey, readValue);
    }

    free(readKey);
    free(readValue);
}

void free_db(KeyValueDb *db)
{
    // db is only one file
    free(db);
}

int main(void)
{
    KeyValueDb *db = open_db("database.txt");

    add(db, "1", "fruit");
    add(db, "banana", "fruit");
    add(db, "cherry", "fruit");

    printf("Value for key 'banana': %s\n", get(db, "1")); // prints "fruit"

    update(db, "banana", "vegetable");
    printf("Value for key 'banana': %s\n", get(db, "banana")); // prints "vegetable"

    delete (db, "apple");
    printf("Value for key 'apple': %s\n", get(db, "apple")); // prints NULL

    printAll(db); // prints key-value pairs for keys "banana" and "cherry"

    fclose(db->fp);
    free_db(db);

    return 0;
}
