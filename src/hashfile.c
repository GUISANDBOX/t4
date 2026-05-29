#include "hashfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int globalDepth;
  int recordSize;
  int numBuckets;
  int bucketSize;
  long directoryOffset;
  long bucketDataOffset;
  int keyOffset;
  int keySize;
} Header;

typedef struct {
  int localDepth;
  int numRecords;
  long nextBucket;
} BucketMeta;

typedef struct {
  Header header;
  long *directory;
  FILE *file;
  FILE *hdrFile;
} sHashFile;

static int writeHeader(sHashFile *hashFile) {
  if (fseek(hashFile->hdrFile, 0, SEEK_SET) != 0) {
    return 0;
  }
  if (fwrite(&hashFile->header, sizeof(Header), 1, hashFile->hdrFile) != 1) {
    return 0;
  }
  fflush(hashFile->hdrFile);
  return 1;
}

static int writeDirectory(sHashFile *hashFile) {
  if (fseek(hashFile->hdrFile, hashFile->header.directoryOffset, SEEK_SET) !=
      0) {
    return 0;
  }
  if (fwrite(hashFile->directory, sizeof(long), hashFile->header.numBuckets,
             hashFile->hdrFile) != (size_t)hashFile->header.numBuckets) {
    return 0;
  }
  fflush(hashFile->hdrFile);
  return 1;
}

static int readDirectory(sHashFile *hashFile) {
  if (fseek(hashFile->hdrFile, hashFile->header.directoryOffset, SEEK_SET) !=
      0) {
    return 0;
  }
  return fread(hashFile->directory, sizeof(long), hashFile->header.numBuckets,
               hashFile->hdrFile) == (size_t)hashFile->header.numBuckets;
}

static int writeBucketMeta(sHashFile *hashFile, long bucketOffset,
                           BucketMeta *bucket) {
  if (fseek(hashFile->file, bucketOffset, SEEK_SET) != 0) {
    return 0;
  }
  if (fwrite(bucket, sizeof(BucketMeta), 1, hashFile->file) != 1) {
    return 0;
  }
  fflush(hashFile->file);
  return 1;
}

static int readBucketMeta(sHashFile *hashFile, long bucketOffset,
                          BucketMeta *bucket) {
  long current_pos = ftell(hashFile->file);
  fseek(hashFile->file, 0, SEEK_END);
  long file_size = ftell(hashFile->file);
  fseek(hashFile->file, current_pos, SEEK_SET);
  if (bucketOffset + (long)sizeof(BucketMeta) > file_size) {
    printf("readBucketMeta: bucketOffset %ld beyond file_size %ld\n",
           bucketOffset, file_size);
    return 0;
  }
  if (fseek(hashFile->file, bucketOffset, SEEK_SET) != 0) {
    printf("Erro: fseek bucketOffset falhou\n");
    return 0;
  }
  if (fread(bucket, sizeof(BucketMeta), 1, hashFile->file) != 1) {
    printf("Erro: fread bucketMeta falhou\n");
    return 0;
  }
  if (bucket->numRecords < 0 || bucket->numRecords > 1000) {
    printf("readBucketMeta: numRecords invalido %d, inicializando\n",
           bucket->numRecords);
    bucket->numRecords = 0;
    bucket->localDepth = hashFile->header.globalDepth;
    bucket->nextBucket = -1;
    writeBucketMeta(hashFile, bucketOffset, bucket);
  }
  return 1;
}

static int bucketCapacity(sHashFile *hashFile) {
  int available = hashFile->header.bucketSize - (int)sizeof(BucketMeta);
  if (available <= 0 || hashFile->header.recordSize <= 0) {
    return 0;
  }
  int capacity = available / hashFile->header.recordSize;
  return capacity;
}

