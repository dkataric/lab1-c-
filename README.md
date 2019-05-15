# Operacijski sustavi - lab1


Vježba-1. Posebni prosti brojevi
Zadatak
Ostvariti program koji u međuspremnik sprema posebne 64-bitovne proste brojeve i neke od njih koristi. Posebnost željenih brojeva je određena s dva kriterija:

    Kada je broj prikazan binarnim zapisom, najviše se dvije iste znamenke mogu pojaviti u nizu (npr. dio broja *1101001* je prihvatljiv dok dio *10001101* nije zbog tri nule u slijedu).
    Broj mora biti prost*.

Skica rješenja

Program je zadan sljedećim pseudokodom.

Zajednička struktura podataka: MS[] - međuspremnik od barem 5 elemenata; popunjavati ga kružno, uzimati "najstariji" broj (kružno)

program
{
   ponavljaj {
      x = pseudo_slučajni_64_bitovni_broj() | 1 //da bude neparni
      dok je ( test_bitovi(x) == LAŽ ILI test_pseudo_prost(x) == LAŽ ) {
         ako je ( x <= NAJVEĆI_BROJ - 2 ) //NAJVEĆI_BROJ = 2^64 - 1 = 0xffffffffffffffffULL
            x = x + 2
         inače
            x = pseudo_slučajni_64_bitovni_broj() | 1
      }
      stavi x u MS
   }
   dok je provjera_zahtjeva() != KRAJ_RADA
}

provjera_zahtjeva() {
   ako "nije prošla bar 1 sekunda od zadnje provjere zahtjeva" tada {
      vrati NIJE_KRAJ
   }
   inače {
      ako "postoji novi zahtjev za brojem" tada
         //neka vjerojatnost za to bude 50%
         ispiši vrijeme (u sekundama)
         ispiši idući broj iz MS (MS[I]; I = (I+1) mod N)
         ispiši sadržaj MS[]-a (samo zadnje 2 znamenke svakog broja)
      ako "postoji zahtjev za kraj rada" tada
         //neka vjerojatnost za to bude 10%
         vrati KRAJ_RADA
      inače
         vrati NIJE_KRAJ
   }
}

Za generiranje slučajnih brojeva uzeti neki od generatora ili stvoriti svoj. Npr. jednostavan generator se može ostvariti formulom:

rnd = ( rnd * A ) mod B

Za A i B uzeti velike brojeve, npr. A=8531648002905263869ULL, B=18116652324302926351ULL, početna vrijednost za rnd može biti trenutno vrijeme (time(NULL)). Obzirom da se radi s jako velikim brojevima u C-u treba dodati sufix ULL (unsigned long long tip podataka).

Alternativno, broj se može generirati bit po bit i pritom odmah paziti da tri u nizu nisu jednaka. Iako je to malo sporije, kasnije se može isplatiti pri provjeri uniformnosti.

Provjera ima li više od dva uzastopna bita u nuli i jedinici može se provjeriti tako da se u petlji uzimaju svaka tri uzastopna bita i provjere jesu li možda sva tri nula ili su sva tri jedinice. Ako jesu onda treba tražiti idući broj koji to nema. Provjeriti je li algoritam dobro radi (npr. upisivanjem tog broja u kalkulator i pretvorbom u binarni).

Obzirom da se radi velikim brojevima, traženje idućeg broja koji nema tri uzastopna bita jednaka mogla bi potrajati jako dugo. Stoga se može ograničiti ta petlja na npr. nekoliko milijardi operacija. Ako se tad ne nađe takav broj potražiti idući prosti broj.

(*) Sličan problem postoji s provjerom je li broj prost. Stoga se može provjera raditi dijeljenjem tog broja samo s manjim brojem brojeva, npr. provjeriti je li broj djeljiv s prvih nekoliko desetaka tisuća brojeva (npr. prvih 30-100 tisuća brojeva).

Podesiti ta ograničenja tako da se brojevi ne generiraju prečesto niti prerijetko (npr. ne više od nekoliko u sekudni, ne dulje od nekoliko sekundi po broju).

Primjer nekoliko pokretanja iz kojeg se vidi i različiti brojevi i različita trajanja programa.
Rad s bitovima

Broj koji na mjestu i ima jedan se generira sa (1<<i) (npr. 100000 se dobiva sa 1<<5).

Je li i-ti bit broja A jedinica provjerava se uvjetom: A & (1<<i)) != 0 (& je binarno I, ne logičko).

Vrijednost izraza A & (1<<i) je nula ili jedinica na i-tom mjestu broja u kojem su svi ostali bitovi nule (0b0...000x000...0). Pomakom tog broja desno može se dobiti broj kojemu je taj bit na prvom mjestu (ili npr. ((A>>i) & 1) == 1).

U C-u, (1<<i) generira 32-bitovni broj (uglavnom). Da bi se dobio 64-bitovni, potrebno je "jedinicu" zapisati kao 64-bitovni broj. To se može napraviti dodavanjem sufiksa ULL (npr. umjesto (1<<i) pisati (1ULL<<i)).

Osim generiranja slučajnih brojeva (za što se mogu koristiti funkcije iz biblioteka), ostalo ostvariti u programu - ne koristiti pomoćne funkcije (npr. ne pretvarati broj u niz znakova 0 i 1 te tražiti ima li podniz od 3 nule ili jedinice).

Mnoge operacije jezgre OS-a i drugih algoritama (npr. UI naprave, sinkronizacija) koriste razne zastavice koje treba ispitati/postaviti/obrisati. Zato je rad s bitovima jedan od naglasaka ove vježbe.
Tip podataka za 64 bitovne brojeve (u C-u)

Prethodno je spomenut tip podatka unsigned long long koji u većini prevoditelja (kompajlera) generira 64-bitovni tip pozitivnih cijelih brojeva (od nule do 2^64-1). Bez unsigned dijela to bi bili cijeli brojevi u rasponu -2^63 (1 i sve nule u binarnom zapisu) do 2^63-1 (nula i sve jedinice). Za ispis takvih brojeva u printf treba dodati format %llu ili %lld(npr. printf("%llu", x);).

"Sigurniji" način dobivanja broja nekih dimenzija jest korištenjem biblioteke stdint.h te inttypes.h za ispis, kao u primjeru koji slijedi.

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h> //ovaj zapravo uključuje prethodni

int main() {
	int64_t  sx = 0xfedcba9876543210LL;  //negativan broj -81985529216486896
	uint64_t ux = 0xfedcba9876543210ULL; //pozitivan broj 18364758544493064720

	printf ("%" PRId64 "\n", sx); //pravi odabir za sx
	printf ("%" PRIu64 "\n", ux); //pravi odabir za ux
	//po potrebi koristiti PRIx64 za heksadekadski ispis

	return 0;
}

U rješenju se može koristiti bilo koji tip podatka koji daje 64-bitovni nenegativni broj.

