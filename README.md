# Blockchain
## PRADINĖ VERSIJA (hash + salt)

Ši programa įgyvendina **hash algoritmą**, kuris naudoja:
- **Bubble sort** rikiavimą,  
- hash reikšmės atnaujinimą kiekvieno swap'o metu,  
- papildomą **salt** (16 baitų),  
- 256 bitų (8 × 32 bit) hash išvestį.

## Kaip veikia algoritmas
1. Sukuriamas **salt** iš pradinės žinutės (`make_salt`) – kiekvienas simbolis įmaišomas į 16 baitų vektorių.  
2. Salt prijungiamas prie pradinės žinutės.  
3. Duomenys rikiuojami **bubble sort** algoritmu. Kiekvieną kartą, kai sukeičiam du simbolius:
   - Atnaujinamas vienas iš 8 hash blokų (32 bitų),  
   - Hash atnaujinime naudojami poslinkiai (`<< 5`, `>> 3`) ir ASCII kodai su konstantomis `17`, `31`, `13`.  
4. Pabaigoje turime 256 bitų hash reikšmę, atvaizduojamą **hex formatu**.


Kompiliavimas:
```bash
g++ -std=c++17 -O2 main.cpp -o hash_program