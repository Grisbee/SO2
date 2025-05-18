# Aplikacja Czatu (Projekt 2)

Prosta aplikacja czatu implementująca własną synchronizację wątków. Projekt składa się z serwera obsługującego wielu klientów jednocześnie za pomocą wątków i własnych mechanizmów synchronizacji.

## Opis problemu

Projekt odpowiada za stworzenie wielowątkowego serwera czatu spełniającego następujące wymagania:

1. Serwer musi tworzyć osobny wątek dla każdego połączenia klienta
2. Wiadomości muszą być właściwie synchronizowane między klientami
3. Należy zaimplementować własne mechanizmy synchronizacji (bez wbudowanych blokad)
4. Klienci powinni móc widzieć i wysyłać wiadomości w czacie

## Instrukcje uruchomienia

### Linux/macOS
1. Nadaj uprawnienia wykonawcze: chmod +x run.sh
2. Uruchom skrypt: `./run.sh`

## Wątki

### Wątki serwera

1. **Wątek główny**:
   - Nasłuchuje przychodzących połączeń klientów
   - Tworzy nowy wątek dla każdego podłączonego klienta

2. **Wątki obsługi klientów**:
   - Jeden wątek na każdego podłączonego klienta
   - Odpowiedzialny za odbieranie wiadomości, dodawanie ich do historii i rozsyłanie do wszystkich klientów

### Wątki klienta

1. **Wątek główny**:
   - Obsługuje dane wprowadzane przez użytkownika
   - Wysyła wiadomości do serwera

2. **Wątek odbierający**:
   - Odbiera wiadomości z serwera
   - Wyświetla je użytkownikowi

## Sekcje krytyczne i ich rozwiązania

### 1. Lista klientów 

**Operacje krytyczne**:
- Dodawanie nowego klienta do listy
- Usuwanie klienta z listy
- Iteracja przez listę w celu rozesłania wiadomości

**Rozwiązanie**:
- Własna implementacja blokady (`self.clients_lock`) chroni dostęp do listy klientów
- Każda operacja na liście musi najpierw uzyskać tę blokadę

### 2. Historia wiadomości 

**Operacje krytyczne**:
- Dodawanie nowych wiadomości do historii
- Odczytywanie wiadomości z historii

**Rozwiązanie**:
- Własna implementacja blokady (`self.messages_lock`) chroni dostęp do historii wiadomości
- Każda operacja na historii wiadomości musi najpierw uzyskać tę blokadę

Zastosowanie własnych blokad zapewnia, że tylko jeden wątek może modyfikować współdzielone zasoby w danym momencie, co zapobiega problemom synchronizacji.