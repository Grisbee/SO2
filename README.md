# SO2 Jedzący Filozofowie (Projekt 1)

## Instrukcje uruchomienia

Aby skompilować i uruchomić projekt, należy wykonać następujące kroki:

1. Skompilować projekt za pomocą Makefile:
   ```
   make
   ```

2. Uruchomić program (domyślnie z 5 filozofami):
   ```
   ./Jedzacy_filozofowie
   ```

3. Można również określić inną liczbę filozofów jako argument:
   ```
   ./Jedzacy_filozofowie 7
   ```


## Problematyka

Problem jedzących filozofów jest klasycznym problemem synchronizacji w informatyce, który ilustruje wyzwania związane z alokacją zasobów i zapobieganiem zakleszczeniom (deadlock).

W problemie tym:
- N filozofów siedzi przy okrągłym stole
- Każdy filozof na zmianę myśli i je
- Pomiędzy każdą parą filozofów znajduje się jeden widelec (łącznie N widelców)
- Aby zjeść, filozof musi podnieść oba widelce sąsiadujące z nim
- Po skończeniu jedzenia, filozof odkłada oba widelce i wraca do myślenia

Główne wyzwania:
- Unikanie zakleszczenia (wszyscy filozofowie trzymają po jednym widelcu i czekają na drugi)
- Unikanie zagłodzenia (niektórzy filozofowie nigdy nie mają szansy zjeść)
- Poprawna synchronizacja dostępu do zasobów (widelców)

## Wątki i ich reprezentacja

W stworzonej implementacji:

- **Wątek główny**: Inicjalizuje symulację, tworzy wątki filozofów i czeka na ich zakończenie.
- **Wątki filozofów**: Każdy filozof jest reprezentowany przez osobny wątek, który wykonuje cyklicznie pięć cykli następujących operacji:
  1. Myślenie (losowy czas 1-3 sekund)
  2. Głodnienie (zmiana stanu)
  3. Podnoszenie widelców
  4. Jedzenie (losowy czas 1-2 sekundy)
  5. Odkładanie widelców

Każdy wątek filozofa działa niezależnie, konkurując z innymi o dostęp do wspólnych zasobów (widelców).

## Sekcje krytyczne i ich rozwiązanie

W programie występują następujące sekcje krytyczne:

1. **Dostęp do widelców**
   - **Rozwiązanie**: Używamy własnej implementacji mutex (`CustomMutex`) opartej na operacjach atomowych, gdzie każdy widelec jest chroniony przez jeden mutex.
   - **Zapobieganie zakleszczeniom**: Zastosowano rozwiązanie z asymetrią - ostatni filozof podnosi najpierw prawy widelec, a potem lewy (odwrotnie niż pozostali), co przerywa potencjalny cykl oczekiwania.

2. **Wyświetlanie komunikatów na konsolę**
   - **Rozwiązanie**: Klasa `ConsoleLock` zapewnia bezpieczny dostęp do konsoli, unikając nakładania się komunikatów z różnych wątków.
