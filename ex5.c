/******************
Name: Samantha Newmark
ID: 346587629
Assignment: ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GO_HOME -2
#define INVALID -1
#define QUIT 0
#define VIEW 1
#define ADD 2
#define DELETE 3
#define KILL 4
#define SORT 4
#define PLAY 5
#define BACK 6

typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

typedef struct playlistGroup {
    Playlist **playlistAll;
    int playlistCount;
} playlistGroup;


void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}


void freePlaylist(Playlist *p) {
    for (int i = 0; i < p->songsNum; i++) {
        freeSong(p->songs[i]);
    }
    free(p->songs);
    free(p->name);
    free(p);
}


void playSong(int songIndex, Song **songCollected) {
    printf("Now playing %s:\n", songCollected[songIndex]->title);
    printf("$ %s $\n", songCollected[songIndex]->lyrics);
    // songCollected[songIndex]->streams
}


void playAllSong(Song **songCollected, int songCount) {
    for (int songIndex = 0; songIndex < songCount; songIndex++) {
        playSong(songIndex, songCollected);
        
        
    }
}


void songID(Song **songCollected, int songCount)
{
    int currentIndex = INVALID;
    int menuNumber = 0;
    for (menuNumber = 1; menuNumber <= songCount; menuNumber++) {
        currentIndex = menuNumber - 1;
        printf("%d. Title: %s\n", menuNumber, songCollected[currentIndex]->title);
        printf("   Artist: %s\n", songCollected[currentIndex]->artist);
        printf("   Year: %d\n", songCollected[currentIndex]->year);
        printf("   Streams: %d\n", songCollected[currentIndex]->streams);
    }
}


int songSelect(char action[], int songCount) {
    int
        chosen = INVALID,
        input = INVALID;

    do {
        printf("choose a song to %s, or 0 to quit:\n", action);
        input = scanf(" %d", &chosen);

        scanf("%*[^\n]");
        scanf("%*c");

        // TODO: input == EOF;

        if (songCount == 0 || chosen == QUIT) {
            return QUIT;
        }
        if (input != 1 || chosen < 0 || chosen > songCount) {
            printf("Invalid option\n");
            chosen = INVALID;
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
            scanf("%*[^\n]");
            scanf("%*c");
            printf("Invalid option\n");
            chosen = INVALID;
            // return INVALID;
        }

        if (chosen == menuNumber) {
            return GO_HOME;
        }
    } while (chosen == INVALID);
    return chosen;
}


void delSong(Song ***songCollected, int *songCount, int songIndex) {
    freeSong((*songCollected)[songIndex]);

    for (int i = songIndex; i < *songCount - 1; i++) {
        (*songCollected)[i] = (*songCollected)[i + 1];
    }

    *songCollected = (Song **)realloc(*songCollected, (*songCount - 1) * sizeof(Song *));
    (*songCount)--;
}


void delPlaylist(Playlist ***playlistCollected, int *playlistCount, int playlistIndex) {
    freePlaylist((*playlistCollected)[playlistIndex]);

    for (int i = playlistIndex; i < *playlistCount - 1; i++) {
        (*playlistCollected)[i] = (*playlistCollected)[i + 1];
    }

    *playlistCollected = (Playlist **)realloc(*playlistCollected, (*playlistCount - 1) * sizeof(Playlist *));
    (*playlistCount)--;
}


char* readStringInput(const char* prompt) {
    char *buffer = NULL;
    size_t size = 0;
    ssize_t len;

    do {
        printf("%s", prompt);
        len = getline(&buffer, &size, stdin);
        if (len == -1 || buffer[0] == '\n') {
            printf("Invalid option\n");
            free(buffer);
            buffer = NULL;
            size = 0;
        } else {
            buffer[strcspn(buffer, "\n")] = '\0';
        }
    } while (len == -1 || buffer == NULL);

    return buffer;
}

int readIntegerInput(const char* prompt) {
    int value;
    int input;
    char confirmInt;
    do {
        printf("%s", prompt);
        input = scanf(" %d%c", &value, &confirmInt);
        if (input != 2 || (confirmInt != '\n' && confirmInt != ' ')) {
            printf("Invalid option\n");
            while (scanf("%*[^\n]") != 0);
            scanf("%*c");
        } else {
            while (confirmInt == ' ') {
                input = scanf("%c", &confirmInt);
                if (confirmInt != ' ' && confirmInt != '\n') {
                    printf("Invalid option\n");
                    while (scanf("%*[^\n]") != 0);
                    scanf("%*c");
                    input = INVALID;
                    break;
                }
            }
        }
    } while (input != 2 || confirmInt != '\n');
    return value;
}


void addSong(Song ***songCollected, int *songCount) {
    printf("Enter song's details:\n");

    Song *newSong = (Song *)malloc(sizeof(Song));
    if (!newSong) {
        printf("Invalid option\n");
        return;
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
        printf("Invalid option\n");
        free(newSong->title);
        free(newSong->artist);
        free(newSong->lyrics);
        free(newSong);
        return;
    }
    (*songCollected)[*songCount] = newSong;
    (*songCount)++;
}


void addPlaylist(Playlist ***playlistCollected, int *playlistCount) {
    printf("Enter playlist's name:\n");
    char *playlistName = NULL;
    size_t len = 0;
    size_t size = 64;
    playlistName = calloc(size, sizeof(char));
    if (!playlistName) {
        printf("Invalid option\n");
        return;
    }

    while (scanf(" %63[^\n]", playlistName + len) == 1) {
        size_t buffer_len = strlen(playlistName + len);
        len += buffer_len;
        if (len + 1 >= size) {
            size *= 2;
            char *new_playlistName = realloc(playlistName, size);
            if (!new_playlistName) {
                printf("Invalid option\n");
                free(playlistName);
                return;
            }
            playlistName = new_playlistName;
        }
        if (buffer_len < 63) {
            break;
        }
    }

    if (len == 0) {
        printf("Invalid option\n");
        free(playlistName);
        return;
    }

    playlistName[len] = '\0';

    Playlist *newPlaylist = (Playlist *)malloc(sizeof(Playlist));
    if (!newPlaylist) {
        printf("Invalid option\n");
        free(playlistName);
        return;
    }
    newPlaylist->name = strdup(playlistName);
    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;

    *playlistCollected = (Playlist **)realloc(*playlistCollected, (*playlistCount + 1) * sizeof(Playlist *));
    if (!*playlistCollected) {
        printf("Invalid option\n");
        free(newPlaylist->name);
        free(newPlaylist);
        free(playlistName);
        return;
    }
    (*playlistCollected)[*playlistCount] = newPlaylist;
    (*playlistCount)++;

    free(playlistName);
}


void playlistGoTo(Playlist *playlist)
{
    int chosen = BACK;
    int option = EOF;
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
        if (option == EOF || chosen == BACK) {
            return;
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
            case BACK:
                break;
            case VIEW: {
                songID(playlistCurrent->songs, playlistCurrent->songsNum);
                while ((chosen = songSelect("play", playlistCurrent->songsNum)) != INVALID && chosen != QUIT) {
                    int chosenIndex = chosen - 1;
                    playSong(chosenIndex, playlistCurrent->songs);
                }
                break;
            }
            case ADD:
                addSong(&playlistCurrent->songs, &playlistCurrent->songsNum);
                break;
            case DELETE: {
                songID(playlistCurrent->songs, playlistCurrent->songsNum);
                if ((chosen = songSelect("delete", playlistCurrent->songsNum)) != INVALID && chosen != QUIT) {
                    int chosenIndex = chosen - 1;
                    delSong(&playlistCurrent->songs, &playlistCurrent->songsNum, chosenIndex);
                }
                break;
            }
            case SORT:
                // TODO: sort
                break;
            case PLAY:
                playAllSong(playlistCurrent->songs, playlistCurrent->songsNum);
                break;
            default:
                // scanf("%*[^\n]");
                // scanf("%*c");
                printf("Invalid option\n");
                break;
        }
    printMenu = 1;
    } while (chosen != BACK);
}


int home(Playlist ***playlistCollected, int *playlistCount)
{
    int
        chosen = INVALID,
        option = INVALID,
        printMenu = 1;

    do {
        if (printMenu) {
            printf(
                "Please Choose:\n"
                "%d. Watch playlists\n"
                "%d. Add playlist\n"
                "%d. Remove playlist\n"
                "%d. Exit\n",

                VIEW,
                ADD,
                DELETE,
                KILL
            );
        }

        option = scanf(" %d", &chosen);

        if (chosen == KILL) {
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
            case KILL:
                break;
            case VIEW:
                do {
                    while ((chosen = playlistID(*playlistCollected, *playlistCount)) != INVALID && chosen != GO_HOME) {
                        int chosenIndex = chosen - 1;
                        playlistGoTo((*playlistCollected)[chosenIndex]);
                    }
                    // if (chosen == INVALID) {
                    //     printf("Invalid option\n");
                    // }
                } while (chosen == INVALID);
                break;
            case ADD:
                addPlaylist(playlistCollected, playlistCount);
                break;
            case DELETE:
            if ((chosen = playlistID(*playlistCollected, *playlistCount)) != INVALID && chosen != GO_HOME) {
                int chosenIndex = chosen - 1;
                delPlaylist(playlistCollected, playlistCount, chosenIndex);
                break;
            default:
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
}
