#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int KeyType;
typedef char *ValueType;

// Create a struct to represent a single key-value pair
typedef struct KeyValuePair
{
    KeyType key;
    ValueType value;
    struct KeyValuePair *next;
} KeyValuePair;

// Create a struct to represent the entire database
typedef struct KeyValueDb
{
    KeyValuePair *first;
    KeyValuePair *last;
    int size;
} KeyValueDb;

// Create a new key-value pair and add it to the database
void add(KeyValueDb *db, KeyType key, ValueType value)
{
    KeyValuePair *pair = (KeyValuePair *)malloc(sizeof(KeyValuePair));
    pair->key = key;
    pair->value = strdup(value); // Make a copy of the value string
    pair->next = NULL;
    if (db->last == NULL)
    {
        db->first = pair;
        db->last = pair;
    }
    else
    {
        db->last->next = pair;
        db->last = pair;
    }
    db->size++;
}

// Search the database for a specific key and return the corresponding value
ValueType get(KeyValueDb *db, KeyType key)
{
    KeyValuePair *pair = db->first;
    while (pair != NULL)
    {
        if (pair->key == key)
        {
            return pair->value;
        }
        pair = pair->next;
    }

    return NULL;
}

// Update the value for a specific key in the database
void update(KeyValueDb *db, KeyType key, ValueType value)
{
    KeyValuePair *pair = db->first;
    while (pair != NULL)
    {
        if (pair->key == key)
        {
            free(pair->value);           // Free the old value string
            pair->value = strdup(value); // Make a copy of the new value string
            return;
        }
        pair = pair->next;
    }
    // Key not found, do nothing
}

// Delete a key-value pair from the database
void delete(KeyValueDb *db, KeyType key)
{
    KeyValuePair *pair = db->first;
    KeyValuePair *prev = NULL;
    while (pair != NULL)
    {
        if (pair->key == key)
        {
            if (prev == NULL)
            {
                // Special case for deleting the first element
                db->first = pair->next;
            }
            else
            {
                prev->next = pair->next;
            }
            if (pair->next == NULL)
            {
                // Special case for deleting the last element
                db->last = prev;
            }
            free(pair->value); // Free the value string
            free(pair);        // Free the key-value pair struct
            db->size--;
            return;
        }
        prev = pair;
        pair = pair->next;
    }
    // Key not found, do nothing
}

// Iterate through all key-value pairs in the database and print them
void printAll(KeyValueDb *db)
{
    KeyValuePair *pair = db->first;
    while (pair != NULL)
    {
        printf("Key: %d, Value: %s\n", pair->key, pair->value);
        pair = pair->next;
    }
}

void free_db(KeyValueDb *db) {
  KeyValuePair *pair = db->first;
  KeyValuePair *next;
  while (pair != NULL) {
    next = pair->next;
    free(pair->value);  // Free the value string
    free(pair);  // Free the key-value pair struct
    pair = next;
  }
  db->first = NULL;
  db->last = NULL;
  db->size = 0;
}

int main(void)
{
    KeyValueDb db;
    db.first = NULL;
    db.last = NULL;
    db.size = 0;

    add(&db, 1, "apple");
    add(&db, 2, "banana");
    add(&db, 3, "cherry");

    printf("Value for key 2: %s\n", get(&db, 2)); // prints "banana"

    update(&db, 2, "orange");
    printf("Value for key 2: %s\n", get(&db, 2)); // prints "orange"

    delete (&db, 1);
    printf("Value for key 1: %s\n", get(&db, 1)); // prints NULL

    printAll(&db); // prints key-value pairs for keys 2 and 3

    free_db(&db);
    return 0;
}