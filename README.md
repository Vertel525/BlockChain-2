# SUPAPRASTINTA BLOKU GRANDINE

## 1. Projekto aprašymas

Šio projekto tikslas – sukurti supaprastintą blokų grandinės (Blockchain) veikimo modelį naudojant C++ programavimo kalbą.

Programa generuoja vartotojus, jų pinigų likučius ir atsitiktines transakcijas. Transakcijos tikrinamos, įtraukiamos į kandidatus į blokus, o blokai kasami naudojant Proof of Work metodą.

Projektas sukurtas mokymosi tikslais, siekiant suprasti pagrindinius blokų grandinės veikimo principus.

## 2. Naudojamos technologijos

- C++
- Visual Studio
- STL bibliotekos
- Individualiai sukurtas maišos (Hash) algoritmas

## 3. Projekto struktūra

- `main.cpp` – pagrindinė programos funkcija.
- `blockchain.h` – vartotojų, transakcijų, blokų ir blokų grandinės struktūros bei funkcijų deklaracijos.
- `hash.h` – maišos funkcijų deklaracijos.
- `hash.cpp` – maišos funkcijų realizacija.
- `users.cpp` – vartotojų generavimas ir paieška.
- `transactions.cpp` – transakcijų generavimas, ID skaičiavimas ir tikrinimas.
- `blocks.cpp` – Merkle medžio, blokų kūrimo, kasimo ir transakcijų apdorojimo funkcijos.
- `validation.cpp` – blokų ir Proof of Work tikrinimas.
- `SimpleBlockChainTests` – atskiras testavimo projektas.

## 4. Programos veikimas

Paleidus programą:

1. Sugeneruojama 1000 vartotojų.
2. Kiekvienam vartotojui priskiriamas unikalus viešasis raktas ir pradinis pinigų likutis.
3. Sugeneruojama iki 10000 transakcijų.
4. Sukuriamas pradinis (Genesis) blokas.
5. Transakcijos tikrinamos ir paskirstomos į kandidatus į blokus.
6. Sukuriami penki kandidatiniai blokai.
7. Kandidatai kasami naudojant Proof of Work metodą.
8. Pasirenkamas tinkamiausias iškastas kandidatas.
9. Patikrinamas laimėjęs blokas ir atnaujinami vartotojų likučiai.
10. Pateikiama galutinė blokų grandinės ir transakcijų suvestinė.

## 5. Transakcijų tikrinimas

Prieš priimant transakciją tikrinama:

- Ar siuntėjas egzistuoja.
- Ar gavėjas egzistuoja.
- Ar siuntėjas ir gavėjas nėra tas pats vartotojas.
- Ar transakcijos suma yra teigiama.
- Ar transakcijos ID yra teisingas.
- Ar transakcija jau nėra patvirtinta arba įtraukta į transakcijų sąrašą.

Taip pat tikrinama, ar siuntėjui pakanka pinigų transakcijai atlikti ir ar nebus viršyta leistina pinigų suma.

## 6. Blokų grandinės principai

### Merkle medis

Transakcijų ID naudojami Merkle medžio lapams sudaryti. Kiekviename lygyje gretimi maišos rezultatai sujungiami ir iš jų apskaičiuojama nauja maišos reikšmė.

Jeigu lygyje yra nelyginis elementų skaičius, paskutinis elementas dubliuojamas.

Gauta galutinė reikšmė vadinama Merkle šaknimi.

### Proof of Work

Blokas laikomas iškastu, kai jo maišos reikšmė atitinka nustatytą sudėtingumo lygį.

Programa keičia nonce reikšmę ir pakartotinai skaičiuoja bloko maišos reikšmę, kol randamas tinkamas rezultatas arba pasiekiamas bandymų limitas.

### Kandidatų pasirinkimas

Kiekvienam blokų grandinės lygiui sudaromi penki kandidatiniai blokai. Jie tikrinami nepriklausomai, o tinkamiausias iškastas kandidatas pasirenkamas pagal transakcijų skaičių. Jeigu skaičius vienodas, naudojama maišos reikšmė kaip papildomas pasirinkimo kriterijus.

Tik laimėjusio kandidato transakcijos įtraukiamos į blokų grandinę.

## 7. Testavimas

Projektui sukurtas atskiras `SimpleBlockChainTests` testavimo projektas.

Testuojami šie atvejai:

- Pakeista transakcijos suma.
- Neigiama transakcijos suma.
- Nežinomas gavėjas.
- Nepakankamas vartotojo pinigų likutis.
- Neteisingas transakcijos ID.
- Pasikartojantis transakcijos ID.
- Pakartotinai pateikta jau patvirtinta transakcija.
- Pakeistas bloko turinys.
- Neteisingas ankstesnio bloko Hash.
- Neteisingas Proof of Work.
- Kelios transakcijos, kurių bendra suma viršija siuntėjo likutį.
- Merkle medžio skaičiavimai su 1, 2, 3 ir 5 transakcijomis.
- Pralaimėjusių kandidatų įtaka vartotojų likučiams.
- Bendros pinigų sumos išsaugojimas.

Testavimo projektas paleidžiamas atskirai nuo pagrindinės programos.

## 8. Apribojimai

Šis projektas yra supaprastintas blokų grandinės modelis, skirtas mokymuisi.

Jame nėra:

- Tikro lygiaverčių mazgų (P2P) tinklo.
- Paskirstyto konsensuso tarp nepriklausomų mazgų.
- Tikros blokų grandinės šakų reorganizavimo mechanizmo.
- Skaitmeninių parašų ir pilnos viešųjų bei privačiųjų raktų infrastruktūros.

Naudojamas maišos algoritmas yra mokomasis ir nėra skirtas realiam saugumui užtikrinti.

## 9. Projekto versijos

- **v0.1** – pradinis supaprastintos blokų grandinės modelis.
- **v0.2** – Merkle medis, transakcijų ir blokų tikrinimas, penkių kandidatų kasimas bei testavimas.
- **v0.3** – numatyti AI pagalba pasiūlyti ir patikrinti programos patobulinimai. Šios versijos pakeitimai ir jų palyginimas su v0.2 dokumentuojami atskirai.
