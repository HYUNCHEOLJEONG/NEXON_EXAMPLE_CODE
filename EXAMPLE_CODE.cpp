#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

const int ROWS = 10;
const int COLS = 17;

int board[ROWS][COLS];
int owner[ROWS][COLS]; // 0: 없음, 1: 우리, 2: 상대
bool isFirst;
int myScore = 0, oppScore = 0;

// 네 변에 하나씩 숫자가 있는지 체크
bool isValid(int r1, int c1, int r2, int c2) {
    int sum = 0;
    bool top = false, bottom = false, left = false, right = false;

    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            if (board[r][c] > 0) {
                sum += board[r][c];
                if (r == r1) top = true;
                if (r == r2) bottom = true;
                if (c == c1) left = true;
                if (c == c2) right = true;
            }
        }
    }

    return (sum == 10) && top && bottom && left && right;
}

// 가능한 직사각형 중 첫 번째 반환
tuple<int, int, int, int> findBestRectangle() {
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int c1 = 0; c1 < COLS; ++c1) {
            for (int r2 = r1; r2 < ROWS; ++r2) {
                for (int c2 = c1; c2 < COLS; ++c2) {
                    if (isValid(r1, c1, r2, c2)) {
                        return {r1, c1, r2, c2};
                    }
                }
            }
        }
    }
    return {-1, -1, -1, -1}; // 패스
}

// 점령 수행
void occupy(int r1, int c1, int r2, int c2, int who) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            if (board[r][c] > 0) {
                board[r][c] = 0;
                if (owner[r][c] != who) {
                    owner[r][c] = who;
                    if (who == 1) myScore++;
                    else oppScore++;
                }
            }
        }
    }
}

// 상대방 선택 반영
void applyOpponentMove(int r1, int c1, int r2, int c2) {
    if (r1 != -1) occupy(r1, c1, r2, c2, 2);
}

// 내 턴 수행
void doTurn() {
    auto [r1, c1, r2, c2] = findBestRectangle();

    if (r1 != -1) {
        occupy(r1, c1, r2, c2, 1);
    }

    cout << r1 << " " << c1 << " " << r2 << " " << c2 << endl;
    cout.flush();
}

// 메인 루프
int main() {
    string cmd;
    while (cin >> cmd) {
        if (cmd == "READY") {
            string turn;
            cin >> turn;
            isFirst = (turn == "FIRST");
            cout << "OK" << endl;
            cout.flush();
        }
        else if (cmd == "INIT") {
            for (int r = 0; r < ROWS; ++r) {
                string line;
                cin >> line;
                for (int c = 0; c < COLS; ++c) {
                    board[r][c] = line[c] - '0';
                    owner[r][c] = 0;
                }
            }
        }
        else if (cmd == "OPP") {
            int r1, c1, r2, c2, t;
            cin >> r1 >> c1 >> r2 >> c2 >> t;
            applyOpponentMove(r1, c1, r2, c2);
        }
        else if (cmd == "TIME") {
            int myTime, oppTime;
            cin >> myTime >> oppTime;
            doTurn(); // 출력 포함
        }
        else if (cmd == "FINISH") {
            break;
        }
    }
    return 0;
}
