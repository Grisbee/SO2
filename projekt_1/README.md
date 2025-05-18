# Problem Jedzących Filozofów

Implementacja problemu "Jedzący Filozofowie" z własną implementacją mechanizmów synchronizacji.

## Opis problemu

Projekt rozwiązuje problem synchronizacji znany jako "Jedzący Filozofowie", w którym:

1. Grupa filozofów siedzi przy okrągłym stole
2. Każdy filozof na zmianę myśli i je
3. Do jedzenia potrzebne są dwa widelce znajdujące się po bokach talerza
4. Każdy filozof musi podnieść oba widelce, aby zjeść
5. Problem polega na uniknięciu zakleszczenia (wszyscy filozofowie trzymają po jednym widelcu)


Domyślnie program uruchamia symulację z 5 filozofami, ale można podać inną liczbę jako argument linii komend.


## Instrukcje uruchomienia

1. Nadanie uprawnień wykonywania : chmod +x Jedzacy_filozofowie
2. Uruchomienie: ./Jedzacy_filozofowie [liczba_filozofow]

## Wątki

### Wątki filozofów

Każdy filozof jest reprezentowany przez osobny wątek:
- Wątki są tworzone w funkcji `main()`
- Każdy wątek wykonuje metodę `dine()` swojego obiektu Philosopher
- Filozofowie przechodzą przez cykl: myślenie → zgłodzenie → podniesienie widelców → jedzenie → odłożenie widelców

### Wątek główny

Wątek główny:
- Tworzy obiekty filozofów
- Inicjalizuje i uruchamia wątki dla każdego filozofa
- Czeka na zakończenie wszystkich wątków

## Sekcje krytyczne i ich rozwiązania

### 1. Dostęp do widelców

**Operacje krytyczne**:
- Podnoszenie widelców przez filozofów
- Odkładanie widelców przez filozofów

**Rozwiązanie**:
- Każdy widelec jest reprezentowany przez własny obiekt `CustomMutex`
- Implementacja własnego muteksu przy użyciu operacji atomowych
- Rozwiązanie problemu zakleszczenia poprzez asymetrię: ostatni filozof podnosi widelce w odwrotnej kolejności niż pozostali

### 2. Dostęp do konsoli

**Operacje krytyczne**:
- Wyświetlanie komunikatów o stanie filozofów

**Rozwiązanie**:
- Klasa `ConsoleLock` chroni dostęp do standardowego wyjścia
- Zapewnia, że komunikaty nie będą się przeplatać podczas wyświetlania
