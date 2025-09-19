/*
 * 核心思路:
 * 本题要求判断国际象棋中某一方是否被“将死”(Checkmate)。根据规则，将死有两个充要条件：
 * 1. 当前玩家的王正被对方棋子攻击（处于“被将军”状态）。
 * 2. 当前玩家没有任何合法的移动方式可以解除“被将军”的状态。
 *
 * 算法遵循这个定义，分为两步：
 * 1.  **检查将军状态**：首先，判断轮到走棋的一方（当前玩家），其王是否处于被攻击的位置。这需要遍历所有对手的棋子，
 * 根据每个棋子的走法规则，判断其攻击范围是否覆盖了当前玩家的王。如果王没有被将军，那么就不可能被将死，局面为"Not Checkmate"。
 *
 * 2.  **模拟所有合法走法**：如果王正被将军，我们需要判断是否存在任何一个合法的走法能摆脱将军。算法会遍历当前玩家的所有棋子，
 * 并为每个棋子生成所有可能的移动（包括吃子、移动到空格，以及兵的升变）。对于每一个可能的移动，我们都进行一次“模拟”：
 * a. 假设棋子进行了移动，生成一个临时的棋盘状态。
 * b. 在这个新的临时棋盘状态下，再次检查当前玩家的王是否还处于被攻击状态。
 * c. 如果存在任何一个移动，使得移动后王不再被攻击，那么当前玩家就没有被将死。我们可以立即停止搜索，并得出"Not Checkmate"的结论。
 *
 * 3.  **判定将死**：如果在遍历了当前玩家所有棋子的所有合法走法后，都无法找到一个能让王脱离被攻击状态的走法，那么就可以判定当前玩家已被将死。
 *
 * 为了简化处理，程序将输入坐标(x,y)从(1,1)至(8,8)的棋盘（右下角为(1,1)）映射到标准的[0,7]x[0,7]数组索引。
 *
 * 时间复杂度: O(N * M), 其中N是当前玩家的棋子总数，M是每个棋子平均的可能移动次数。对于一个8x8的棋盘，这个数值是一个较小的常数。
 * 遍历所有棋子生成所有可能的移动，并对每个移动进行一次棋盘扫描来检查将军状态，所以复杂度相对较高，但对于本题数据范围完全足够。
 *
 * 空间复杂度: O(1)。我们主要使用一个8x8的棋盘来存储状态，棋子数量有限，因此空间开销是常数级别的。模拟移动时创建的临时棋盘也可以视为常数空间。
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>

using namespace std;

// 定义棋子类型和颜色，便于管理
enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY };
enum PieceColor { WHITE, BLACK, NONE };

// 棋子结构体
struct Piece {
    PieceType type;
    PieceColor color;
};

// 使用map将输入的字符串转换为枚举类型
map<string, PieceType> pieceTypeMap = {
    {"King", KING}, {"Queen", QUEEN}, {"Rook", ROOK},
    {"Bishop", BISHOP}, {"Knight", KNIGHT}, {"Pawn", PAWN}
};

// 棋盘定义，8x8，使用Piece结构体
using Board = vector<vector<Piece>>;

// 函数声明
bool isSquareAttacked(int r, int c, PieceColor attackerColor, const Board& board);
bool canEscapeCheck(PieceColor playerColor, const Board& board);

// 检查指定坐标是否在棋盘内
bool isValid(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

// 主解决函数
void solve() {
    int n, p;
    cin >> n >> p;

    // 初始化一个8x8的空棋盘
    Board board(8, vector<Piece>(8, {EMPTY, NONE}));
    pair<int, int> kingPos[2]; // 存储白王和黑王的位置

    // 读取棋子信息并放置到棋盘上
    for (int i = 0; i < n; ++i) {
        string s;
        int f, x, y;
        cin >> s >> f >> x >> y;

        // 将输入坐标(1,1)~(8,8)转换为数组索引(0,0)~(7,7)
        // 输入的(x,y)中，x是列，y是行。右下角为(1,1)，左上角为(8,8)
        // 映射关系: row = 8 - y, col = 8 - x
        int r = 8 - y;
        int c = 8 - x;

        PieceType type = pieceTypeMap[s];
        PieceColor color = (f == 0) ? WHITE : BLACK;
        board[r][c] = {type, color};

        if (type == KING) {
            kingPos[color] = {r, c};
        }
    }

    PieceColor currentPlayer = (p == 0) ? WHITE : BLACK;
    PieceColor opponentPlayer = (p == 0) ? BLACK : WHITE;

    // 获取当前玩家的王的位置
    int kingR = kingPos[currentPlayer].first;
    int kingC = kingPos[currentPlayer].second;

    // 1. 判断当前王是否被将军
    if (!isSquareAttacked(kingR, kingC, opponentPlayer, board)) {
        cout << "Not Checkmate" << endl;
        return;
    }

    // 2. 如果被将军，判断是否有任何合法的走法可以逃脱
    if (canEscapeCheck(currentPlayer, board)) {
        cout << "Not Checkmate" << endl;
    } else {
        // 如果无法逃脱，则被将死
        if (currentPlayer == WHITE) {
            cout << "White" << endl;
        } else {
            cout << "Black" << endl;
        }
    }
}


// 检查一个格子(r, c)是否被指定颜色的棋子攻击
bool isSquareAttacked(int r, int c, PieceColor attackerColor, const Board& board) {
    // 检查来自8个方向的直线和斜线攻击 (车、后、象)
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < 8; ++i) {
        for (int k = 1; k < 8; ++k) {
            int nr = r + k * dr[i];
            int nc = c + k * dc[i];
            if (!isValid(nr, nc)) break;
            if (board[nr][nc].type != EMPTY) {
                if (board[nr][nc].color == attackerColor) {
                    PieceType piece = board[nr][nc].type;
                    if (piece == QUEEN) return true; // 后可以从任何方向攻击
                    if (i % 2 == 0) { // 斜线方向
                        if (piece == BISHOP) return true;
                    } else { // 直线方向
                        if (piece == ROOK) return true;
                    }
                }
                break; // 遇到任何棋子都会阻挡
            }
        }
    }

    // 检查来自马的攻击
    int kn_dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int kn_dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    for (int i = 0; i < 8; ++i) {
        int nr = r + kn_dr[i];
        int nc = c + kn_dc[i];
        if (isValid(nr, nc) && board[nr][nc].type == KNIGHT && board[nr][nc].color == attackerColor) {
            return true;
        }
    }

    // 检查来自兵的攻击
    int pawnDir = (attackerColor == WHITE) ? 1 : -1;
    // 白兵从下往上攻击，黑兵从上往下攻击
    if (isValid(r + pawnDir, c - 1) && board[r + pawnDir][c - 1].type == PAWN && board[r + pawnDir][c - 1].color == attackerColor) {
        return true;
    }
    if (isValid(r + pawnDir, c + 1) && board[r + pawnDir][c + 1].type == PAWN && board[r + pawnDir][c + 1].color == attackerColor) {
        return true;
    }

    // 检查来自对方王的攻击
    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (isValid(nr, nc) && board[nr][nc].type == KING && board[nr][nc].color == attackerColor) {
            return true;
        }
    }

    return false;
}


// 尝试移动棋子并检查是否能解除将军状态
bool tryMove(int r1, int c1, int r2, int c2, PieceColor playerColor, const Board& board, PieceType promotionType = EMPTY) {
    Board tempBoard = board;
    Piece movedPiece = tempBoard[r1][c1];

    if (promotionType != EMPTY) {
        movedPiece.type = promotionType;
    }

    tempBoard[r2][c2] = movedPiece;
    tempBoard[r1][c1] = {EMPTY, NONE};

    // 找到王的最新位置
    int kingR = -1, kingC = -1;
    if (movedPiece.type == KING) {
        kingR = r2;
        kingC = c2;
    } else {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (tempBoard[i][j].type == KING && tempBoard[i][j].color == playerColor) {
                    kingR = i;
                    kingC = j;
                    break;
                }
            }
            if (kingR != -1) break;
        }
    }

    // 如果移动后，王不再被对方攻击，则这是一个合法的逃脱走法
    return !isSquareAttacked(kingR, kingC, (playerColor == WHITE ? BLACK : WHITE), tempBoard);
}


// 检查是否存在一个合法的走法来逃脱将军
bool canEscapeCheck(PieceColor playerColor, const Board& board) {
    // 遍历棋盘上所有属于当前玩家的棋子
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c].color == playerColor) {
                Piece piece = board[r][c];
                // 为该棋子生成所有可能的走法并测试
                // 1. 直线和斜线移动 (后、车、象)
                if (piece.type == QUEEN || piece.type == ROOK || piece.type == BISHOP) {
                    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
                    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
                    int startDir = (piece.type == ROOK) ? 3 : 0;
                    int endDir = (piece.type == ROOK) ? 7 : (piece.type == BISHOP ? 3 : 8);
                     if (piece.type == BISHOP) endDir = 8;


                    for (int i = 0; i < 8; ++i) {
                        if (piece.type == ROOK && (i==0 || i==1 || i==2 || i==5 || i==7)) continue;
                         if (piece.type == BISHOP && (i==3 || i==4 || i==6)) continue;

                        for (int k = 1; k < 8; ++k) {
                            int nr = r + k * dr[i];
                            int nc = c + k * dc[i];
                            if (!isValid(nr, nc)) break;
                            if (board[nr][nc].color == playerColor) break; // 被己方棋子挡住
                            if (tryMove(r, c, nr, nc, playerColor, board)) return true;
                            if (board[nr][nc].color != NONE) break; // 遇到对方棋子，吃掉后不能再前进
                        }
                    }
                }
                // 2. 马的移动
                if (piece.type == KNIGHT) {
                    int kn_dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
                    int kn_dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
                    for (int i = 0; i < 8; ++i) {
                        int nr = r + kn_dr[i];
                        int nc = c + kn_dc[i];
                        if (isValid(nr, nc) && board[nr][nc].color != playerColor) {
                            if (tryMove(r, c, nr, nc, playerColor, board)) return true;
                        }
                    }
                }
                // 3. 王的移动
                if (piece.type == KING) {
                    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
                    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
                    for (int i = 0; i < 8; ++i) {
                        int nr = r + dr[i];
                        int nc = c + dc[i];
                        if (isValid(nr, nc) && board[nr][nc].color != playerColor) {
                            if (tryMove(r, c, nr, nc, playerColor, board)) return true;
                        }
                    }
                }
                // 4. 兵的移动
                if (piece.type == PAWN) {
                    int dir = (playerColor == WHITE) ? -1 : 1; // 白兵向上(-r), 黑兵向下(+r)
                    // 向前移动一格
                    if (isValid(r + dir, c) && board[r + dir][c].type == EMPTY) {
                        // 检查是否升变
                        if (r + dir == 0 || r + dir == 7) {
                            if (tryMove(r, c, r + dir, c, playerColor, board, QUEEN)) return true;
                            if (tryMove(r, c, r + dir, c, playerColor, board, ROOK)) return true;
                            if (tryMove(r, c, r + dir, c, playerColor, board, BISHOP)) return true;
                            if (tryMove(r, c, r + dir, c, playerColor, board, KNIGHT)) return true;
                        } else {
                            if (tryMove(r, c, r + dir, c, playerColor, board)) return true;
                        }
                    }
                    // 斜向吃子
                    int captureCols[] = {c - 1, c + 1};
                    for (int nc : captureCols) {
                        if (isValid(r + dir, nc) && board[r + dir][nc].color != playerColor && board[r + dir][nc].color != NONE) {
                             if (r + dir == 0 || r + dir == 7) {
                                if (tryMove(r, c, r + dir, nc, playerColor, board, QUEEN)) return true;
                                if (tryMove(r, c, r + dir, nc, playerColor, board, ROOK)) return true;
                                if (tryMove(r, c, r + dir, nc, playerColor, board, BISHOP)) return true;
                                if (tryMove(r, c, r + dir, nc, playerColor, board, KNIGHT)) return true;
                            } else {
                                if (tryMove(r, c, r + dir, nc, playerColor, board)) return true;
                            }
                        }
                    }
                }
            }
        }
    }
    // 遍历完所有可能的走法，都无法解除将军状态
    return false;
}

int main() {
    // 提高cin/cout效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}