static long allocateBucketPage(sHashFile *hashFile) {
  fflush(hashFile->file);
  if (fseek(hashFile->file, 0, SEEK_END) != 0) {
    return -1;
  }
  long offset = ftell(hashFile->file);

  if (fseek(hashFile->file, offset + hashFile->header.bucketSize - 1,
            SEEK_SET) == 0) {
    char zero = 0;
    fwrite(&zero, 1, 1, hashFile->file);
  }

  // Initialize the new bucket meta
  BucketMeta newBucket = {.localDepth = hashFile->header.globalDepth,
                          .numRecords = 0,
                          .nextBucket = -1};
  if (!writeBucketMeta(hashFile, offset, &newBucket)) {
    printf("Erro: writeBucketMeta falhou para novo bucket\n");
    return -1;
  }
  return offset;
}

static int expandDirectory(sHashFile *hashFile) {
  int oldNumBuckets = hashFile->header.numBuckets;
  int newNumBuckets = oldNumBuckets << 1;
  long *newDirectory = (long *)malloc((size_t)newNumBuckets * sizeof(long));
  if (!newDirectory) {
    printf("Erro: malloc falhou para newDirectory\n");
    return 0;
  }

  for (int i = 0; i < oldNumBuckets; i++) {
    newDirectory[i] = hashFile->directory[i];
    newDirectory[i + oldNumBuckets] = hashFile->directory[i];
  }

  hashFile->header.globalDepth++;
  hashFile->header.numBuckets = newNumBuckets;
  // directoryOffset continua sendo o mesmo em hdrFile

  if (!writeHeader(hashFile)) {
    free(newDirectory);
    return 0;
  }

  if (fseek(hashFile->hdrFile, hashFile->header.directoryOffset, SEEK_SET) !=
      0) {
    printf("Erro: fseek END falhou\n");
    free(newDirectory);
    return 0;
  }

  if (fwrite(newDirectory, sizeof(long), newNumBuckets, hashFile->hdrFile) !=
      (size_t)newNumBuckets) {
    printf("Erro: fwrite newDirectory falhou\n");
    free(newDirectory);
    return 0;
  }
  fflush(hashFile->hdrFile);

  free(hashFile->directory);
  hashFile->directory = newDirectory;
  return 1;
}

static int writeRecordToBucket(sHashFile *hashFile, HashItem item,
                               int bucketIndex) {
  long bucketOffset = hashFile->directory[bucketIndex];
  BucketMeta bucketMeta;
  if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
    return 0;
  }

  int maxRecords = bucketCapacity(hashFile);
  if (bucketMeta.numRecords >= maxRecords) {
    printf("Bucket %d cheio, numRecords=%d, max=%d\n", bucketIndex,
           bucketMeta.numRecords, maxRecords);
    return 0;
  }

  long recordPosition =
      bucketOffset + sizeof(BucketMeta) +
      (long)bucketMeta.numRecords * hashFile->header.recordSize;
  if (fseek(hashFile->file, recordPosition, SEEK_SET) != 0) {
    return 0;
  }

  if (fwrite(item, hashFile->header.recordSize, 1, hashFile->file) != 1) {
    return 0;
  }

  bucketMeta.numRecords++;
  if (!writeBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
    return 0;
  }

  return 1;
}

