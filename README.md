#SUPAPRASTINTA BLOKU GRANDINE
============================

1. PROJEKTO APRASYMAS

Programa sukuria supaprastinta bloku grandine.
Joje generuojami vartotojai, transakcijos ir blokai.

Kiekvienas vartotojas turi:
- Varda ir pavarde
- Viesaji rakta
- Pradini pinigu balansa
- Dabartini pinigu balansa

Kiekviena transakcija turi:
- Siunteja
- Gaveja
- Pinigu suma
- Eiles numeri
- Transakcijos ID

2. PROJEKTO FAILAI

main.cpp          - pagrindine programos funkcija
blockchain.h      - duomenu strukturu aprasymas
hash.h            - maisos funkciju deklaracijos
hash.cpp          - maisos funkciju realizacija
users.cpp         - vartotoju generavimas
transactions.cpp  - transakciju generavimas
blocks.cpp        - bloku kurimas ir kasimas
validation.cpp    - bloku ir transakciju tikrinimas

3. PALEIDIMAS

Projektas skirtas C++ aplinkai.

Norint paleisti:
1. Atidaryti projekta Visual Studio.
2. Prideti visus .cpp ir .h failus.
3. Sukompiliuoti projekta.
4. Paleisti programa.

4. PARAMETRAI

Vartotoju skaicius: 1000
Transakciju skaicius: 10000
Maksimalus transakciju skaicius bloke: 100
Bloko sudetingumas: 3 nuliai hash pradzioje.

5. MAISOS FUNKCIJA

Naudojama savarankiskai sukurta maisos funkcija.
Jos rezultatas pateikiamas 64 sesioliktainiais simboliais.

6. PASTABA

Tai mokomasis supaprastintos bloku grandines projektas.
Maisos funkcija nera standartinis SHA-256 algoritmas.
