# Blockchain

## Aprašymas
Šiame projekte realizuota paprasta **hash funkcija**, paremta:
- **SALT generavimu** – iš pranešimo simbolių sukuriamas 16 baitų masyvas.
- **Bubble sort** – duomenys (salt + pranešimas) rikiuojami, o kiekvieno swap metu atnaujinamas hash.

Tikslas: parodyti, kaip iš paprastų veiksmų galima sukonstruoti deterministinę, fiksuoto ilgio hash funkciją.

---
## Algoritmo detalės

### 1. Hash atnaujinimo formulė
Kiekvieno **swap** metu bubble sort’e hash atnaujinamas:
```cpp
h = (h << 3) + (h >> 2) + (a * 17 + b * 31 + j * 13)
```

kur:
- `h` – dabartinė hash reikšmė
- `(h << 3)` – hash pastumtas į kairę per 3 bitus (padaugintas iš 8)
- `(h >> 2)` – hash pastumtas į dešinę per 2 bitus (padalintas iš 4)
- `a`, `b` – sukeitamų simbolių ASCII kodai
- `j` – jų pozicija masyve
- konstantos `17`, `31`, `13` – parinktos tam, kad padidintų rezultatų įvairovę

### Kodėl tai svarbu?
- Be bubble sort – hash beveik nekistų.
- Su bubble sort – kiekvienas swap „įmaišo“ naują informaciją, todėl hash priklauso ne tik nuo simbolių, bet ir nuo jų tvarkos.

---

### 2. SALT generavimas
Salt sudaromas iš pranešimo simbolių:

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

---

## Pseudo kodai

> **Salt generavimas**
>
> ```text
> FUNKCIJA MAKE_SALT(MSG):
>     SALT = {0,0,...,0}
>
>     CIKLAS i nuo 0 iki MSG_ilgis-1:
>         idx = i MOD 16
>         SALT[idx] = ( SALT[idx] + ASCII(MSG[i]) + i*13 ) MOD 256
>
>     GRĄŽINTI SALT
> ```

---

> **Bubble sort su hash atnaujinimu**
>
> ```text
> FUNKCIJA BUBBLE_SORT_AND_HASH(DATA, STATE[8]):
>     n = DATA_ilgis
>
>     CIKLAS i nuo 0 iki n-2:
>         CIKLAS j nuo 0 iki n-2-i:
>             JEI DATA[j] > DATA[j+1]:
>                 a = ASCII(DATA[j])
>                 b = ASCII(DATA[j+1])
>                 idx = j MOD 8
>
>                 STATE[idx] = (STATE[idx] << 5)
>                              + (STATE[idx] >> 3)
>                              + (a*17 + b*31 + j*13)
>
>                 sukeisti DATA[j] ir DATA[j+1]
>
>     GRĄŽINTI STATE
> ```

---

> **Pagrindinė programa**
>
> ```text
> MSG <- perskaityti visą tekstą iš failo
> SALT <- MAKE_SALT(MSG)
>
> DATA <- SALT || MSG   // pirmiausia salt, po to pranešimas
>
> STATE[0..7] <- inicializuoti pagal MSG
>                (ilgis, pirmas simbolis, paskutinis simbolis, konstantos)
>
> HASH <- BUBBLE_SORT_AND_HASH(DATA, STATE)
> ```