static int splitBucket(sHashFile *hashFile, int bucketIndex) {
  long oldBucketOffset = hashFile->directory[bucketIndex];
  BucketMeta oldBucket;
  if (!readBucketMeta(hashFile, oldBucketOffset, &oldBucket)) {
    return 0;
  }

  int oldLocalDepth = oldBucket.localDepth;
  int newLocalDepth = oldLocalDepth + 1;
  if (newLocalDepth > hashFile->header.globalDepth) {
    return 0;
  }

  int oldNumRecords = oldBucket.numRecords;
  long newBucketOffset = allocateBucketPage(hashFile);
  if (newBucketOffset < 0) {
    return 0;
  }

  BucketMeta newBucket = {
      .localDepth = newLocalDepth, .numRecords = 0, .nextBucket = 0};

  oldBucket.localDepth = newLocalDepth;
  oldBucket.numRecords = 0;

  int numBuckets = hashFile->header.numBuckets;
  for (int i = 0; i < numBuckets; i++) {
    if (hashFile->directory[i] == oldBucketOffset) {
      if (((i >> oldLocalDepth) & 1) != 0) {
        hashFile->directory[i] = newBucketOffset;
      }
    }
  }

  if (!writeDirectory(hashFile)) {
    return 0;
  }

  if (!writeBucketMeta(hashFile, oldBucketOffset, &oldBucket)) {
    return 0;
  }

  if (!writeBucketMeta(hashFile, newBucketOffset, &newBucket)) {
    return 0;
  }

  if (oldNumRecords > 0) {
    int recordSize = hashFile->header.recordSize;
    char *buffer = (char *)malloc((size_t)oldNumRecords * recordSize);
    if (!buffer) {
      printf("Erro: malloc buffer falhou, oldNumRecords=%d, recordSize=%d\n",
             oldNumRecords, recordSize);
      return 0;
    }

    long readPosition = oldBucketOffset + sizeof(BucketMeta);
    if (fseek(hashFile->file, readPosition, SEEK_SET) != 0) {
      free(buffer);
      return 0;
    }

    if (fread(buffer, recordSize, (size_t)oldNumRecords, hashFile->file) !=
        (size_t)oldNumRecords) {
      free(buffer);
      return 0;
    }

    HashFile tempHash = (HashFile)hashFile;
    for (int i = 0; i < oldNumRecords; i++) {
      char *record = buffer + (size_t)i * recordSize;
      char *recordKey = record + hashFile->header.keyOffset;
      int targetBucketIndex = getKey(recordKey, hashFile->header.globalDepth);
      if (targetBucketIndex < 0 ||
          targetBucketIndex >= hashFile->header.numBuckets) {
        free(buffer);
        return 0;
      }
      if (!adicionarHashItem(&tempHash, record, recordKey)) {
        printf("Erro: falha ao re-inserir record i=%d\n", i);
        free(buffer);
        return 0;
      }
    }

    free(buffer);
  }

  return 1;
}

HashFile criarHashFile(char *nome, int recordSize, int bucketSize) {

  char hdrName[256];
  strcpy(hdrName, nome);
  char *dot = strrchr(hdrName, '.');
  if (dot) {
    strcpy(dot, ".hfc");
  } else {
    strcat(hdrName, ".hfc");
  }

  int d = 1;
  int numBuckets = 1 << d;
  Header header = {.globalDepth = d,
                   .recordSize = recordSize,
                   .numBuckets = numBuckets,
                   .bucketSize = bucketSize,
                   .directoryOffset = sizeof(Header),
                   .bucketDataOffset =
                       0, // Os buckets começam no início do arquivo de dados
                   .keyOffset = 0,
                   .keySize = 0};

  if (recordSize <= 0 || bucketSize <= (int)sizeof(BucketMeta)) {
    return NULL;
  }

  sHashFile *hashFile = (sHashFile *)malloc(sizeof(sHashFile));
  if (!hashFile) {
    return NULL;
  }

  hashFile->directory = (long *)malloc((size_t)numBuckets * sizeof(long));
  if (!hashFile->directory) {
    free(hashFile);
    return NULL;
  }

  for (int i = 0; i < numBuckets; i++) {
    hashFile->directory[i] = header.bucketDataOffset + (long)i * bucketSize;
  }

  hashFile->header = header;
  hashFile->file = fopen(nome, "wb+");
  if (!hashFile->file) {
    free(hashFile->directory);
    free(hashFile);
    return NULL;
  }

  hashFile->hdrFile = fopen(hdrName, "wb+");
  if (!hashFile->hdrFile) {
    fclose(hashFile->file);
    free(hashFile->directory);
    free(hashFile);
    return NULL;
  }

  if (fwrite(&hashFile->header, sizeof(Header), 1, hashFile->hdrFile) != 1) {
    fclose(hashFile->file);
    fclose(hashFile->hdrFile);
    free(hashFile->directory);
    free(hashFile);
    return NULL;
  }

  if (fwrite(hashFile->directory, sizeof(long), numBuckets,
             hashFile->hdrFile) != (size_t)numBuckets) {
    fclose(hashFile->file);
    fclose(hashFile->hdrFile);
    free(hashFile->directory);
    free(hashFile);
    return NULL;
  }
  fflush(hashFile->hdrFile);

  BucketMeta bucket = {.localDepth = d, .numRecords = 0, .nextBucket = -1};

  for (int i = 0; i < numBuckets; i++) {
    if (!writeBucketMeta(hashFile, hashFile->directory[i], &bucket)) {
      fclose(hashFile->file);
      fclose(hashFile->hdrFile);
      free(hashFile->directory);
      free(hashFile);
      return NULL;
    }
  }

  if (fseek(hashFile->file, (long)numBuckets * bucketSize - 1, SEEK_SET) == 0) {
    char zero = 0;
    fwrite(&zero, 1, 1, hashFile->file);
  }

  fflush(hashFile->file);
  return hashFile;
}

