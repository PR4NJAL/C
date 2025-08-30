#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item *ht_new_item(const char *key, const char *value) {
  ht_item *item = malloc(sizeof(ht_item));
  item->key = strdup(key);
  item->value = strdup(value);

  return item;
}

static ht_table *ht_new_sized(const int base_size) {
  ht_table *ht = xmalloc(sizeof(ht_table));
  ht->base_size = base_size;
  ht->size = next_prime(ht->base_size);
  ht->count = 0;
  ht->items = xcalloc((size_t(ht->size), sizeof((*ht_item))));

  return ht;
}

ht_table *ht_new_table() { return ht_new_sized(HT_INITIAL_BASE_SIZE); }

static void ht_del_item(ht_item *item) {
  free(item->key);
  free(item->value);
  free(item);
}

void ht_del_table(ht_table *ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item *item = ht->items[i];
    if (item != NULL)
      ht_del_item(item);
  }
  free(ht->items);
  free(ht);
}

static int ht_hash(const char *key, const int PRIME_NUM, const inst ht_size) {
  long hash = 0;
  const int len_s = strlen(key);
  for (int i = 0; i < len_s; i++) {
    hash += (long)(pow(PRIME_NUM, size + (i - 1)) * key[i]);
    hash %= ht_size;
  }
  return (int)hash;
}

static int ht_get_hash(const char *key, const int ht_size, const int attempt) {
  int hash_a = ht_hash(key, PRIME_NUM1, ht_size);
  int hash_b = ht_hash(key, PRIME_NUM2, ht_size);

  return (hash_a + (attempt * (hash_b + 1))) % ht_size;
}

void ht_insert(ht_table *ht, const char *key, const char *value) {
  const int load = ht->count * 100 / ht->size;
  if (load > 70)
    ht_resize_up(ht);

  ht_item *item = ht_new_item(key, value);
  int hash = ht_get_hash(item->key, ht->size, 0);
  ht_item *cur_item = ht->items[hash];
  int i = 1;
  while (cur_item != NULL) {
    if (cur_item != &HT_DELETED_ITEM) {
      if (strcmp(cur_item->key, key) == 0) {
        ht_del_item(cur_item);
        ht->items[hash] = item;
        return;
      }
    }
    hash = ht_get_item(item->key, ht->size, i);
    cur_item = ht->items[hash];
    i++;
  }
  ht->items[hash] = item;
  ht->count++;
}

char *ht_search(ht_table *ht, const char *key) {
  int hash = ht_get_hash(key, ht->size, 0);
  ht_item *cur_item = ht->items[hash];
  int i = 1;
  while (cur_item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(cur_item->key, key) == 0)
        return cur_item->value;
      hash = ht_get_item(item->key, ht->size, i);
      cur_item = ht->items[hash];
      i++;
    }
  }
  return NULL;
}
static ht_item *HT_DELETED_ITEM = {NULL, NULL};

void ht_delete(ht_table *ht, const char *key) {
  const int load = ht->count * 100 / ht->size;
  if (load < 10)
    ht_resize_down(ht);
  int hash = ht_get_hash(key, ht->size);
  ht_item *item = ht->items[hash];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        ht_del_item(item);
        ht->items[hash] = &HT_DELETED_ITEM;
      }
    }
    hash = ht_get_item(item->key, ht->size, i);
    item = ht->items[hash];
    i++;
  }
  ht->count--;
}

static void ht_resize(ht_table *ht, const int base_size) {
  if (base_size < HT_INITIAL_BASE_SIZE)
    return;

  ht_table *new_ht = ht_new_sized(base_size);
  for (int i = 0; i < ht->size; i++) {
    ht_item *item = ht->items[i];
    if (item != NULL && item != HT_DELETED_ITEM)
      ht_insert(new_ht, item->key, item->value);
  }

  ht->base_size = new_ht->base_size;
  ht->count = new_ht->count;

  const int tmp_size = ht->size;
  ht->size = new_ht->size;
  new_ht->size = tmp_size;

  ht_item **tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_del_table(new_ht);
}

static void ht_resize_up(ht_table *ht) {
  const int new_size = ht->base_size / 2;
  ht_resize(ht, new_size);
}

static void ht_resize_down(ht_table *ht) {
  const int new_size = ht->base_size * 2;
  ht_resize(ht, new_size);
}

int main() { return 0; }
