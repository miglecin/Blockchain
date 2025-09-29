# Blockchain

## APRAŠYMAS
Šiame projekte realizuota paprasta **hash funkcija**, paremta:
- **SALT generavimu** – iš pranešimo simbolių sukuriamas 16 baitų masyvas.
- **Bubble sort** – duomenys (salt + pranešimas) rikiuojami, o kiekvieno swap metu atnaujinamas hash.

Tikslas: parodyti, kaip iš paprastų veiksmų galima sukonstruoti deterministinę, fiksuoto ilgio hash funkciją.

---
## ALGORITMO DETALĖS

### 1. Hash atnaujinimo formulė
Kiekvieno **swap** metu bubble sort’e hash atnaujinamas:
```cpp
h = (h << 3) + (h >> 2) + (a * 17 + b * 31 + j * 13)
```

kur:
- `h` – dabartinė hash reikšmė
- `(h << 3)` – hash pastumtas į kairę per 3 bitus (padaugintas iš 8)
- `(h >> 2)` – hash pastumtas į dešinę per 2 bitus (padalintas iš 4)
- `a`, `b` – sukeičiamų simbolių ASCII kodai
- `j` – jų pozicija masyve
- konstantos `17`, `31`, `13` – parinktos tam, kad padidintų rezultatų įvairovę

### Kodėl tai svarbu?
- Be bubble sort – hash beveik nekistų.
- Su bubble sort – kiekvienas swap įmaišo naują informaciją, todėl hash priklauso ne tik nuo simbolių, bet ir nuo jų tvarkos.

---

### 2. SALT generavimas
Salt sudaromas iš įvesties simbolių:

```cpp
for (size_t i = 0; i < msg.size(); i++) {
    salt[i % 16] = (salt[i % 16] + (uint8_t)msg[i] + (i * 13)) & 0xFF;
}
```

kur:
- i % 16 – pasirenkama, kurį iš 16 salt elementų atnaujinti.
- (uint8_t)msg[i] – simbolio ASCII reikšmė.
- (i * 13) – priklausomybė nuo pozicijos.
- & 0xFF – užtikrina, kad reikšmė liktų 0–255 (vienas baitas).

----
 
## PALYGINIMAS SU STANDARTINIAIS HASH (papildomai)

### Lavinos efektas

Atliktas lavinos efekto testas (10000 porų, stringo ilgis = 20), palyginant mano hash su MD5, SHA-1 ir SHA-256.

Rezultatai:

| Algoritmas   | Hex lygmuo (vidurkis) |
|--------------|------------------------|
| Mano hash    | ~85.4%                 |
| MD5          | ~93.7%                 |
| SHA-1        | ~93.7%                 |
| SHA-256      | ~93.8%                 |

**Išvados:**  
- Mano hash parodo lavinos efektą, bet jis silpnesnis ir mažiau stabilus.  
- Standartiniai hash algoritmai (MD5, SHA-1, SHA-256) pasižymi labai stipriu lavinos efektu (~94%).  idealiam lavinos efektu (~94%).  

### Kolizijų paieška ir sparta

Sugeneruojamos stringų poros (ilgio 10, 100, 500, 1000 simbolių).
- Skaičiuojami hash’ai naudojant:
  - mano hash algoritmą,
  - MD5,
  - SHA-1,
  - SHA-256.
- Jei hash’ai vienodi, registruojama kolizija.
- Skaičiavimai atlikti paralelizuojant su **OpenMP**.

### Rezultatai (100 000 porų kiekvienam ilgiui)
| Algoritmas | Ilgis=10 | Ilgis=100 | Ilgis=500 | Ilgis=1000 |
|------------|----------|-----------|-----------|------------|
| Mano hash  | 0 kolizijų | 0 kolizijų | 0 kolizijų | 0 kolizijų |
| MD5        | 0 kolizijų | 0 kolizijų | 0 kolizijų | 0 kolizijų |
| SHA-1      | 0 kolizijų | 0 kolizijų | 0 kolizijų | 0 kolizijų |
| SHA-256    | 0 kolizijų | 0 kolizijų | 0 kolizijų | 0 kolizijų |

### Laiko palyginimas
| Algoritmas | 10 simbolių | 100 simbolių | 500 simbolių | 1000 simbolių |
|------------|-------------|--------------|--------------|---------------|
| Mano hash  | 0.19 s      | 0.53 s       | 7.17 s       | 29.27 s       |
| MD5        | 0.008 s     | 0.049 s      | 0.28 s       | 0.47 s        |
| SHA-1      | 0.072 s     | 0.071 s      | 0.22 s       | 0.49 s        |
| SHA-256    | 0.040 s     | 0.081 s      | 0.24 s       | 0.55 s        |

### Išvados
- Nei mano hash, nei MD5/SHA algoritmai nerado kolizijų tarp 100 000 atsitiktinių porų (ilgiui iki 1000).  
- Mano hash yra daug **lėtesnis** (dėl `O(n²)` bubble sort), tačiau kolizijų atžvilgiu pasirodė panašiai kaip standartiniai algoritmai.


## Negrįžtamumo demonstracija (palyginimas su MD5 / SHA)
Paleidus programą su failu `input/negriztamas.txt` (turinčiu tekstą `negriztamas1`), gaunami šie hash’ai:

| Algoritmas   | Hash (hex) |
|--------------|------------|
| Custom (mano) | ea01459beb53c8fae646162629330ea571d3c8035684168e966c033052b47ee1 |
| MD5          | 0c7e574ab62718e44053d91248d57dca |
| SHA-1        | fb1f3c0c64be91457d950764b2336cbbac3a9fc9 |
| SHA-256      | 6e5c9c08484cefe0b1ebef983c090620e7fdebaa062f7504124a26773f057ff5 |

**Išvados:**
- Visi algoritmai iš tos pačios įvesties duoda visiškai skirtingus rezultatus.
- Rezultatas atrodo kaip atsitiktinis 16/20/32/64 simbolių eilutės, todėl neįmanoma atspėti pradinio teksto vien tik iš hash.
- Mano hash yra žymiai lėtesnis nei standartiniai, tačiau irgi užtikrina negrįžtamumą.

## AI pagerinimai

Naudodama AI įrankius (ChatGPT), patobulinau savo realizaciją:

1. **Patobulintas seed maišymas**  
   - Vietoj paprasto poslinkio (`(seed << 5) + (seed >> 3) + ...`) įdiegiau `mix32` funkciją  
   - Ji paremta MurmurHash3 `fmix32` algoritmu ir užtikrina gerą **avalanche efektą**  
   - Vieno bito pakeitimas įėjime paveikia daug išėjimo bitų

2. **Bitų rotacija (`rotl32`)**  
   - Įdėjau rotacijos operaciją, kad papildomai chaotiškai sumaišytų bitus  
   - Sumažina kolizijų tikimybę ir pagerina difuziją

3. **Salt integracija į seed**  
   - Salt dabar naudojamas ne tik kaip papildomi duomenys, bet ir XOR’inamas į pradinį `seed`  
   - Užtikrina, kad du vienodi failai su skirtingu salt turės visiškai skirtingą hash

4. **Struktūros tvarkymas**  
   - Sukūriau atskirus failus:  
     - `ai.h` ir `ai.cpp` – helper funkcijos (`mix32`, `rotl32`) 

## Rezultatas
- Hash funkcija dabar turi žymiai stipresnį **avalanche efektą**  
- Pagerinta apsauga nuo kolizijų  