HashFile lerHashFile(char *file_name) {
  sHashFile *hashFile = (sHashFile *)malloc(sizeof(sHashFile));
  if (!hashFile) {
    return NULL;
  }

  hashFile->file = fopen(file_name, "rb+");
  if (!hashFile->file) {
    free(hashFile);
    return NULL;
  }

  char hdrName[256];
  strcpy(hdrName, file_name);
  char *dot = strrchr(hdrName, '.');
  if (dot) {
    strcpy(dot, ".hfc");
  } else {
    strcat(hdrName, ".hfc");
  }
  hashFile->hdrFile = fopen(hdrName, "rb+");
  if (!hashFile->hdrFile) {
    fclose(hashFile->file);
    free(hashFile);
    return NULL;
  }

  if (fread(&hashFile->header, sizeof(Header), 1, hashFile->hdrFile) != 1) {
    fclose(hashFile->file);
    fclose(hashFile->hdrFile);
    free(hashFile);
    return NULL;
  }

  if (hashFile->header.numBuckets <= 0) {
    fclose(hashFile->file);
    fclose(hashFile->hdrFile);
    free(hashFile);
    return NULL;
  }

  hashFile->directory =
      (long *)malloc((size_t)hashFile->header.numBuckets * sizeof(long));
  if (!hashFile->directory) {
    fclose(hashFile->file);
    fclose(hashFile->hdrFile);
    free(hashFile);
    return NULL;
  }

  if (!readDirectory(hashFile)) {
    fclose(hashFile->file);
    fclose(hashFile->hdrFile);
    free(hashFile->directory);
    free(hashFile);
    return NULL;
  }

  return hashFile;
}

void printHashFileInfo(HashFile hash) {
  sHashFile *hashFile = (sHashFile *)hash;
  if (!hashFile) {
    return;
  }

  printf("Global Depth: %d\n", hashFile->header.globalDepth);
  printf("Record Size: %d\n", hashFile->header.recordSize);
  printf("Number of Buckets: %d\n", hashFile->header.numBuckets);
  printf("Bucket Size: %d\n", hashFile->header.bucketSize);
  printf("Directory Offset: %ld\n", hashFile->header.directoryOffset);
  printf("Bucket Data Offset: %ld\n", hashFile->header.bucketDataOffset);
  printf("Key Offset: %d\n", hashFile->header.keyOffset);
  printf("Key Size: %d\n", hashFile->header.keySize);
  printf("\nDirectory:\n");
  for (int i = 0; i < hashFile->header.numBuckets; i++) {
    printf("Dir[%d] = %ld\n", i, hashFile->directory[i]);
  }
}

int getBucketsLength(HashFile hash) {
  if (!hash)
    return 0;
  sHashFile *hashFile = (sHashFile *)hash;
  return hashFile->header.numBuckets;
}

int getKey(char *key, int depth) {
  if (!key || depth <= 0) {
    return 0;
  }

  unsigned int hashed = (unsigned int)hashString(key);
  unsigned int mask;

  if (depth >= (int)(sizeof(mask) * 8)) {
    mask = ~0u;
  } else {
    mask = (1u << depth) - 1u;
  }

  return (int)(hashed & mask);
}

