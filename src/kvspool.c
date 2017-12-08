#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "kvspool.h"
#include "kvspool_internal.h"

void sp_oom(void) {
  fprintf(stderr, "out of memory\n");
  exit(-1);
}

void* kv_set_new(void) {
  kvset_t *set;
  if ( (set = malloc(sizeof(kvset_t))) == NULL) sp_oom();
  memset(set,0,sizeof(*set));
  return set;
}

void kv_set_clear(void*_set) {
  kvset_t *set = (kvset_t*)_set;
  kv_t *kv, *tmp;
  HASH_ITER(hh, set->kvs, kv, tmp) {  
    HASH_DEL(set->kvs, kv);
    free(kv->key); free(kv->val); free(kv);
  }
}

void kv_set_dump(void*_set,FILE *out) {
  kvset_t *set = (kvset_t*)_set;
  kv_t *kv;
  char c;
  int i;

  kv=NULL;
  while ( (kv=kv_next(set,kv))) {
    fprintf(out," %.*s: ", kv->klen, kv->key);
    fprintf(out, "%.*s\n", kv->vlen, kv->val);
  }
}

void kv_set_free(void*_set) {
  kvset_t *set = (kvset_t*)_set;
  kv_t *kv, *tmp;
  HASH_ITER(hh, set->kvs, kv, tmp) {
    HASH_DEL(set->kvs, kv);
    free(kv->key); free(kv->val);
    free(kv);
  }
  assert(set->kvs == NULL);
  free(set);
}

kv_t *kv_get(void*_set, char *key) {
  kv_t *kv;
  kvset_t *set = (kvset_t*)_set;
  HASH_FIND(hh, set->kvs, key, strlen(key), kv);
  return kv;
}

void kv_add(void*_set, const char *key, int klen, const char *val, int vlen) {
  kvset_t *set = (kvset_t*)_set;
  assert(klen); //assert(vlen);
  kv_t *kv;
 
  /* check if we're replacing an existing key */
  HASH_FIND(hh, set->kvs, key, klen, kv);
  if (kv) { /* yes, free the old value and replace it */
    free(kv->val);
    if ( (kv->val = malloc(vlen+1)) == NULL) sp_oom(); kv->vlen = vlen;
    memcpy(kv->val, val, vlen); kv->val[vlen]='\0';
    return;
  }
  /* new key. deep copy the key/val and add it, null term for convenience */
  if ( (kv = malloc(sizeof(*kv))) == NULL) sp_oom();
  if ( (kv->key = malloc(klen+1)) == NULL) sp_oom(); kv->klen = klen;
  if ( (kv->val = malloc(vlen+1)) == NULL) sp_oom(); kv->vlen = vlen;
  memcpy(kv->key, key, klen); kv->key[klen]='\0';
  memcpy(kv->val, val, vlen); kv->val[vlen]='\0';
  HASH_ADD_KEYPTR(hh,set->kvs,kv->key,kv->klen,kv);
}

int kv_len(void*_set) {
  kvset_t *set = (kvset_t*)_set;
  return set->kvs ? (HASH_COUNT(set->kvs)) : 0;
}

kv_t *kv_next(void*_set,kv_t *kv) {
  kvset_t *set = (kvset_t*)_set;
  if (!kv) return set->kvs; /* get first element */
  return kv->hh.next;
}

