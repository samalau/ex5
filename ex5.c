/******************
Name: Samantha Newmark
ID: 346587629
Assignment: ex5
*******************/

// TODO: EOF
// TODO: comments
// TODO: streams
// TODO: enumerate macros


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
#define MAX_SCANF_INPUT_STR "4"

#define SORT_BY_YEAR 1
#define SORT_ASCEND_STREAM 2
#define SORT_DESCEND_STREAM 3
#define SORT_ALPHABETIC 4
#define SORT_DEFAULT SORT_ALPHABETIC

#define GO_HOME (-2)
#define INVALID (-1)
#define QUIT 0
#define VALID 1
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
    if (song == NULL) {
        return;
    }
    free(song->title);
    song->title = NULL;

    free(song->artist);
    song->artist = NULL;

    free(song->lyrics);
    song->lyrics = NULL;

    free(song);
    song = NULL;
}


void freePlaylist(Playlist *p)
{
    if (p == NULL) {
        return;
    }
    for (int i = 0; i < p->songsNum; i++) {
        if (p->songs[i] != NULL) {
            freeSong(p->songs[i]);
            p->songs[i] = NULL;
        }
    }
    free(p->songs);
    p->songs = NULL;

    free(p->name);
    p->name = NULL;

    free(p);
    p = NULL;
}


char *getlineCustom(char **buffer, size_t *size) {
    if (*buffer == NULL) {
        *size = INITIAL_BUFFER_SIZE;
        *buffer = calloc(*size, sizeof(char));
        if (*buffer == NULL) {
            printf("Invalid option\n");
            return NULL;
        }
    }

    size_t len = 0;
    int inputRead = 0;
    char formatStr[] = " %[^\n]";

    do {
        // check if buffer needs expansion
        if ((len + 1) >= *size) {
            *size *= EXPANSION_FACTOR;
            char *temp = realloc(*buffer, *size);
            if (temp == NULL) {
                free(*buffer);
                *buffer = NULL;
                printf("Invalid option\n");
                return NULL;
            }
            // while (!newBuffer) {
            //     newBuffer = realloc(*buffer, *size);
            // }
            *buffer = temp;
        }

        // read input dynamically without length restriction
        inputRead = scanf(formatStr, (*buffer + len));

        if (inputRead == 1) {
            len += strlen(*buffer + len);
        }

        // reset buffer content if invalid input or empty buffer
        if (inputRead != 1 || len == 0) {
            free(*buffer);
            *buffer = NULL;
            printf("Invalid input\n");
            return NULL;
        }

        // shrink buffer if usage drops below the threshold
        if (len < (*size) / SHRINK_THRESHOLD_FACTOR && *size > INITIAL_BUFFER_SIZE) {
            *size /= EXPANSION_FACTOR;
            char *temp = realloc(*buffer, *size);
            if (temp == NULL) {
                free(*buffer);
                *buffer = NULL;
                printf("Invalid option\n");
                return NULL;
            }
            *buffer = temp;
        }
    } while (inputRead != 1 || len == 0);
    return *buffer;
}


char *strdupCustom(const char *s)
{
    if (s == NULL) {
        return NULL;
    }
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy == NULL) {
        printf("Invalid option\n");
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
            free(buffer);
            buffer = NULL;
            printf("Invalid option\n");
        }
    } while (!validInput);

    return buffer;
}