int adicionarHashItem(HashFile *hash, HashItem item, char *key) {
  if (!hash || !*hash || !item || !key) {
    return 0;
  }

  sHashFile *hashFile = (sHashFile *)(*hash);
  if (!hashFile->file) {
    return 0;
  }

  while (1) {
    int bucketIndex = getKey(key, hashFile->header.globalDepth);
    if (bucketIndex < 0 || bucketIndex >= hashFile->header.numBuckets) {
      return 0;
    }

    if (writeRecordToBucket(hashFile, item, bucketIndex)) {
      fflush(hashFile->file);
      return 1;
    }

    long bucketOffset = hashFile->directory[bucketIndex];
    BucketMeta bucketMeta;
    if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
      printf("Erro: falha ao ler bucketMeta\n");
      return 0;
    }

    if (bucketMeta.localDepth == hashFile->header.globalDepth) {
      printf("Tentando expandir directory\n");
      if (!expandDirectory(hashFile)) {
        printf("Erro: falha ao expandir directory\n");
        return 0;
      }
      continue;
    }

    if (!splitBucket(hashFile, bucketIndex)) {
      printf("Erro: falha ao split bucket\n");
      return 0;
    }
  }
}

int atualizarHashItem(HashFile *hash, HashItem item, char *key) {
  if (!hash || !*hash || !item || !key) {
    return 0;
  }

  sHashFile *hashFile = (sHashFile *)(*hash);
  if (!hashFile->file) {
    return 0;
  }

  int bucketIndex = getKey(key, hashFile->header.globalDepth);
  if (bucketIndex < 0 || bucketIndex >= hashFile->header.numBuckets) {
    return 0;
  }

  long bucketOffset = hashFile->directory[bucketIndex];
  BucketMeta bucketMeta;
  if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
    return 0;
  }

  int recordSize = hashFile->header.recordSize;
  if (recordSize <= 0 || bucketMeta.numRecords <= 0) {
    return 0;
  }

  char *record = (char *)malloc(recordSize);
  if (!record) {
    return 0;
  }

  long currentPosition = bucketOffset + sizeof(BucketMeta);
  size_t keyLength = strlen(key) + 1;

  for (int i = 0; i < bucketMeta.numRecords; i++) {
    if (fseek(hashFile->file, currentPosition, SEEK_SET) != 0) {
      break;
    }

    if (fread(record, recordSize, 1, hashFile->file) != 1) {
      break;
    }

    char *recordKey = record + hashFile->header.keyOffset;
    if (strncmp(recordKey, key, keyLength) == 0) {
      if (fseek(hashFile->file, currentPosition, SEEK_SET) != 0) {
        free(record);
        return 0;
      }
      int result = fwrite(item, recordSize, 1, hashFile->file) == 1;
      free(record);
      return result;
    }

    currentPosition += recordSize;
  }

  free(record);
  return 0;
}

HashItem buscarHashItem(HashFile hash, char *key) {
  if (!hash || !key) {
    return NULL;
  }

  sHashFile *hashFile = (sHashFile *)hash;
  if (!hashFile->file) {
    return NULL;
  }

  int bucketIndex = getKey(key, hashFile->header.globalDepth);
  if (bucketIndex < 0 || bucketIndex >= hashFile->header.numBuckets) {
    return NULL;
  }

  long bucketOffset = hashFile->directory[bucketIndex];
  BucketMeta bucketMeta;
  if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
    return NULL;
  }

  int recordSize = hashFile->header.recordSize;
  if (recordSize <= 0 || bucketMeta.numRecords <= 0) {
    return NULL;
  }

  char *record = (char *)malloc(recordSize);
  if (!record) {
    return NULL;
  }

  long currentPosition = bucketOffset + sizeof(BucketMeta);
  size_t keyLength = strlen(key) + 1;

  for (int i = 0; i < bucketMeta.numRecords; i++) {
    printf("buscarHashItem: i=%d, currentPosition=%ld\n", i, currentPosition);
    if (fseek(hashFile->file, currentPosition, SEEK_SET) != 0) {
      printf("buscarHashItem: fseek failed\n");
      break;
    }

    if (fread(record, recordSize, 1, hashFile->file) != 1) {
      printf("buscarHashItem: fread failed\n");
      break;
    }

    char *recordKey = record + hashFile->header.keyOffset;
    if (strncmp(recordKey, key, keyLength) == 0) {
      return (HashItem)record;
    }

    currentPosition += recordSize;
  }

  free(record);
  return NULL;
}

