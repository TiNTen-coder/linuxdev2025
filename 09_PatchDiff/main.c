#include <stdio.h>
#include <stdlib.h>

typedef enum wall_type {None = 0, Room, DWall, IWall, Empty} tile;
char sym_vis[] = {'-', '.', '#', '#', '.'};

#define NUM_ROOMS 6

tile** init_lab(int num_rooms) {
    int i, j, lab_size = num_rooms * 2 + 1;
    tile **lab = malloc(lab_size * sizeof(tile*));
    for(i = 0; i < lab_size; i++) {
        lab[i] = malloc(lab_size * sizeof(tile));
        for(j = 0; j < lab_size; j++) {
            if (((j % 2 == 0) && (i % 2 == 0)) || (i == 0) ||
                    (i == 2 * num_rooms) || (j == 0) || (j == 2 * num_rooms)) {
                lab[i][j] = IWall;
                continue;
            }
            if ((j % 2 == 1) && (i % 2 == 1)) {
                lab[i][j] = Room;
                continue;
            }
            lab[i][j] = DWall;
        }
    }
    return lab;
}


int direction_x[] = {0, 1, -1,  0};
int direction_y[] = {1, 0,  0, -1};
void generate_lab_rec(tile **lab, int num_rooms, int **visited, int room_i, int room_j) {
    int i, x = 1, y = 1, direction, lab_i = room_i * 2 + 1, lab_j = room_j * 2 + 1;
    int visited_directions[] = {0, 0, 0, 0};
    if (visited[room_i][room_j]) {
        return;
    }
    visited[room_i][room_j] = 1;
    for(i = 0; i < 4; i++) {
        while (visited_directions[direction = rand() % 4] != 0) {};
        visited_directions[direction] = 1;
        x = direction_x[direction];
        y = direction_y[direction];
        switch (lab[lab_i + x][lab_j + y]) {
        case Empty:
            continue;
        case DWall:
            if ((room_i + x >= 0) && (room_j + y >= 0) &&
                    (room_i + x < num_rooms) && (room_j + y < num_rooms)) {
                if (visited[room_i + x][room_j + y])
                    continue;
                else {
                    lab[lab_i + x][lab_j + y] = Empty;
                    generate_lab_rec(lab, num_rooms, visited, room_i+x, room_j+y);
                }
            }
            continue;
        default:
            continue;
        }
    }
}

void generate_lab(tile **lab, int num_rooms) {
    int start_room_i, start_room_j, i;
    start_room_i = rand() % num_rooms;
    start_room_j = rand() % num_rooms;
    int **visited = calloc(num_rooms, sizeof(int*));
    for(i = 0; i < num_rooms; i++)
        visited[i] = calloc(num_rooms, sizeof(int));
    generate_lab_rec(lab, num_rooms, visited, start_room_i, start_room_j);
    for(i = 0; i < num_rooms; i++)
        free(visited[i]);
    free(visited);

}

void print_lab(tile **lab, int num_rooms) {
    int i, j, lab_size = num_rooms * 2 + 1;
    char *buf = malloc((lab_size + 1) * sizeof(char));
    for(i = 0; i < lab_size; i++) {
        for(j = 0; j < lab_size; j++) {
            buf[j] = sym_vis[lab[i][j]];
        }
        buf[j] = '\0';
        puts(buf);
    }
    free(buf);
}

void deinit_lab(tile **lab, int num_rooms) {
    int i, lab_size = num_rooms * 2 + 1;
    for(i = 0; i < lab_size; i++)
        free(lab[i]);
    free(lab);
}

int main() {
    tile **lab = init_lab(NUM_ROOMS);
    generate_lab(lab, NUM_ROOMS);
    print_lab(lab, NUM_ROOMS);
    deinit_lab(lab, NUM_ROOMS);
    return 0;
}