int readIntegerInput(const char* prompt)
{
    int
        value = INVALID;
        // input = INVALID;
    char confirm = '\0';
    do {
        printf("%s", prompt);
        if (scanf(" %d%c", &value, &confirm) == 2 && confirm == '\n') {
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
    int yearA = (*(Song **)a)->year;
    int yearB = (*(Song **)b)->year;
    return (yearA > yearB) - (yearA < yearB);
}


int compareByStreamAscend(const void *a, const void *b)
{
    int streamA = (*(Song **)a)->streams;
    int streamB = (*(Song **)b)->streams;
    return (streamA > streamB) - (streamA < streamB);
}


int compareByStreamDescend(const void *a, const void *b)
{
    int streamA = (*(Song **)a)->streams;
    int streamB = (*(Song **)b)->streams;
    return (streamB > streamA) - (streamB < streamA);
}


int compareByTitle(const void *a, const void *b)
{
    return strcmp((*(Song **)a)->title, (*(Song **)b)->title);
}


void insertionSort(Song **songs, int n, int (*comparator)(const void *, const void *))
{
    for (int i = 1; i < n; i++) {
        if (songs[i] == NULL) {
            continue;
        }
        Song *key = songs[i];
        int j = (i - 1);
        while (j >= 0 && songs[j] != NULL && comparator(&songs[j], &key) > 0) {
            songs[j + 1] = songs[j];
            j--;
        }
        songs[j + 1] = key;
    }
}

Song **merge(Song **left, int leftCount, Song **right, int rightCount, int (*comparator)(const void *, const void *))
{
    Song **result = (Song **)malloc((leftCount + rightCount) * sizeof(Song *));
    if (result == NULL) {
        printf("Invalid option\n");
        return NULL;
    }
    int
        i = 0,
        j = 0,
        k = 0;
    while (i < leftCount && j < rightCount) {
        if (left[i] != NULL && right[j] != NULL && comparator(&left[i], &right[j]) <= 0) {
            result[k++] = left[i++];
        } else {
            result[k++] = right[j++];
        }
    }
    while (i < leftCount && left[i] != NULL) {
        result[k++] = left[i++];
    }
    while (j < rightCount && right[j] != NULL) {
        result[k++] = right[j++];
    }
    return result;
}


Song **mergeSort(Song **songs, int n, int (*comparator)(const void *, const void *))
{
    if (n < 2) {
        return songs;
    }

    int mid = (n / 2);

    Song **left = mergeSort(songs, mid, comparator);
    Song **right = mergeSort((songs + mid), (n - mid), comparator);

    if (left == NULL || right == NULL) {
        free(left);
        free(right);
        printf("Invalid option\n");
        return NULL;
    }

    Song **sorted = merge(left, mid, right, n - mid, comparator);
    if (sorted == NULL) {
        free(left);
        free(right);
        printf("Invalid option\n");
        return NULL;
    }

    memcpy(songs, sorted, n * sizeof(Song *));
    free(sorted);

    return songs;
}


void hybridSort(Song **songs, int n, int (*comparator)(const void *, const void *))
{
    if (n <= SMALL_DATASET_THRESHOLD) {
        insertionSort(songs, n, comparator);
    } else {
        Song **sorted = mergeSort(songs, n, comparator);
        if (sorted != NULL) {
            memcpy(songs, sorted, n * sizeof(Song *));
        }
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
    if (songIndex >= 0 && songCollected != NULL && songCollected[songIndex] != NULL) {
        printf("Now playing %s:\n", songCollected[songIndex]->title);
        printf("$ %s $\n", songCollected[songIndex]->lyrics);
        songCollected[songIndex]->streams++;
    }
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
        chosen = QUIT,
        input = INVALID;

    do {
        printf("choose a song to %s, or 0 to quit:\n", action);
        if (songCount <= 0 || ((input = scanf(" %d", &chosen) == 1) && chosen >= QUIT && chosen <= songCount)) {
            break;
        }
        printf("Invalid option\n");
        scanf("%*[^\n]");
        scanf("%*c");
    } while (1);
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

        if (scanf(" %d", &chosen) != 1 || chosen <= (menuNumber - menuNumber) || chosen > menuNumber) {
            chosen = INVALID;
            scanf("%*[^\n]");
            scanf("%*c");
            printf("Invalid option\n");
            continue;
        }

        if (chosen == menuNumber) {
            return GO_HOME;
        }
        break;
    } while (1);
    return chosen;
}


void delSong(Song ***songCollected, int *songCount, int songIndex)
{
    freeSong((*songCollected)[songIndex]);
    (*songCollected)[songIndex] = NULL;

    for (int i = songIndex; i < *songCount - 1; i++) {
        (*songCollected)[i] = (*songCollected)[i + 1];
    }

    (*songCount)--;

    if (*songCount > 0) {
        Song **temp = realloc(*songCollected, (*songCount) * sizeof(Song *));
        if (temp == NULL) {
            printf("Invalid option\n");
        } else {
            *songCollected = temp;
        }
    } else {
        free(*songCollected);
        *songCollected = NULL;
    }
}


void delPlaylist(Playlist ***playlistCollected, int *playlistCount, int playlistIndex)
{
    freePlaylist((*playlistCollected)[playlistIndex]);
    (*playlistCollected)[playlistIndex] = NULL;

    for (int i = playlistIndex; i < *playlistCount - 1; i++) {
        (*playlistCollected)[i] = (*playlistCollected)[i + 1];
    }

    (*playlistCount)--;

    if (*playlistCount == 0) {
        free(*playlistCollected);
        *playlistCollected = NULL;
    } else {
        Playlist **temp = realloc(*playlistCollected, (*playlistCount) * sizeof(Playlist *));
        if (temp == NULL) {
            printf("Invalid option\n");
            free(*playlistCollected);
            *playlistCollected = NULL;
        } else {
            *playlistCollected = temp;
        }
    }
}


void addSong(Song ***songCollected, int *songCount)
{
    printf("Enter song's details:\n");

    Song *newSong = malloc(sizeof(Song));
    if (newSong == NULL) {
        printf("Invalid option\n");
        return;
    }

    // get song title
    newSong->title = readStringInput("Title:\n");
    if (newSong->title == NULL) {
        free(newSong);
        printf("Invalid option\n");
        return;
    }

    // get artist name
    newSong->artist = readStringInput("Artist:\n");
    if (newSong->artist == NULL) {
        free(newSong->title);
        free(newSong);
        printf("Invalid option\n");
        return;
    }

    // get year of release
    newSong->year = readIntegerInput("Year of release:\n");

    // get lyrics
    newSong->lyrics = readStringInput("Lyrics:\n");
    if (newSong->lyrics == NULL) {
        free(newSong->title);
        free(newSong->artist);
        free(newSong);
        printf("Invalid option\n");
        return;
    }

    // initialize stream count
    newSong->streams = 0;

    // Attempt to expand the song list
    Song **temp = realloc(*songCollected, (*songCount + 1) * sizeof(Song *));
    if (temp == NULL) {
        freeSong(newSong);
        printf("Invalid option\n");
        return;
    }

    *songCollected = temp;
    (*songCollected)[*songCount] = newSong;
    (*songCount)++;
}


void addPlaylist(Playlist ***playlistCollected, int *playlistCount) {
    printf("Enter playlist's name:\n");
    char *playlistName = NULL;
    size_t size = 0;

    if (getlineCustom(&playlistName, &size) == NULL || strlen(playlistName) == 0) {
        free(playlistName);
        playlistName = NULL;
        printf("Invalid option\n");
        return;
    }

    Playlist *newPlaylist = malloc(sizeof(Playlist));
    if (newPlaylist == NULL) {
        free(playlistName);
        playlistName = NULL;
        printf("Invalid option\n");
        return;
    }

    newPlaylist->name = strdupCustom(playlistName);
    free(playlistName);
    playlistName = NULL;

    if (newPlaylist->name == NULL) {
        free(newPlaylist);
        printf("Invalid option\n");
        return;
    }

    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;

    Playlist **tempCollection = realloc(*playlistCollected, (*playlistCount + 1) * sizeof(Playlist *));
    if (tempCollection == NULL) {
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
    if (playlist == NULL) {
        return;
    }
    int
        chosen = BACK,
        identity = INVALID,
        option = INVALID,
        printMenu = 1;
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
                VIEW, ADD, DELETE, SORT, PLAY, BACK
            );
        }
        option = scanf(" %d", &chosen);
        if (chosen == BACK) {
            return;
        }

        if (option != 1) {
            printMenu = 1;
            scanf("%*[^\n]");
            scanf("%*c");
            printf("Invalid option\n");
            continue;
        } else {
            printMenu = 0;
        }

        switch (chosen) {
            case VIEW: {
                if (playlistCurrent->songs != NULL && playlistCurrent->songsNum > 0) {
                    songID(playlistCurrent->songs, playlistCurrent->songsNum);
                }
                do {
                    identity = songSelect("play", playlistCurrent->songsNum);
                    if (identity != INVALID && identity != QUIT) {
                        int identityIndex = identity - 1;
                        playSong(identityIndex, playlistCurrent->songs);
                    }
                } while (identity != QUIT);
                break;
            }
            case ADD: {
                addSong(&playlistCurrent->songs, &playlistCurrent->songsNum);
                break;
            }
            case DELETE: {
                if (playlistCurrent->songs != NULL && playlistCurrent->songsNum > 0) {
                    songID(playlistCurrent->songs, playlistCurrent->songsNum);
                }
                if ((identity = songSelect("delete", playlistCurrent->songsNum)) != INVALID && identity != QUIT) {
                    int identityIndex = identity - 1;
                    delSong(&playlistCurrent->songs, &playlistCurrent->songsNum, identityIndex);
                }
                break;
            }
            case SORT: {
                if (playlistCurrent->songs != NULL && playlistCurrent->songsNum > 0) {
                    songSort(playlistCurrent);
                }
                printf("sorted\n");
                break;
            }
            case PLAY: {
                if (playlistCurrent->songs != NULL && playlistCurrent->songsNum > 0) {
                    playAllSong(playlistCurrent->songs, playlistCurrent->songsNum);
                }
                break;
            }
            case BACK: {
                break;
            }
            default: {
                scanf("%*[^\n]");
                scanf("%*c");
                printf("Invalid option\n");
                break;
            }
        }
    printMenu = 1;
    } while (1);
}