int hashString(char *str) {
  int hash = 0;
  while (*str) {
    hash = (hash * 31) + *str++;
  }
  return hash;
}

void removerHashItem(HashFile hash, char *key) {
  if (!hash || !key) {
    return;
  }

  sHashFile *hashFile = (sHashFile *)hash;
  if (!hashFile->file) {
    return;
  }

  int bucketIndex = getKey(key, hashFile->header.globalDepth);
  if (bucketIndex < 0 || bucketIndex >= hashFile->header.numBuckets) {
    return;
  }

  long bucketOffset = hashFile->directory[bucketIndex];
  BucketMeta bucketMeta;
  if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta)) {
    return;
  }

  int recordSize = hashFile->header.recordSize;
  if (recordSize <= 0 || bucketMeta.numRecords <= 0) {
    return;
  }

  char *record = (char *)malloc(recordSize);
  if (!record) {
    return;
  }

  long currentPosition = bucketOffset + sizeof(BucketMeta);
  size_t keyLength = strlen(key) + 1;

  for (int i = 0; i < bucketMeta.numRecords; i++) {
    printf("removerHashItem: i=%d, currentPosition=%ld\n", i, currentPosition);
    if (fseek(hashFile->file, currentPosition, SEEK_SET) != 0) {
      printf("removerHashItem: fseek failed\n");
      break;
    }

    if (fread(record, recordSize, 1, hashFile->file) != 1) {
      printf("removerHashItem: fread failed\n");
      break;
    }

    char *recordKey = record + hashFile->header.keyOffset;
    if (strncmp(recordKey, key, keyLength) == 0) {

      if (i < bucketMeta.numRecords - 1) {
        // Lê o último registro
        long lastPosition = bucketOffset + sizeof(BucketMeta) +
                            (bucketMeta.numRecords - 1) * recordSize;
        char *lastRecord = (char *)malloc(recordSize);
        fseek(hashFile->file, lastPosition, SEEK_SET);
        fread(lastRecord, recordSize, 1, hashFile->file);

        // Sobrescreve o registro atual com o último
        fseek(hashFile->file, currentPosition, SEEK_SET);
        fwrite(lastRecord, recordSize, 1, hashFile->file);
        free(lastRecord);
      }

      // Atualiza o bucketMeta
      bucketMeta.numRecords--;
      fseek(hashFile->file, bucketOffset, SEEK_SET);
      fwrite(&bucketMeta, sizeof(BucketMeta), 1, hashFile->file);
      fflush(hashFile->file);

      break;
    }

    currentPosition += recordSize;
  }

  free(record);
}

int getListaItens(HashFile hash, HashItem *itens) {
  if (!hash || !itens)
    return 0;
  sHashFile *hashFile = (sHashFile *)hash;
  if (!hashFile->file)
    return 0;

  int count = 0;
  for (int i = 0; i < hashFile->header.numBuckets; i++) {
    int isDuplicate = 0;
    for (int j = 0; j < i; j++) {
      if (hashFile->directory[j] == hashFile->directory[i]) {
        isDuplicate = 1;
        break;
      }
    }
    if (isDuplicate)
      continue;

    long bucketOffset = hashFile->directory[i];
    BucketMeta bucketMeta;
    if (!readBucketMeta(hashFile, bucketOffset, &bucketMeta))
      continue;

    long currentPosition = bucketOffset + sizeof(BucketMeta);
    for (int k = 0; k < bucketMeta.numRecords; k++) {
      char *record = malloc(hashFile->header.recordSize);
      fseek(hashFile->file, currentPosition, SEEK_SET);
      fread(record, hashFile->header.recordSize, 1, hashFile->file);
      itens[count++] = (HashItem)record;
      currentPosition += hashFile->header.recordSize;
    }
  }
  return count;
}