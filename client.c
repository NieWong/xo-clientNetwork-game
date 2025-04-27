#include "csapp.h"
#include "tictac.h"

void start_client(int sock);
void send_my_move(int sock, int r, int c);
void ask_for_move(char grid[B_size][B_size], int *r, int *c);
int get_server_msg(int sock, game_msg_t *msg);

int main(int argc, char **argv) {
    int sock;
    if (argc != 3) {
        exit(0);
    }
    char *host = argv[1];
    int port = atoi(argv[2]);
    char port_str[10];
    sprintf(port_str, "%d", port);
    printf("holbogdoj bn %s:%s...\n", host, port_str);
    
    sock = open_clientfd(host, port_str);
    if (sock < 0) {
        fprintf(stderr, "holbogdoh bolomjgv bn %s:%d\n", host, port);
        exit(1);
    }
    printf("holbogdson ta p2 (%c).\n", P2);
    start_client(sock);
    close(sock);
    return 0;
}

void start_client(int sock) {
    game_msg_t msg;
    char grid[B_size][B_size];
    int r, c;
    int done = 0;
    init_board(grid);
    while (!done) {
        if (!get_server_msg(sock, &msg)) {
            fprintf(stderr, "server error :(\n");
            exit(1);
        }
        switch (msg.type) {
            case M_board:
                memcpy(grid, msg.data.board_update.grid, B_size * B_size);
                clear_screen();
                printf("xo (hojih onoo ; %d)\n", W_score);
                printf("ta bl p2 (%c)\n\n", P2);
                dis_board(grid);
                if (msg.data.board_update.turn == P2) {
                    printf("uildlee hiinuu /mor bagana/: ");
                    ask_for_move(grid, &r, &c);
                    
                    grid[r][c] = P2;
                    send_my_move(sock, r, c);
                } else {
                    printf("ail bodjiin...\n");
                }
                break;
            case M_over:
                clear_screen();
                dis_board(grid);
                if (msg.data.result == P1_win) {
                    printf("p1 (%c) hojson!\n", P1);
                } else if (msg.data.result == P2_win) {
                    printf("ta hojson! (%c)\n", P2);
                } else {
                    printf("tentschilee.\n");
                }
                done = 1;
                break;
                
            default:
                break;
        }
    }
    printf("enter daraad garna uu.");
    getchar();
}
void send_my_move(int sock, int r, int c) {
    game_msg_t msg;
    msg.type = M_move;
    msg.data.move.row = r;
    msg.data.move.col = c;
    Rio_writen(sock, &msg, sizeof(msg));
}
void ask_for_move(char grid[B_size][B_size], int *r, int *c) {
    char input[64];   
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("buru input: ");
            continue;
        }
        if (sscanf(input, "%d %d", r, c) != 2) {
            printf("too oruulna uu");
            continue;
        }
        if (!valid_move(grid, *r, *c)) {
            printf("tend nuuh bolomjgvi: ");
            continue;
        }
        break;
    }
}
int get_server_msg(int sock, game_msg_t *msg) {
    ssize_t n = rio_readn(sock, msg, sizeof(game_msg_t));
    if (n <= 0) {
        return 0;
    }
    return 1;
}

void init_board(char grid[B_size][B_size]) {
    int i, j;
    for (i = 0; i < B_size; i++) {
        for (j = 0; j < B_size; j++) {
            grid[i][j] = BL;
        }
    }
}

void dis_board(char grid[B_size][B_size]) {
    int i, j;
    printf("   ");
    for (j = 0; j < B_size; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    printf("  +");
    for (j = 0; j < B_size; j++) {
        printf("---");
    }
    printf("+\n");
    for (i = 0; i < B_size; i++) {
        printf("%2d |", i);
        for (j = 0; j < B_size; j++) {
            printf(" %c ", grid[i][j]);
        }
        printf("|\n");
    }
    printf("  +");
    for (j = 0; j < B_size; j++) {
        printf("---");
    }
    printf("+\n");
}

int valid_move(char grid[B_size][B_size], int r, int c) {
    if (r < 0 || r >= B_size || c < 0 || c >= B_size) {
        return 0;
    }
    if (grid[r][c] != BL) {
        return 0;
    }
    
    return 1;
}

void clear_screen() {
    printf("\033[H\033[J");
}