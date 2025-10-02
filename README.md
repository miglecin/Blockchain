# Blockchain

# Hash algoritmų palyginimas (studentų darbai)

Šiame skyriuje pateikiami visų studentų hash funkcijų rezultatai.  
Palyginti trys aspektai: **efektyvumas**, **atsparumas kolizijoms** ir **lavinos efektas**.

---

## Efektyvumas

Matuotas skaičiavimo laikas (sekundėmis), priklausomai nuo eilučių kiekio.

| Eilučių kiekis            | Augustės ir Astridos | Benedikto ir Tauro | Eligijaus ir Povilo | Dominyko ir Marijaus | **Miglės** | Monikos | Justes | Nikitos | Andriaus ir Valentino | Nerijaus ir Igno | Nikos ir Nastios |
| :------------------------ | :------------------: | :----------------: | :-----------------: | :------------------: | :--------: | :-----: | :-----: | :-----: | :-------------------: | :---------------: | :---------------: |
| 1 eilutė                  | 0.000003 | 0.000051 | 0.000042 | 0.001912 | **0.000072** | 0.000691 | 0.000003 | 0.000022 | 0.000000 | 0.000172 | 0.000000 |
| 2 eilutės                 | 0.000002 | 0.000044 | 0.000049 | 0.001764 | **0.000152** | 0.000672 | 0.000003 | 0.000017 | 0.000000 | 0.000187 | 0.000000 |
| 4 eilutės                 | 0.000003 | 0.000052 | 0.000054 | 0.001569 | **0.000397** | 0.001482 | 0.000002 | 0.000019 | 0.000000 | 0.000308 | 0.000000 |
| 8 eilutės                 | 0.000003 | 0.000068 | 0.000063 | 0.001557 | **0.001594** | 0.003564 | 0.000004 | 0.000024 | 0.000002 | 0.000564 | 0.001257 |
| 16 eilučių                | 0.000006 | 0.000081 | 0.000085 | 0.001572 | **0.002066** | 0.005757 | 0.000015 | 0.000065 | 0.000003 | 0.001078 | 0.000000 |
| 32 eilutės                | 0.000009 | 0.000094 | 0.000107 | 0.001690 | **0.004557** | 0.018003 | 0.000029 | 0.000081 | 0.000005 | 0.002669 | 0.001002 |
| 64 eilutės                | 0.000027 | 0.000127 | 0.000426 | 0.001922 | **0.006891** | 0.039891 | 0.000056 | 0.000163 | 0.000022 | 0.006254 | 0.002071 |
| 128 eilutės               | 0.000041 | 0.000341 | 0.000877 | 0.003004 | **0.008034** | 0.074522 | 0.000088 | 0.000399 | 0.000037 | 0.008936 | 0.004241 |
| 256 eilutės               | 0.000198 | 0.000602 | 0.001891 | 0.003646 | **0.010019** | 0.188049 | 0.000397 | 0.000995 | 0.000106 | 0.020262 | 0.010296 |
| 512 eilučių               | 0.000257 | 0.001296 | 0.005545 | 0.006413 | **0.012872** | 0.300145 | 0.000438 | 0.002834 | 0.000247 | 0.055744 | 0.023281 |
| Visas failas (789 eilučių)| 0.000378 | 0.002236 | 0.006541 | 0.007956 | **0.018226** | 0.334500 | 0.000512 | 0.004237 | 0.000545 | 0.087845 | 0.063517 |

**Išvada:** visi algoritmai veikia greitai, tačiau mano hash („Miglės“) yra lėtesnis dėl **bubble sort (O(n²))**.

---

## Atsparumas kolizijoms

Sugeneruota po 100 000 atsitiktinių porų kiekvienam ilgiui (10, 100, 500, 1000).  

