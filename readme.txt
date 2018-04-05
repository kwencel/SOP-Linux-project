Opis klas:
SequentialFile  - Reprezentuje plik o dostępie sekwencyjnym (łącze nazwane).
SeekableFile    - Reprezentuje plik o dostępie bezpośrednim (plik zwykły). Dziedziczy po SequentialFile.
Process         - Reprezentuje proces.
SharedMemory    - Reprezentuje zbiór segmentów pamięci współdzielonej.
Semaphore       - Reprezentuje pojedynczy zbiór (set) semaforów.
MessageQueue    - Reprezentuje pojedynczą kolejkę komunikatów.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Na początek krótki opis koncepcji projektu:

Każda z klas posiada tylko jedną swoją instancję (obiekt) w programie.
Istnieją one niezależnie od tego, czy istnieją elementy, które reprezentują.

Każdy obiekt może przechowywać informację o jednym elemencie systemu operacyjnego, który reprezentuje,
To znaczy, że na przykład SeekableFile/Process reprezentuje w danym momencie maksymalnie jeden plik/proces,
może także nie reprezentować żadnego, lecz nie więcej niż jeden.

Wyjątkiem od tej zasady jest SharedMemory, którego instancja pozwala na reprezentację dowolnej liczby
segmentów pamięci współdzielonej, pozwala w prosty sposób nimi zarządzać, a każda operacja na tym obiekcie
poprzedzona jest pytaniem, na którym ze współdzielonych segmentów użytkownik chce przeprowadzić operację.

Na samym początku obiekty te nie reprezentują niczego, należy do nich "załadować" element, który mają reprezentować.
Jest to możliwe poprzez swtorzenie nowego elementu (funkcja Create) bądź załadowanie zasobu już istniejącego w systemie operacyjnym.
Dla przykładu - do klasy Process można podpiąć istniejący proces (funkcja "Attach"), bądź stworzyć nowy (potomek) funkcją "Create",
który w następnie będzie można podpiąć funkcją "Attach", bądź zostanie podpięty automatycznie (zależnie od klasy).
Z racji tego, że każdy (poza SharedMemory) obiekt może przechowywać informację tylko o jednym elemencie, próba podpięcia
bądź stworzenia kolejnego procesu zakończy się fiaskiem, a użytkownik zostanie poinformowany o konieczności zabicia bądź zakończnia
załadowanego procesu.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Program po uruchomieniu pozwala użytkownikowi na interakcję poprzez klawisze numeryczne.

Główne menu programu dzieli się na 3 części:
1) File operations
2) Process operations
3) IPC operations

----------------------------------------- File operations --------------------------------------------------------------------------------------------------------------
Na początku należy wybrać na jakim typie pliku chcemy operować, o dotępie bezpośrednim czy sekwencyjnym.
Wybór pliku sekwencyjnego uniemożliwia użycie funkcji "7: Seek file" oraz funkcja "1: Create file" tworzy łącze nazwane,
a nie plik zwykły jak ma to miejsce w przypadku pliku o dostępie bezpośrednim.

1) Sequential access
1: Create file          - Tworzy łącze nazwane o danej nazwie.
2: Delete file          - Usuwa plik o danej nazwie.
3: Open file            - Otwiera (podpina, ładuje) plik o danej nazwie i w danym trybie.
4: Close file           - Zamyka plik.
5: Read from file       - Czyta n bajtów z załadowanego pliku.
6: Write to file        - Zapisuje podany tekst do załadowanego pliku.

2) Direct  access
Ta sama funkcjonalność co dla plików o dostępie sekwencyjnym poszerzona o możliwość
przesuwania wskaźnika pozycji "7) Seek file" oraz funkcja "1. Create file" tworzy plik zwykły, a nie łącze nazwane

----------------------------------------- Process operations -----------------------------------------------------------------------------------------------------------
1: Attach to process                         - Podpina istniejący w systemie proces o podanym PID. Funkcja zwróci błąd przy próbie podpięcia procesu należącego
                                               do innego użytkownika (chyba że program zostanie uruchomiony jako root).
2: Create a child process                    - Tworzy proces potomka, a następnie podmienia jego obraz na program o podanej nazwie i podpina go).
3: Send a signal to process                  - Wysyła sygnał o podanym numerze do podpiętego procesu.
4: Change process priority                   - Zmienia priorytet podpiętego procesu. Można tylko zmniejszać priorytet (zwiększać wartość nice) własnych procesów, chyba że
                                               uruchamia się program jako root, wtedy można zmieniać priorytety dowolnych procesów, a także zwiększać im priorytety.
5: Wait for process                          - Czeka na zakończenie procesu potomka. Jeżeli podpięty proces nie jest potomkiem to funkcja zwróci błąd i wypisze odpowiedni
                                               komunikat.
6: List all your running processes           - Tworzy potomka i podmienia jego obraz na program "ps", który wyświetla listę procesów należących do użytkownika
                                               uruchamiającego Program.
