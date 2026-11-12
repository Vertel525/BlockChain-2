# Supaprastina blokų grandinė

## 1. Projekto aprašymas

Šiame projekte realizuota supaprastinta blokų grandinės (Blockchain) simuliacija naudojant C++ programavimo kalbą.

Programa generuoja vartotojus ir jų transakcijas, tikrina transakcijų galiojimą, formuoja blokus ir prijungia juos prie blokų grandinės. Taip pat realizuotas Proof of Work mechanizmas, Merkle medis ir blokų validavimas.

## 2. Pagrindinės funkcijos

- Vartotojų generavimas.
- Atsitiktinių transakcijų generavimas.
- Transakcijų galiojimo tikrinimas.
- Vartotojų pinigų likučių atnaujinimas.
- Genesis bloko sukūrimas.
- Merkle medžio šaknies apskaičiavimas.
- Blokų maišos apskaičiavimas.
- Proof of Work mechanizmas.
- Kelių kandidatinių blokų formavimas ir palyginimas.
- Blokų validavimas prieš prijungiant juos prie grandinės.
- Galutinės statistikos pateikimas.

## 3. Versijų pakeitimai

### v0.1

Sukurta pagrindinė blokų grandinės simuliacijos struktūra ir pagrindinis programos funkcionalumas.

### v0.2

Tobulinta transakcijų generavimo, tikrinimo ir blokų apdorojimo logika. Atlikti programos veikimo testai.

### v0.3

Atliktos programos našumo optimizacijos:

1. **Vartotojų paieška.** Vietoje nuoseklios paieškos panaudotas `unordered_map`, leidžiantis greičiau surasti vartotoją pagal viešąjį raktą.
2. **Transakcijų ID paieška.** Panaudotas `unordered_set`, leidžiantis efektyviau tikrinti pasikartojančius transakcijų ID.
3. **Transakcijų šalinimas.** Patvirtintos transakcijos iš laukiančių transakcijų sąrašo šalinamos naudojant `remove_if` kartu su `erase`.

## 4. Našumo palyginimas

Našumas palygintas tarp v0.2 ir v0.3 versijų.

Matavimų sąlygos:
- 1000 vartotojų.
- 10 000 transakcijų.
- 5 matavimų pakartojimai.
- Pateikiamas penkių matavimų vidurkis.
- Laikas matuojamas milisekundėmis (ms).

| Matavimas | v0.2 | v0.3 |
|---|---:|---:|
| Vartotojų generavimas | 74,1764 ms | 64,8821 ms |
| Transakcijų generavimas | 20 411,5 ms | 2 451,8 ms |

### Rezultatų analizė

**Vartotojų generavimas:**
- v0.2: 74,1764 ms.
- v0.3: 64,8821 ms.
- Pagreitėjimas: apie 12,53 %.

**Transakcijų generavimas:**
- v0.2: 20 411,5 ms.
- v0.3: 2 451,8 ms.
- Pagreitėjimas: apie 8,32 karto.
- Vykdymo laikas sumažėjo apie 87,98 %.

Didžiausias našumo pagerėjimas pastebimas transakcijų generavimo metu. Tai rodo, kad pritaikytos paieškos optimizacijos reikšmingai sumažino šios operacijos vykdymo laiką.

*Pastaba: pateikti rezultatai priklauso nuo naudoto kompiuterio techninių charakteristikų ir programos vykdymo aplinkos. Blokų apdorojimo ir kasimo laikas į šiuos matavimus neįtrauktas.*

## 5. Automatinių testų rezultatai

Projektui sukurtas atskiras `SimpleBlockChainTests` testavimo projektas.

Atlikta 14 automatinių testų:

| Nr. | Testas | Rezultatas |
|---:|---|---|
| 1 | Neteisinga transakcijos suma | Pavyko |
| 2 | Neigiama transakcijos suma | Pavyko |
| 3 | Nežinomas gavėjas | Pavyko |
| 4 | Nepakankamas siuntėjo likutis | Pavyko |
| 5 | Neteisingas transakcijos ID | Pavyko |
| 6 | Pasikartojantis transakcijos ID | Pavyko |
| 7 | Jau patvirtintos transakcijos pakartojimas | Pavyko |
| 8 | Pakeisto bloko turinio aptikimas | Pavyko |
| 9 | Neteisinga ankstesnio bloko maiša | Pavyko |
| 10 | Neteisingas Proof of Work | Pavyko |
| 11 | Nepakankamas bendras transakcijų likutis | Pavyko |
| 12 | Merkle medžio patikrinimas | Pavyko |
| 13 | Pralaimėję kandidatų blokai nekeičia likučių | Pavyko |
| 14 | Bendras pinigų kiekis išlieka nepakitęs | Pavyko |

**Galutinis testavimo rezultatas: 14 iš 14 testų sėkmingi.**

Visi atlikti automatiniai testai praėjo sėkmingai. Tai patvirtina, kad tikrinami pagrindiniai transakcijų, blokų ir pinigų likučių vientisumo scenarijai veikia tinkamai.

## 6. Programos paleidimas

Projektas sukurtas naudojant C++ ir gali būti atidaromas per Microsoft Visual Studio.

1. Atidaryti projekto sprendimo (`.sln`) failą.
2. Pasirinkti pagrindinį programos projektą.
3. Pasirinkti `Debug` arba `Release` konfigūraciją.
4. Sukompiliuoti projektą.
5. Paleisti programą.

Automatinius testus galima vykdyti atskirai, pasirinkus `SimpleBlockChainTests` testavimo projektą.

## 7. Išvados

Projekto metu sukurta veikianti supaprastinta blokų grandinės simuliacija. Įgyvendintos pagrindinės transakcijų ir blokų tikrinimo funkcijos, Merkle medis bei Proof of Work mechanizmas.

v0.3 versijoje pritaikytos optimizacijos reikšmingai pagerino transakcijų generavimo našumą. Palyginus su v0.2 versija, transakcijų generavimas tapo maždaug 8,32 karto greitesnis.

Visi 14 automatinių testų buvo sėkmingi, todėl galima teigti, kad atliktos optimizacijos nepablogino tikrinamų programos funkcijų veikimo.
