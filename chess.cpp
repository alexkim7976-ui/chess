#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
FILE *in;

void game_init();
void now_board();
void game(int tempo);
bool piece_move(int turn, int type, int sty, int stx, int edy, int edx);
void now_analysis(int tempo);

int board[8][8];
char chess_piece[7][7] = {  {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                            {'K', 'k', 'i', 'n', 'g', ' ', ' '},
                            {'Q', 'q', 'u', 'e', 'e', 'n', ' '},
                            {'R', 'R', 'o', 'o', 'k', ' ', ' '},
                            {'B', 'b', 'i', 's', 'h', 'o', 'p'},
                            {'N', 'k', 'n', 'i', 'g', 'h', 't'},
                            {' ', 'p', 'a', 'w', 'n', ' ', ' '}, };
char board_print[2][8] = {  {'8', '7', '6', '5', '4', '3', '2', '1'},
                            {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}};
int chess_point[7] = {0, 999, 9, 5, 3, 3, 1};
/// White's piece -> +, Black's piece -> -
/// chess 판은 높이는 8 7 6 5 4 4 3 2 1, 열은  a b c d e f g h 이다.


void game_init() {
    int i, p;
    for(i = 0; i < 8; i++) { board[1][i] = -6; board[6][i] = 6; }  /// pawn 설정
    board[0][0] = -3; board[0][7] = -3; board[7][0] = 3; board[7][7] = 3; /// Look 설정
    board[0][2] = -4; board[0][5] = -4; board[7][2] = 4; board[7][5] = 4; /// Bishop 설정
    board[0][1] = -5; board[0][6] = -5; board[7][1] = 5; board[7][6] = 5; /// Knight 설정
    board[0][3] = -2; board[0][4] = -1; board[7][3] = 2; board[7][4] = 1; /// KingQueen 설정
    printf("> game initialization done\n\n");
    now_board();
    printf("\n");
    while(true) {
        printf("-----------------------------------------------------\n");
        printf("진행하고 싶은 작업을 선책하세요. \n1. Game 시작 2. 종료 \n");
        fflush(stdin);
        scanf("%d", &p);
        if(p == 1) {
            game(1);
        }
        if(p == 2) {
            printf("프로그램을 종료합니다.\n");
            exit(0);
        }
    }
}
void now_board() {
    int i, j, k;
    printf("-------------------Now Board-------------------------\n\n");
    for(i = 0; i < 8; i++) {
        printf("%c | ", board_print[0][i]);
        for(j = 0; j < 8; j++) {
            if(board[i][j] == 0) { printf("  "); }
            else if(board[i][j] > 0) { printf("W "); }
            else if(board[i][j] < 0) { printf("B "); }
            for(k = 1; k <= 6; k++) { printf("%c", chess_piece[abs(board[i][j])][k]); }
            printf("|");
        }
        printf("\n");
    }
    printf("  | ");
    for(j = 0; j < 8; j++) {
        for(k = 1; k <= 6; k++) { printf("%c", chess_piece[0][k]); if(k == 3) { printf("%c ", board_print[1][j]); } }
        printf("|");
    }
    printf("\n");
    printf("-----------------------------------------------------\n");
}
void SF(char *tmp) {
    fscanf(in, "%s", tmp);
    printf("%s\n", tmp);
}
void castling(int turn, char lo) {
    int i, nx;
    for(i = 0; i < 8; i++) { if(lo == board_print[1][i]){ nx = i; break; } }
    if(nx == 7 && (turn%2)==1) {
        board[7][7] = 0; board[7][4] = 0;
        board[7][6] = 1; board[7][5] = 3;
    }
    else if(nx == 0 && (turn%2)==1) {
        board[7][0] = 0; board[7][4] = 0;
        board[7][2] = 1; board[7][3] = 3;
    }
    else if(nx == 7 && (turn%2)==0) {
        board[0][7] = 0; board[0][4] = 0;
        board[0][6] = -1; board[0][5] = -3;
    }
    else if(nx == 0 && (turn%2)==0) {
        board[0][0] = 0; board[0][4] = 0;
        board[0][2] = -1; board[0][3] = -3;
    }
}
void game(int tempo) {
    char tmp1[10], temp[10];
    int i, nx, ny, lx, ly, typ;

    if(tempo % 2 == 1) { printf("<백 차례>\n"); }
    else { printf("<흑 차례>\n"); }

    now_analysis(tempo%2);
    ///f1: 수동일 떄 !
    printf("움직이려는 말의 위치를 지정하십시오 : "); SF(tmp1);
    if(tmp1[0] == 'C' && tmp1[1] == 'a') { castling(tempo, tmp1[4]); goto f4;}
    if(tmp1[0] == 'Q' && tmp1[1] == 'u') { goto f3;}
    if((strlen(tmp1) != 2) || (((tmp1[0] < 'a') || (tmp1[0] > 'h')) || ((tmp1[1] < '1') || (tmp1[1] > '8')))) { printf("잘못된 입력입니다.\n"); goto f1;}
    for(i = 0; i < 8; i++) { if(tmp1[0] == board_print[1][i]){ nx = i; break; } }
    for(i = 0; i < 8; i++) { if(tmp1[1] == board_print[0][i]){ ny = i; break; } }
    if(board[ny][nx] == 0) { printf("해당 위치에 말이 존재하지 않습니다.\n"); goto f1;}
    if(tempo%2 == 1 && board[ny][nx] < 0) { printf("현재 턴에 조정할 수 없는 말입니다.\n"); goto f1;}
    if(tempo%2 == 0 && board[ny][nx] > 0) { printf("현재 턴에 조정할 수 없는 말입니다.\n"); goto f1;}

    printf("기보를 입력하세요 : "); SF(temp);
    if((strlen(temp) != 2) && (strlen(temp) != 3)) { printf("잘못된 기보입니다.\n"); goto f1;}

    /// <입력 기보 특수 특징>
    /// 해당 위치 이동 가능한 기물의 중복에 따른 기보 표시 생략
    /// check, checkmate, promotion 표시 생략
    /// 말 잡는 것 제외
    /// Cast_ -> castling (o - o)
    if(strlen(temp) == 2) {
        if(((temp[0] < 'a') || (temp[0] > 'h')) || ((temp[1] < '1') || (temp[1] > '8'))) { printf("잘못된 기보입니다 - 판을 벗어남.\n"); goto f1;}
        for(i = 0; i < 8; i++) { if(temp[0] == board_print[1][i]){ lx = i; break; } }
        for(i = 0; i < 8; i++) { if(temp[1] == board_print[0][i]){ ly = i; break; } }
        typ = 6;
        if(typ != abs(board[ny][nx])) {printf("잘못된 기보입니다 - 지정한 위치의 말과 일치하지 않음.\n"); goto f1; }
    }
    else if(strlen(temp) == 3) {
        if(((temp[1] < 'a') || (temp[1] > 'h')) || ((temp[2] < '1') || (temp[2] > '8'))) { printf("잘못된 기보입니다 - 판을 벗어남.\n"); goto f1;}
        typ = -1;
        for(i = 1; i < 6; i++) { if(temp[0] == chess_piece[i][0]){ typ = i; break; } }
        if(typ == -1) { printf("잘못된 기보입니다 - 존재하지 않는 기물에 대한 기보.\n"); goto f1; }
        for(i = 0; i < 8; i++) { if(temp[1] == board_print[1][i]){ lx = i; break; } }
        for(i = 0; i < 8; i++) { if(temp[2] == board_print[0][i]){ ly = i; break; } }
        if(typ != abs(board[ny][nx])) {printf("잘못된 기보입니다 - 지정한 위치의 말과 일치하지 않음.\n"); goto f1; }
    }
    if(piece_move(tempo, typ, ny, nx, ly, lx)) {
        if(tempo%2 == 1 && board[ly][lx] > 0) { printf("잘못된 기보입니다 - 같은 편의 기물은 잡을 수 없습니다.\n"); goto f1; }
        if(tempo%2 == 0 && board[ly][lx] < 0) { printf("잘못된 기보입니다 - 같은 편의 기물은 잡을 수 없습니다.\n"); goto f1; }
        board[ly][lx] = board[ny][nx];  board[ny][nx] = 0;
    }
    else { printf("잘못된 기보입니다 - 기물이 이동가능한 위치가 아님.\n"); goto f1; }
    f4:
    now_board();

    ///promotion
    if(typ == 6 && ((tempo%2==1 && ly == 0) || (tempo%2==0 && ly == 7)) ) {
        f2: printf("<Promotion> 변경하고자 하는 말의 종류를 약자로 지정하세요 : "); scanf("%s", &temp);
        for(i = 1; i < 6; i++) { if(temp[0] == chess_piece[i][0]){ typ = i; break; } }
        if(typ == -1) { printf("잘못된 입력입니다 - 존재하지 않는 기물임.\n"); goto f2; }
        else { board[ly][lx] /= 6; board[ly][lx] *= typ; }
    }

    if(0) {
        f1: /// 자동 일떄 !
        f3:
        printf("종료합니다.");
    }
    else {
        game(tempo+1);
    }
}
void must_move(int ny, int nx,  int py, int px) {
    printf("must_move_available %c%c - %c%c\n", board_print[1][px],board_print[0][py], board_print[1][nx],board_print[0][ny]);
}
void double_attack_check(int tempo, int typ, int ny, int nx, int py, int px) {
    /// double attack
    /// 먼저 해당 기물이 잡힐 수 없는 상태일 때
    int i, j, co=0;
    for(i = 0; i <8; i++) {
        for(j = 0; j < 8; j++) {
            if(board[i][j]*(2*(tempo%2)-1) < 0 && piece_move(tempo+1, abs(board[i][j]), i, j, ny, nx)) {
                goto p1;
            } /// 잡힐 수 없는 상태
            if(board[i][j]*(2*(tempo%2)-1) < 0 && piece_move(tempo, typ, ny, nx, i, j))
            {
                co++;
                if(chess_point[abs(board[i][j])] > 100)
                {
                    must_move(ny, nx, py, px);
                }
            }
        }
    }
    if(co > 1)
    {
        printf("Double Attack Available : %c%c - %c%c\n", board_print[1][px],board_print[0][py], board_print[1][nx],board_print[0][ny]);
        int poin=0;
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                if(board[i][j]*(2*(tempo%2)-1) < 0 && piece_move(tempo, typ, ny, nx, i, j))
                {
                    printf("   %c%c%c\n", board_print[1][j],board_print[0][i],chess_piece[abs(board[i][j])][0]);
                    poin += chess_point[abs(board[i][j])];
                }
            }
        }
        printf("이득 : %d\n", poin);
        return;
    }
    else
    {
        p1:
        return;
    }
    /// 전체 board를 scan하여 기물 배열을 구성한 후 각각 piece move 로 공격권에 있는지 체크
    /// 1. 체크일 경우 - 무한이니까 무조건 손해 x
    /// 2. 체크가 아닐 경우 - 그 기물을 잡았을 때 잡히는 경우 손해이면 안됨
    /// 또는 상대방이 기물을 움직여서 두개의 기물을 동시에 방어하는 경우도 제외 -> 이 때는 표시해주자(왜냐하면 체크일 때는 아님)

}
void now_analysis(int tempo) {
    printf("-----------------현재 가능 수 분석-------------------\n");
    int i, j, k, l, tmp;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if(board[i][j]*(2*tempo-1) > 0)
            {
                for(k = 0; k < 8; k++)
                {
                    for(l = 0; l < 8; l++)
                    {
                        if(piece_move(tempo, abs(board[i][j]), i, j, k, l) && board[k][l]*(2*tempo-1) <= 0) {
                            tmp = board[k][l]; board[k][l] = board[i][j];  board[i][j] = 0;
                            double_attack_check(tempo, abs(board[k][l]), k, l, i, j);
                            board[i][j] = board[k][l];  board[k][l] = tmp;
                        }
                    }
                }
            }
        }
    }
    printf("-----------------------------------------------------\n");
}

