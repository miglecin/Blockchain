#!/bin/bash
INPUT_DIR="$HOME/Desktop/blockchain/hash/input"
OUT="$HOME/Desktop/blockchain/hash/results.txt"

# išvalom seną rezultatą
: > "$OUT"

for f in "$INPUT_DIR"/*; do
  echo "=== $(basename "$f") ===" >> "$OUT"
  ./hash_program "$f" >> "$OUT" 2>&1
  echo "" >> "$OUT"
done

echo "Atlikta. Rezultatai: $OUT"