| Stringo ilgis | Augustė & Astrida | Benediktas & Tauras | Eligijus & Povilas | Dominykas & Marijus | **Miglė** | Monika | Justė | Nikita | Andrius & Valentinas | Nerijus & Ignas | Nika & Nastia |
|---------------|:-----------------:|:-------------------:|:------------------:|:-------------------:|:---------:|:------:|:-----:|:------:|:-------------------:|:----------------:|:-------------:|
| 10            | 0 | 0 | 0 | 0 | **0** | 0 | 0 | 0 | 0 | 0 | 0 |
| 100           | 0 | 0 | 0 | 0 | **0** | 0 | 0 | 0 | 0 | 0 | 0 |
| 500           | 0 | 0 | 0 | 0 | **0** | 0 | 0 | 0 | 0 | 0 | 0 |
| 1000          | 0 | 0 | 0 | 0 | **0** | 0 | 0 | 0 | 0 | 0 | 0 |

**Išvada:** kolizijų nerasta nei pas mane, nei pas kitus.

---

## Lavinos efektas

Matavome, kiek skiriasi hash’ai, jei pakeičiame tik vieną simbolį.  
Rezultatai pateikti bitų ir hex lygmenyje.

### Bitų lygmuo

| Hash                       | Max | Min | Vidurkis |
|----------------------------|:---:|:---:|:--------:|
| Augustės ir Astridos Hash  | 84.69% | 0.00% | 49.56% |
| Benedikto ir Tauro Hash    | 48.47% | 0.84% | 35.48% |
| Eligijaus ir Povilo Hash   | 45.55% | 1.45% | 35.63% |
| Dominyko ir Marijaus Hash  | 51.11% | 42.9% | 50.65% |
| **Miglės Hash**            | 62.26% | 0.55% | **45.85%** |
| Monikos Hash               | 33.59% | 11.32% | 21.42% |
| Justės Hash                | 83.23% | 97.65% | 91.82% |
| Nikitos Hash               | 89.62% | 34.19% | 53.00% |
| Andriaus & Valentino Hash  | 65.14% | 9.33% | 36.88% |
| Nerijaus & Igno Hash       | 63.41% | 22.88% | 49.84% |
| Nikos & Nastios Hash       | 67.97% | 31.25% | 50.00% |

### Hex lygmuo

| Hash                       | Max | Min | Vidurkis |
|----------------------------|:---:|:---:|:--------:|
| Augustės ir Astridos Hash  | 100% | 2.54% | 94.15% |
| Benedikto ir Tauro Hash    | 100% | 1.15% | 82.13% |
| Eligijaus ir Povilo Hash   | 96%  | 1.54% | 75.73% |
| Dominyko ir Marijaus Hash  | 97.88% | 19.04% | 93.71% |
| **Miglės Hash**            | 100% | 0.31% | **85.92%** |
| Monikos Hash               | 100% | 50% | 81.45% |
| Justės Hash                | 93.18% | 76.61% | 83.82% |
| Nikitos Hash               | 100% | 0.23% | 86.13% |
| Andriaus & Valentino Hash  | 64.45% | 48.21% | 92.74% |
| Nerijaus & Igno Hash       | 100% | 45% | 93.24% |
| Nikos & Nastios Hash       | 100% | 68.75% | 93.77% |

**Išvada:** mano hash vidutiniškai turi ~46% bitų skirtumą ir ~86% hex skirtumą – tai reiškia, kad lavinos efektas veikia, nors pasitaiko ir išimčių (kartais skirtumas 0%).

---

## Bendros išvados

- **Efektyvumas:** mano hash lėtesnis už kitus dėl bubble sort algoritmo.  
- **Kolizijos:** kolizijų nerasta nei mano, nei kitų hash’uose.  
- **Lavinos efektas:** mano rezultatai panašūs į klasikinius hash algoritmus – pakeitus vieną simbolį, pasikeičia apie pusė bitų.  

## Galutinė santrauka (Miglės Hash)

- **Efektyvumas:** 7/11 → vidutinė vieta, lėtesnis už daugumą, bet ne pats lėčiausias.  
- **Kolizijos:** 1/1 → tokia pati kaip pas kitus, kolizijų nerasta.  
- **Lavinos efektas (bitai):** 7/11 → vidutinė vieta, ~46 %, arti idealaus 50 %.  
- **Lavinos efektas (hex):** 6/11 → virš vidurkio, panašus į gerus rezultatus.  