int home(Playlist ***playlistCollected, int *playlistCount)
{
    int
        identity = INVALID,
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
                VIEW, ADD, DELETE, KILL
            );
        }

        option = scanf(" %d", &chosen);

        if (option == EOF || chosen == KILL) {
            return KILL;
        }

        if (option != 1) {
            chosen = INVALID;
            printMenu = 1;
            scanf("%*[^\n]");
            scanf("%*c");
            printf("Invalid option\n");
            continue;
        } else {
            printMenu = 0;
        }

        switch (chosen) {
            case VIEW: {
                while (*playlistCollected != NULL && *playlistCount > 0 && (identity = playlistID(*playlistCollected, *playlistCount)) && identity >= 1) {
                    int identityIndex = (identity - 1);
                    playlistGoTo((*playlistCollected)[identityIndex]);
                }
                break;
            }
            case ADD: {
                addPlaylist(playlistCollected, playlistCount);
                break;
            }
            case DELETE: {
                if (*playlistCollected != NULL && *playlistCount > 0 && (identity = playlistID(*playlistCollected, *playlistCount)) != INVALID && identity >= 1) {
                    int identityIndex = (identity - 1);
                    delPlaylist(playlistCollected, playlistCount, identityIndex);
                }
            break;
            }
            case KILL: {
                return KILL;
            }
            default: {
                printf("Invalid option\n");
                break;
            }
        }
        printMenu = 1;
    } while (1);
    return chosen;
}


int main()
{
    Playlist **playlistCollected = NULL;
    int playlistCount = 0;
    while ((home(&playlistCollected, &playlistCount)) != KILL);
    if (playlistCollected != NULL) {
        for (int i = 0; i < playlistCount; i++) {
            if (playlistCollected[i] != NULL) {
                freePlaylist(playlistCollected[i]);
            }
        }
         free(playlistCollected);
        playlistCollected = NULL;
    }
    printf("Goodbye!\n");
    return 0;
}
