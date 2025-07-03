typedef struct {
  const char* key;
  const char* value;
} ht_item;

typedef struct {
  int count;  
  int size; 
  ht_item** items;
} ht_table;
