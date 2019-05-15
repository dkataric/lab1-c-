#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#define KRAJ_RADA 0
#define NIJE_KRAJ 1

uint64_t MS[5] = {0ULL,0ULL,0ULL,0ULL,0ULL};
int najnoviji = 0;
int najstariji = 0;
int iduci = 0;
clock_t begin;
clock_t end;
clock_t vrijeme;

uint64_t slucajni64(void)
{
  uint64_t x = (rand() * time(NULL) * 648002905263869ULL)
                % 18116652324202926351ULL;
  return x;
}

int test_bitovi(uint64_t x)
{
  int i;

  //zadnja provjera indeksa je 61,62,63
  for (i = 0; i < 62; ++i) {
    //ako su 3 jedinice za redom vrati laz
    if (((x & (1ULL<<i)) != 0) &&
        ((x & (1ULL<<(i+1))) != 0) &&
        ((x & (1ULL<<(i+2))) != 0))
      return 0;
    //ako nije niti jedna jedinica vrati laz
    if (!(((x & (1ULL<<i)) != 0) ||
        ((x & (1ULL<<(i+1))) != 0) ||
        ((x & (1ULL<<(i+2))) != 0)))
      return 0;
  }
  //ako se petlja zavrsila onda nema 3 ponavljanja
  return 1;
}

int test_pseudo_prost(uint64_t x)
{

  int i;
  //provjera samo za neparne ali do 100
  for (i = 3; i < 10; i += 2) {
    if(x%i == 0)
      return 0;
  }
  return 1;
}

int postoji_novi_zahtjev(void)
{
  int n = rand();
  return n%2;
}

int zahtjev_za_kraj(void)
{
  int n = rand();
  if (n%100 < 10) return 1;
  return 0;
}

int provjera_zahtjeva(void)
{
  if ((double)(end - begin) / CLOCKS_PER_SEC < 1)
    return NIJE_KRAJ;
  else {
    if (postoji_novi_zahtjev()){
      printf("%03ld - ", (end - vrijeme) / CLOCKS_PER_SEC);
      printf("MS[] = { %02llx %02llx %02llx %02llx %02llx }",
            MS[0] & 0xFF, MS[1] & 0xFF, MS[2] & 0xFF,
            MS[3] & 0xFF, MS[4] & 0xFF);
            printf(" I=%d, U=%d ",iduci, najnoviji);
            printf("MS[I] = %llx \n", MS[iduci]);
      if (iduci == 4) iduci = 0;
      else ++iduci;
    }
    if (zahtjev_za_kraj())
      return KRAJ_RADA;
    else
      return NIJE_KRAJ;
  }
}

void program()
{
  do {
    begin = clock();
    int iteracija = 0;
    uint64_t x = slucajni64() | 1;
    while (test_bitovi(x) == 0 || test_pseudo_prost(x) == 0) {
      if (x <= 0xffffffffffffffffULL && iteracija < 60){
        ++iteracija;
        x += 2;
      }
      else {
        x = slucajni64() | 1;
        iteracija = 0;
      }
    }
    //stavi na mjesto najstarijeg u MS
    //popunjavamo od 0 prema desno (cirkularno)
    MS[najstariji] = x;
    najnoviji = najstariji;
    if (najstariji == 4)
      najstariji = 0;
    else
      najstariji++;
    end = clock();
  } while(provjera_zahtjeva() != KRAJ_RADA);
}


int main() {

  vrijeme = clock();
  program();

  return 0;
}
