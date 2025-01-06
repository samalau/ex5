/******************
Name: Samantha Newmark
ID: 346587629
Assignment: ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIEW 1
#define ADD 2
#define DELETE 3
#define SORT 4
#define PLAY 5
#define BACK 6

#define INITIAL_BUFFER_SIZE 4
#define EXPANSION_FACTOR 2
#define SHRINK_THRESHOLD_FACTOR 2
#define MAX_SCANF_INPUT (INITIAL_BUFFER_SIZE - 1)

#define SORT_BY_YEAR 1
#define SORT_ASCEND_STREAM 2
#define SORT_DESCEND_STREAM 3
#define SORT_ALPHABETIC 4
#define SORT_DEFAULT SORT_ALPHABETIC

#define GO_HOME (-2)
#define INVALID (-1)
#define QUIT 0
#define KILL 4

#define SMALL_DATASET_THRESHOLD 10

typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

typedef struct playlistGroup {
    Playlist **playlistAll;
    int playlistCount;
} playlistGroup;

void freeSong(Song *song);
void freePlaylist(Playlist *p);

char *getlineCustom(char **buffer, size_t *size);
char *strdupCustom(const char *s);

char* readStringInput(const char* prompt);
int readIntegerInput(const char* prompt);

int compareByYear(const void *a, const void *b);
int compareByStreamAscend(const void *a, const void *b);
int compareByStreamDescend(const void *a, const void *b);
int compareByTitle(const void *a, const void *b);

void insertionSort(Song **songs, int n, int (*comparator)(const void *, const void *));
Song **merge(Song **left, int leftCount, Song **right, int rightCount, int (*comparator)(const void *, const void *));
Song **mergeSort(Song **songs, int n, int (*comparator)(const void *, const void *));
void hybridSort(Song **songs, int n, int (*comparator)(const void *, const void *));
void songSort (Playlist *playlist);

void playSong(int songIndex, Song **songCollected);
void playAllSong(Song **songCollected, int songCount);

void songID(Song **songCollected, int songCount);
int songSelect(char action[], int songCount);

int playlistID(Playlist **playlistCollected, int playlistCount);

void delSong(Song ***songCollected, int *songCount, int songIndex);
void delPlaylist(Playlist ***playlistCollected, int *playlistCount, int playlistIndex);

void addSong(Song ***songCollected, int *songCount);
void addPlaylist(Playlist ***playlistCollected, int *playlistCount);

void playlistGoTo(Playlist *playlist);

int home(Playlist ***playlistCollected, int *playlistCount);


void freeSong(Song *song)
{
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}


void freePlaylist(Playlist *p)
{
    for (int i = 0; i < p->songsNum; i++) {
        freeSong(p->songs[i]);
    }
    free(p->songs);
    free(p->name);
    free(p);
}



char *getlineCustom(char **buffer, size_t *size) {
    if (*buffer == NULL) {
        *size = INITIAL_BUFFER_SIZE;
        *buffer = calloc(*size, sizeof(char));
        while (!*buffer) {
            *buffer = calloc(*size, sizeof(char));
        }
    }

    size_t len = 0;
    int inputRead;

    size_t temp = MAX_SCANF_INPUT;
    size_t digitCount = 0;
    do {
        digitCount++;
        temp /= 10;
    } while (temp > 0);

    size_t formatSize = sizeof(" %") + digitCount + sizeof("[^\n]");
    char *formatStr = malloc(formatSize);
    while (!formatStr) {
        formatStr = malloc(formatSize);
    }

    sprintf(formatStr, " %%%d[^\n]", MAX_SCANF_INPUT);

    do {
        if ((len + MAX_SCANF_INPUT + 1) >= *size) {
            *size *= EXPANSION_FACTOR;
            char *newBuffer = realloc(*buffer, *size);
            if (!newBuffer) {
                free(formatStr);
                return NULL;
            }
            *buffer = newBuffer;
        }

        inputRead = scanf(formatStr, (*buffer + len));

        if (inputRead == 1) {
            len += strlen(*buffer + len);
        }

        if (inputRead != 1 || len == 0) {
            (*buffer)[0] = '\0';  
            scanf("%*[^\n]");  
            scanf("%*c");  
        }

    } while (inputRead != 1 || len == 0);

    free(formatStr);
    return *buffer;
}


// char *getlineCustom(char **buffer, size_t *size)
// {
//     if (*buffer == NULL) {
//         *size = INITIAL_BUFFER_SIZE;
//         *buffer = calloc(*size, sizeof(char));
//         if (!*buffer) {
//             return NULL;
//         }
//     }

//     size_t len = 0;
//     int inputRead;
    
//     size_t temp = MAX_SCANF_INPUT;
//     size_t digitCount = 0;
//     do {
//         digitCount++;
//         temp /= 10;
//     } while (temp > 0);

//     size_t formatSize = sizeof(" %") + digitCount + sizeof("[^\n]");
//     char *formatStr = malloc(formatSize);
//     if (!formatStr) {
//         free(*buffer);
//         *buffer = NULL;
//         return NULL;
//     }

//     sprintf(formatStr, " %%%d[^\n]", MAX_SCANF_INPUT);

//     while (1) {
//         if ((len + MAX_SCANF_INPUT + 1) >= *size) {
//             *size *= EXPANSION_FACTOR;
//             char *newBuffer = realloc(*buffer, *size);
//             if (!newBuffer) {
//                 free(*buffer);
//                 free(formatStr);
//                 *buffer = NULL;
//                 return NULL;
//             }
//             *buffer = newBuffer;
//         }

//         inputRead = scanf(formatStr, *buffer + len);

//         if (inputRead == INVALID) { 
//             if (len == 0) {
//                 free(*buffer);
//                 *buffer = NULL;
//                 return NULL;
//             }
//             break;
//         }

//         if (inputRead == 1) { 
//             len += strlen(*buffer + len);
//         } else { 
//             (*buffer)[len] = '\0';
//             break;
//         }
//     }

//     free(formatStr);

//     if (inputRead != 1) {
//         scanf("%*[^\n]");
//         scanf("%*c");
//     }

//     if (*size > (len * SHRINK_THRESHOLD_FACTOR) && *size > INITIAL_BUFFER_SIZE) {
//         size_t newSize = INITIAL_BUFFER_SIZE;
//         while (newSize < (len + 1)) {
//             newSize *= EXPANSION_FACTOR;
//         }
//         char *shrunkBuffer = realloc(*buffer, newSize);
//         if (shrunkBuffer) {
//             *buffer = shrunkBuffer;
//             *size = newSize;
//         }
//     }

//     return *buffer;
// }


char *strdupCustom(const char *s)
{
    if (s == NULL) {
        return NULL;
    }

    size_t len = (strlen(s) + 1);
    char *copy = malloc(len);
    if (!copy) {
        return NULL;
    }
    memcpy(copy, s, len);
    return copy;
}


char* readStringInput(const char* prompt) {
    char *buffer = NULL;
    size_t size = 0;
    int validInput = 0;

    do {
        printf("%s", prompt);
        if (getlineCustom(&buffer, &size) != NULL && buffer[0] != '\0') {
            validInput = 1;
        } else {
            printf("Invalid option\n");
            free(buffer);
            buffer = NULL;
        }
    } while (!validInput);

    return buffer;
}

// char* readStringInput(const char* prompt)
// {
//     char *buffer = NULL;
//     size_t size = 0;
//     do {
//         printf("%s", prompt);
       
//     //    if (getlineCustom(&buffer, &size) == NULL || buffer[0] == '\0') {
//         if (getlineCustom(&buffer, &size) == NULL || strlen(buffer) == 0) {
//             free(buffer);
//             printf("Invalid option\n");
//             buffer = NULL;
//             size = 0;
//         }

//         size_t length = strcspn(buffer, "\n");
//         buffer[length] = '\0';

//         if (length == 0) {
//             free(buffer);
//             printf("Invalid option\n");
//             buffer = NULL;
//             size = 0;
//         }
//     } while (buffer == NULL);

//     return buffer;
// }


int readIntegerInput(const char* prompt)
{
    int
        value,
        input = INVALID;
    char confirm = '\0';
    do {
        printf("%s", prompt);
        input = scanf(" %d%c", &value, &confirm);
        if (input == 2 && confirm == '\n') {
            break;
        }
        printf("Invalid option\n");
        scanf("%*[^\n]");
        scanf("%*c");
    } while (1);
    return value;
}


int compareByYear(const void *a, const void *b)
{
    return ((*(Song **)a)->year) - ((*(Song **)b)->year);
}


int compareByStreamAscend(const void *a, const void *b)
{
    return ((*(Song **)a)->streams) - ((*(Song **)b)->streams);
}


int compareByStreamDescend(const void *a, const void *b)
{
    return( (*(Song **)b)->streams) - ((*(Song **)a)->streams);
}


int compareByTitle(const void *a, const void *b)
{
    return strcmp((*(Song **)a)->title, (*(Song **)b)->title);
}


void insertionSort(Song **songs, int n, int (*comparator)(const void *, const void *))
{
    for (int i = 1; i < n; i++) {
        Song *key = songs[i];
        int j = (i - 1);
        while (j >= 0 && comparator(&songs[j], &key) > 0) {
            songs[j + 1] = songs[j];
            j--;
        }
        songs[j + 1] = key;
    }
}


Song **merge(Song **left, int leftCount, Song **right, int rightCount, int (*comparator)(const void *, const void *))
{
    Song **result = (Song **)malloc((leftCount + rightCount) * sizeof(Song *));
    int i = 0, j = 0, k = 0;
    while (i < leftCount && j < rightCount) {
        if (comparator(&left[i], &right[j]) <= 0) {
            result[k++] = left[i++];
        } else {
            result[k++] = right[j++];
        }
    }
    while (i < leftCount) {
        result[k++] = left[i++];
    }
    while (j < rightCount) {
        result[k++] = right[j++];
    }
    return result;
}


Song **mergeSort(Song **songs, int n, int (*comparator)(const void *, const void *))
{
    if (n < 2) {
        return songs;
    }
    int mid = n / 2;
    Song **left = mergeSort(songs, mid, comparator);
    Song **right = mergeSort((songs + mid), (n - mid), comparator);
    Song **sorted = merge(left, mid, right, (n - mid), comparator);
    memcpy(songs, sorted, (n * sizeof(Song *)));
    free(sorted);
    return songs;
}


void hybridSort(Song **songs, int n, int (*comparator)(const void *, const void *))
{
    if (n <= SMALL_DATASET_THRESHOLD) {
        insertionSort(songs, n, comparator);
    } else {
        songs = mergeSort(songs, n, comparator);
    }
}


void songSort (Playlist *playlist)
{
    int method = readIntegerInput(
        "choose:\n"
        "1. sort by year\n"
        "2. sort by streams - ascending order\n"
        "3. sort by streams - descending order\n"
        "4. sort alphabetically\n"
    );

    if (method <= (SORT_DEFAULT - SORT_DEFAULT) || method > SORT_DEFAULT) {
        method = SORT_DEFAULT;
    }

    switch (method) {
        case SORT_BY_YEAR: {
            hybridSort(playlist->songs, playlist->songsNum, compareByYear); 
            break;
        }
        case SORT_ASCEND_STREAM: {
            hybridSort(playlist->songs, playlist->songsNum, compareByStreamAscend); 
            break;
        }
        case SORT_DESCEND_STREAM: {
            hybridSort(playlist->songs, playlist->songsNum, compareByStreamDescend); 
            break;
        }
        case SORT_ALPHABETIC: {
            hybridSort(playlist->songs, playlist->songsNum, compareByTitle); 
            break;
        }
        default: {
            hybridSort(playlist->songs, playlist->songsNum, compareByTitle); 
            break;
        }
    }
}


void playSong(int songIndex, Song **songCollected)
{
    printf("Now playing %s:\n", songCollected[songIndex]->title);
    printf("$ %s $\n", songCollected[songIndex]->lyrics);
    // songCollected[songIndex]->streams
}


void playAllSong(Song **songCollected, int songCount)
{
    for (int songIndex = 0; songIndex < songCount; songIndex++) {
        playSong(songIndex, songCollected);
        
        
    }
}


void songID(Song **songCollected, int songCount)
{
    int currentIndex = INVALID;
    int menuNumber = 0;
    for (menuNumber = 1; menuNumber <= songCount; menuNumber++) {
        currentIndex = (menuNumber - 1);
        printf("%d. Title: %s\n", menuNumber, songCollected[currentIndex]->title);
        printf("   Artist: %s\n", songCollected[currentIndex]->artist);
        printf("   Year: %d\n", songCollected[currentIndex]->year);
        printf("   Streams: %d\n", songCollected[currentIndex]->streams);
    }
}


int songSelect(char action[], int songCount)
{
    int
        chosen = INVALID,
        input = INVALID;

    do {
        printf("choose a song to %s, or 0 to quit:\n", action);
        input = scanf(" %d", &chosen);
        
        if (songCount == 0 || chosen == QUIT) {
            if (input != 1) {
                scanf("%*[^\n]");
                scanf("%*c");
            }
            return QUIT;
        }
        if (input != 1 || chosen < 0 || chosen > songCount) {
            chosen = INVALID;
            printf("Invalid option\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (chosen == INVALID);

    return chosen;
}


int playlistID(Playlist **playlistCollected, int playlistCount)
{
    int
        menuNumber = 0,
        chosen = INVALID;
    do {
        printf("Choose a playlist:\n");
        menuNumber = 0;

        for (menuNumber = 1; menuNumber <= playlistCount; menuNumber++) {
            printf("%d. %s\n", menuNumber, playlistCollected[menuNumber - 1]->name);
        }
        printf("%d. Back to main menu\n", menuNumber);

        if (scanf(" %d", &chosen) != 1 || chosen < 1 || chosen > menuNumber) {
            chosen = INVALID;
            scanf("%*[^\n]");
            scanf("%*c");
            printf("Invalid option\n");
            continue;
        }

        if (chosen == menuNumber) {
            return GO_HOME;
        }
    } while (chosen == INVALID);
    return chosen;
}


void delSong(Song ***songCollected, int *songCount, int songIndex)
{
    freeSong((*songCollected)[songIndex]);

    for (int i = songIndex; i < *songCount - 1; i++) {
        (*songCollected)[i] = (*songCollected)[i + 1];
    }

    *songCollected = (Song **)realloc(*songCollected, (*songCount - 1) * sizeof(Song *));
    (*songCount)--;
}


void delPlaylist(Playlist ***playlistCollected, int *playlistCount, int playlistIndex)
{
    freePlaylist((*playlistCollected)[playlistIndex]);

    for (int i = playlistIndex; i < *playlistCount - 1; i++) {
        (*playlistCollected)[i] = (*playlistCollected)[i + 1];
    }

    *playlistCollected = (Playlist **)realloc(*playlistCollected, (*playlistCount - 1) * sizeof(Playlist *));
    (*playlistCount)--;
}


void addSong(Song ***songCollected, int *songCount)
{
    printf("Enter song's details:\n");

    Song *newSong = (Song *)malloc(sizeof(Song));
    if (!newSong) {
        printf("Invalid option\n");
        addSong(songCollected, songCount);
    }

    // get song title
    newSong->title = readStringInput("Title:\n");

    // get artist name
    newSong->artist = readStringInput("Artist:\n");

    // get year of release
    newSong->year = readIntegerInput("Year of release:\n");

    // get lyrics
    newSong->lyrics = readStringInput("Lyrics:\n");

    // initialize stream count
    newSong->streams = 0;

    *songCollected = (Song **)realloc(*songCollected, (*songCount + 1) * sizeof(Song *));
    if (!*songCollected) {
        freeSong(newSong);
        printf("Invalid option\n");
        return;
    }
    (*songCollected)[*songCount] = newSong;
    (*songCount)++;
}

void addPlaylist(Playlist ***playlistCollected, int *playlistCount) {
    printf("Enter playlist's name:\n");
    char *playlistName = NULL;
    size_t size = 0;

    getlineCustom(&playlistName, &size);

    if (!playlistName || strlen(playlistName) == 0) {
        free(playlistName);
        printf("Invalid option\n");
        return;
    }

    Playlist *newPlaylist = malloc(sizeof(Playlist));
    if (!newPlaylist) {
        free(playlistName);
        printf("Invalid option\n");
        return;
    }

    newPlaylist->name = strdupCustom(playlistName);
    free(playlistName);

    if (!newPlaylist->name) {
        free(newPlaylist);
        printf("Invalid option\n");
        return;
    }

    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;

    Playlist **tempCollection = realloc(*playlistCollected, (*playlistCount + 1) * sizeof(Playlist *));
    if (!tempCollection) {
        free(newPlaylist->name);
        free(newPlaylist);
        printf("Invalid option\n");
        return;
    }

    *playlistCollected = tempCollection;
    (*playlistCollected)[*playlistCount] = newPlaylist;
    (*playlistCount)++;
}


void playlistGoTo(Playlist *playlist)
{
    int chosen = BACK;
    int option = INVALID;
    int printMenu = 1;
    Playlist *playlistCurrent = playlist;
    printf("playlist %s:\n", playlistCurrent->name);

    do {
        if (printMenu) {
            printf(
                "%d. Show Playlist\n"
                "%d. Add Song\n"
                "%d. Delete Song\n"
                "%d. Sort\n"
                "%d. Play All\n"
                "%d. Back\n",

                VIEW,
                ADD,
                DELETE,
                SORT,
                PLAY,
                BACK
            );
        }
        option = scanf(" %d", &chosen);
        if (chosen == BACK) {
            return;
        }

        if (option != 1) {
            chosen = INVALID;
            scanf("%*[^\n]");
            scanf("%*c");
            printMenu = 1;
        } else {
            printMenu = 0;
        }

        switch (chosen) {
            case VIEW: {
                songID(playlistCurrent->songs, playlistCurrent->songsNum);
                while ((chosen = songSelect("play", playlistCurrent->songsNum)) != INVALID && chosen != QUIT) {
                    int chosenIndex = chosen - 1;
                    playSong(chosenIndex, playlistCurrent->songs);
                }
                break;
            }
            case ADD: {
                addSong(&playlistCurrent->songs, &playlistCurrent->songsNum);
                break;
            }
            case DELETE: {
                songID(playlistCurrent->songs, playlistCurrent->songsNum);
                if ((chosen = songSelect("delete", playlistCurrent->songsNum)) != INVALID && chosen != QUIT) {
                    int chosenIndex = chosen - 1;
                    delSong(&playlistCurrent->songs, &playlistCurrent->songsNum, chosenIndex);
                }
                break;
            }
            case SORT: {
            songSort(playlistCurrent);
            printf("sorted\n");
            break;
            }
            case PLAY: {
                playAllSong(playlistCurrent->songs, playlistCurrent->songsNum);
                break;
            }
            default: {
                printf("Invalid option\n");
                break;
            }
        }
    printMenu = 1;
    } while (chosen != BACK);
}


int home(Playlist ***playlistCollected, int *playlistCount)
{
    int
        chosen = INVALID,
        option = QUIT,
        printMenu = 1;

    do {
        if (printMenu) {
            printf(
                "Please Choose:\n"
                "%d. Watch playlists\n"
                "%d. Add playlist\n"
                "%d. Remove playlist\n"
                "%d. exit\n",

                VIEW,
                ADD,
                DELETE,
                KILL
            );
        }

        option = scanf(" %d", &chosen);

        if (option == EOF || chosen == KILL) {
            return KILL;
        }

        if (option != 1) {
            chosen = INVALID;
            printMenu = 1;
        } else {
            printMenu = 0;
        }

        scanf("%*[^\n]");
        scanf("%*c");

        switch (chosen) {
            case VIEW: {
                while ((chosen = playlistID(*playlistCollected, *playlistCount)) != INVALID) {
                    if (chosen == GO_HOME) { 
                        break;
                    }
                    int chosenIndex = (chosen - 1);
                    playlistGoTo((*playlistCollected)[chosenIndex]);
                }
                break;
            }
            case ADD: {
                addPlaylist(playlistCollected, playlistCount);
                break;
            }
            case DELETE: {
                if ((chosen = playlistID(*playlistCollected, *playlistCount)) != INVALID && chosen != GO_HOME) {
                    int chosenIndex = (chosen - 1);
                    delPlaylist(playlistCollected, playlistCount, chosenIndex);
                }
            break;
            }
            default: {
                printf("Invalid option\n");
                break;
            }
        }
        printMenu = 1;
    } while (chosen != KILL);
    return chosen;
}


int main()
{
    Playlist **playlistCollected = NULL;
    int playlistCount = 0;
    while ((home(&playlistCollected, &playlistCount)) != KILL);
    for (int i = 0; i < playlistCount; i++) {
        freePlaylist(playlistCollected[i]);
    }
    free(playlistCollected);
    printf("Goodbye!\n");
    return 0;
}

// TODO: EOF
// TODO: comments
// TODO: streams