bool piece_move(int turn, int type, int sty, int stx, int edy, int edx) {
    if(stx == edx && sty == edy) {
        return false;
    }
    switch (type) {
        case 1 :
            if(abs(sty - edy) <= 1 && abs(stx - edx) <= 1) { return true; }
            else { return false; }
        case 2 :
            if(((sty!=edy) && (edx!=stx)) && (abs(sty-edy) != abs(stx - edx))) { return false; }
            else {
                if(edy == sty) { edy+=0; } else if(edy > sty) { sty++; } else { sty--; }
                if(edx == stx) { edx+=0; } else if(edx > stx) { stx++; } else { stx--; }
                while(sty != edy || stx != edx) {
                    if(board[sty][stx] != 0) {
                        return false;
                    }
                    if(edy == sty) { edy+=0; } else if(edy > sty) { sty++; } else { sty--; }
                    if(edx == stx) { edx+=0; } else if(edx > stx) { stx++; } else { stx--; }
                }
                return true;
            }
        case 3 :
            if((sty!=edy) && (edx!=stx)) { return false; }
            else {
                if(edy == sty) { edy+=0; } else if(edy > sty) { sty++; } else { sty--; }
                if(edx == stx) { edx+=0; } else if(edx > stx) { stx++; } else { stx--; }
                while(sty != edy || stx != edx) {
                    if(board[sty][stx] != 0) {
                        return false;
                    }
                    if(edy == sty) { edy+=0; } else if(edy > sty) { sty++; } else { sty--; }
                    if(edx == stx) { edx+=0; } else if(edx > stx) { stx++; } else { stx--; }
                }
                return true;
            }
        case 4 :
            if(abs(sty-edy) != abs(stx - edx)) { return false;}
            else {
                if(edy > sty) { sty++; } else { sty--; }
                if(edx > stx) { stx++; } else { stx--; }
                while(sty != edy || stx != edx) {
                    if(board[sty][stx] != 0) {
                        return false;
                    }
                    if(edy > sty) { sty++; } else { sty--; }
                    if(edx > stx) { stx++; } else { stx--; }
                }
                return true;
            }
        case 5 :
            if(abs(sty - edy) == 1 && abs(stx - edx) == 2) { return true; }
            else if(abs(sty - edy) == 2 && abs(stx - edx) == 1) { return true; }
            else { return false; }
        case 6 :
            if((turn%2) == 1) {
                if((sty - edy == 1 && edx == stx) && board[edy][edx]==0) { return true; }
                else if((board[edy][edx]==0 && sty == 6) && (sty - edy == 2 && edx == stx)) { return true; }
                else if(board[edy][edx] < 0 && (sty - edy == 1 && abs(edx-stx) == 1)) { return true; }
                else { return false; }
            }
            else {
                if(edy - sty == 1 && edx == stx) { return true; }
                else if((board[edy][edx]==0 && sty == 1) && (edy - sty == 2 && edx == stx)) { return true; }
                else if(board[edy][edx] > 0 && (edy - sty == 1 && abs(edx-stx) == 1)) { return true; }
                else { return false; }
            }
    }
}


int main() {
    in = fopen("input.txt", "r");
    game_init();

    return 0;
}