7: Show PID of currently assigned process    - Pokazuje PID aktualnie podpiętego procesu.

Zarówno funkcja "Attach" jak i "Create" w przypadku, gdy ostatnio podpiętym procesem był potomek Programu, a następnie został zakończony i nie została
na nim wywołana operacja "Wait for process" (zatem proces jest zombie) wywołuje funkcję "Wait for process" na tym procesie przed właściwym dla swojej
specyfikacji działaniem. Dzieje się tak dlatego, że jeżeli wywołana została funkcja "Attach" bądź "Create" oznacza to, że użytkownik zażyczył sobie podpiąć
nowy proces, zatem siłą rzeczy PID starego procesu zostanie zapomniany. Zatem nie będzie już możliwości wykonania na starym procesie (potomku) operacji
"Wait for process", zatem proces pozostanie zombie do końca działania Programu. Powyższy zabieg zapobiega sytuacji w której w systemie gromadzą się
procesy Zombie, na które nikt nie zaczekał.

W sytuacji, gdy aktualnie podpięty proces jest potomkiem (została użyta funkcja "Create a child process" i użytkownik spróbuje podpiać istniejący proces bądź
stworzyć nowego potmka zostanie poinformowany o tym, że posiada już potomka, który nie został zakończony oraz zostanie zapytany czy chciałby wysłać do potmka
sygnał SIGTERM lub SIGKILL.

Natomiast, gdy aktualnie podpięty proces nie jest potomkiem (została użyta funkcja "Attach to process") i użytkownik spróbuje podpiać kolejny istniejący proces
bądź stworzyć potmka, PID aktualnego procesu zostanie zapomniany. Mozna go oczywiście później znowu podpiąć funkcją "Attach to process"

--------------------------------------------- IPC operations -----------------------------------------------------------------------------------------------------------
1) Shared memory

1: Create shared memory segment              - Tworzy segment pamięci współdzielonej i umieszcza informacje o nim w wektorze allSegments.
2: Delete shared memory segment              - Usuwa wybrany z listy segment pamięci współdzielonej i usuwa informacje o nim z wektora allSegments oraz attachedSegments,
                                               jeśli był podpięty. Zezwala także na podanie ID segmentu, którego nie ma na liście.
3: Attach shared memory segment              - Podpina wybrany z listy segment pamięci współdzielonej. Zezwala także na podanie ID segmentu, którego nie ma na liście.
4: Detach shared memory segment              - Odpina wybrany z listy segment pamięci współdzielonej.
5: Read from shared memory segment           - Prosi użytkownika o wybór segmentu z listy aktualnie podpiętych segmentów i odczytuje z niego podaną przez użytkownika
                                               liczbę bajtów.
6: Write to shared memory segment            - Prosi użytkownika o wybór segmentu z listy aktualnie podpiętych segmentów i zapisuje do niego podaną przez użytkownika
                                               liczbę bajtów.
7: List all created and attached segments    - Wyświetla zawartość wektora z informacjami o utworzonych i podpiętych segmentach.

2) Semaphores

1: Create a semaphore set                    - Tworzy zbiór (set) semaforów o podanej przez użytkownika liczbie semaforów.
2: Attach a previously created semaphore set - Podpina zbiór semaforów o podanym przez użytkownika ID oraz liczbie semaforów w zbiorze.
3: Remove a semaphore set                    - Usuwa aktualnie podpięty zbiór semaforów.
4: Perform P() (lock) operations             - Zmniejsza wartość m-tego semafora o n ('m' i 'n' podawane przez użytkownika) i wstrzymuje proces Programu,
                                               jeśli w wyniku wykonania tej operacji wartość semaphore spadła poniżej zera.
5: Perform V() (unlock) operation            - Zwiększa wartość m-tego semafora o n ('m' i 'n' podawane przez użytkownika) i wznawia proces Programu,
                                               jeśli w wyniku wykonania tej operacji wartość semaphore stała się liczbą nieujemną.
6: Set value of semaphore                    - Ustawia wartość m-tego semafora na wartość n ('m' i 'n' podawane przez użytkownika).
7: Show ID of the semaphore set              - Pokazuje ID aktualnie podpiętego setu semaforów.
8: Show number of the semaphores in set      - Pokazuje ilość semaforów w aktualnie podpiętym zbiorze.

3) Message queues

1: Create a message queue                     - Tworzy kolejkę komunikatów.
2: Attach a previously created message queue  - Podpina kolejkę komunikatów o danym ID.
3: Remove a message queue                     - Usuwa aktualnie podpiętą kolejkę komunnikatów.
4: Receive a message                          - Odbiera wiadomość n-tego typu z aktualnie podpiętej kolejki komunikatów ('n' podawane przez użytkownika).
5: Send a message                             - Wysyła wiadomość n-tego typu o podanej treści do aktualnie podpiętej kolejki komunikatów ('n' oraz treść
                                                komunikatu podawana przez użytkownika).
6: Get ID of the message queue                - Wyświetla ID aktualnie podpiętej kolejki komunikatów.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------

