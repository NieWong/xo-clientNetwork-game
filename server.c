#include "csapp.h"
#include "tictac.h"

void run_game(int conn);
void update_opponent(int conn, char grid[B_size][B_size], int turn);
void tell_game_end(int conn, int result);
void get_input(char grid[B_size][B_size], int *r, int *c, int local_player);
void wait_for_move(int conn, int *r, int *c);

int main(int argc, char **argv) {
    int listen_sock, conn_sock;
    socklen_t client_size;
    struct sockaddr_in client_addr;
    char ip_buf[INET_ADDRSTRLEN];
    if (argc != 2) {
        fprintf(stderr, "err: %s <port>\n", argv[0]);
        exit(0);
    }
    int port_num = atoi(argv[1]);
    char port_buf[10];
    sprintf(port_buf, "%d", port_num);
    listen_sock = open_listenfd(port_buf);
    if (listen_sock < 0) {
        fprintf(stderr, "port bolomjgv bn %d\n", port_num);
        exit(1);
    }
    printf("port :  %d...\n", port_num);
    printf("p2 orohiig hvleej bn...\n");
    client_size = sizeof(client_addr);
    conn_sock = Accept(listen_sock, (SA *)&client_addr, &client_size);
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_buf, INET_ADDRSTRLEN);
    printf("p2 orson ip: %s\n", ip_buf);
    run_game(conn_sock);
    close(conn_sock);
    close(listen_sock);
    return 0;
}

void run_game(int conn) {
    char grid[B_size][B_size];
    int r, c;
    int cur_player = P1;
    int status = G_on;
    init_board(grid);
    while (status == G_on) {
        clear_screen();
        printf("xo (hojih onoo ; %d)\n", W_score);
        printf("ta bl p1 (%c)\n\n", P1);
        dis_board(grid);
        if (cur_player == P1) {
            printf("nuu nuu (5 8): ");
            get_input(grid, &r, &c, 1);
            grid[r][c] = P1;
            if (check_win(grid, r, c)) {
                status = P1_win;
            } else if (check_tie(grid)) {
                status = Tie;
            }
            update_opponent(conn, grid, P2);
            if (status == G_on) {
                cur_player = P2;
            }
        } else {
            printf("hueelgerei...\n");
            wait_for_move(conn, &r, &c);
            grid[r][c] = P2;
            if (check_win(grid, r, c)) {
                status = P2_win;
            } else if (check_tie(grid)) {
                status = Tie;
            }
            if (status == G_on) {
                cur_player = P1;
            }
        }
    }
    clear_screen();
    dis_board(grid);    
    if (status == P1_win) {
        printf("hojil! (%c)\n", P1);
    } else if (status == P2_win) {
        printf("P2 hojil (%c) wins!\n", P2);
    } else {
        printf("tentsseen!\n");
    }
    tell_game_end(conn, status);
    printf("Enter darad gararai.");
    getchar();
}
void update_opponent(int conn, char grid[B_size][B_size], int turn) {
    game_msg_t msg;
    msg.type = M_board;
    memcpy(msg.data.board_update.grid, grid, B_size * B_size);
    msg.data.board_update.turn = turn;
    Rio_writen(conn, &msg, sizeof(msg));
}
void tell_game_end(int conn, int result) {
    game_msg_t msg;   
    msg.type = M_over;
    msg.data.result = result;
    Rio_writen(conn, &msg, sizeof(msg));
}
void get_input(char grid[B_size][B_size], int *r, int *c, int local_player) {
    char buf[64];   
    if (local_player) {
        for (;;) {
            if (fgets(buf, sizeof(buf), stdin) == NULL) {
                printf("utga aldaatai: ");
                continue;
            }
            if (sscanf(buf, "%d %d", r, c) != 2) {
                printf("buruu format: ");
                continue;
            }
            if (!valid_move(grid, *r, *c)) {
                printf("tend nuuj bolohgvi: ");
                continue;
            }
            break;
        }
    }
}
void wait_for_move(int conn, int *r, int *c) {
    game_msg_t msg;
    Rio_readn(conn, &msg, sizeof(msg));
    if (msg.type != M_move) {
        fprintf(stderr, "err\n");
        exit(1);
    }
    *r = msg.data.move.row;
    *c = msg.data.move.col;
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
int check_win(char grid[B_size][B_size], int r, int c) {
    char mark = grid[r][c];
    int i, j, count;
    count = 1;
    for (j = c-1; j >= 0 && grid[r][j] == mark; j--) count++;
    for (j = c+1; j < B_size && grid[r][j] == mark; j++) count++;
    if (count >= W_score) return 1;
    count = 1;
    for (i = r-1; i >= 0 && grid[i][c] == mark; i--) count++;
    for (i = r+1; i < B_size && grid[i][c] == mark; i++) count++;
    if (count >= W_score) return 1;
    count = 1;
    for (i = r-1, j = c-1; i >= 0 && j >= 0 && grid[i][j] == mark; i--, j--) count++;
    for (i = r+1, j = c+1; i < B_size && j < B_size && grid[i][j] == mark; i++, j++) count++;
    if (count >= W_score) return 1;
    count = 1;
    for (i = r-1, j = c+1; i >= 0 && j < B_size && grid[i][j] == mark; i--, j++) count++;
    for (i = r+1, j = c-1; i < B_size && j >= 0 && grid[i][j] == mark; i++, j--) count++;
    if (count >= W_score) return 1;
    return 0;
}
int check_tie(char grid[B_size][B_size]) {
    int i, j;
    for (i = 0; i < B_size; i++) {
        for (j = 0; j < B_size; j++) {
            if (grid[i][j] == BL) {
                return 0;
            }
        }
    }
    return 1;
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