#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// CODE DOESNT WORK. RETURNS A SEGMENTATION FAULT. DONT COMPILE THIS FILE IT WONT EVEN WORK ISTG //
//

struct KeyValue {
  char *key;
  char *value;
  struct KeyValue *next;
};

struct KeyValueDb {
  struct KeyValue *head;
};

void init_db(struct KeyValueDb *db) {
  db->head = NULL;
}

void insert(struct KeyValueDb *db, const char *key, const char *value) {
    
        printf("%s", strdup(key));
    
  // Allocate memory for the new key-value pair
  struct KeyValue *kv = malloc(sizeof(struct KeyValue));
  kv->key = strdup(key);
  kv->value = strdup(value);
  kv->next = NULL;

  // Insert the key-value pair at the front of the linked list
  kv->next = db->head;
  db->head = kv;
}

char *get(struct KeyValueDb *db, const char *key) {
  // Search for the key in the linked list
  struct KeyValue *kv = db->head;
  while (kv != NULL) {
    if (strcmp(kv->key, key) == 0) {
      // Key found, return the corresponding value
      return kv->value;
    }
    kv = kv->next;
  }

  // Key not found, return NULL
  return NULL;
}

void free_db(struct KeyValueDb *db) {
  // Free the memory for each key-value pair in the linked list
  struct KeyValue *kv = db->head;
  while (kv != NULL) {
    struct KeyValue *next = kv->next;
    free(kv->key);
    free(kv->value);
    free(kv);
    kv = next;
  }

  // Set the head of the linked list to NULL
  db->head = NULL;
}

int main() {
    struct KeyValueDb db;
    init_db(&db);

    insert(&db, "key1", "data");
    insert(&db, "test", "fun");
    insert(&db, "iq", 0);

    char* val1 = get(&db, "key1");
    char* val2 = get(&db, "test");
    char* val3 = get(&db, "iq");

    printf("key1: %s key2: %s key3: %s\n", val1, val2, val3);

    free_db(&db);

    return 0;
}