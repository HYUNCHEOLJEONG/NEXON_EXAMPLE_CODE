#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

const int ROWS = 10;
const int COLS = 17;

int board[ROWS][COLS];
int owner[ROWS][COLS];
int prefixSum[ROWS + 1][COLS + 1]; // 누적합 배열
bool isFirst; // 내가 선공인지 여부
int myScore = 0, oppScore = 0;

// 누적합 계산 함수
void buildPrefixSum() {
    for (int r = 0; r <= ROWS; ++r)
        for (int c = 0; c <= COLS; ++c)
            prefixSum[r][c] = 0;

    for (int r = 1; r <= ROWS; ++r) {
        for (int c = 1; c <= COLS; ++c) {
            prefixSum[r][c] = board[r-1][c-1] + prefixSum[r-1][c] + prefixSum[r][c-1] - prefixSum[r-1][c-1];
        }
    }
}

/*

prefix[r][c] = board[r-1][c-1]
             + prefix[r-1][c]   // 위쪽 누적합
             + prefix[r][c-1]   // 왼쪽 누적합
             - prefix[r-1][c-1];// 겹친 왼쪽 위는 다시 빼줌
1차원과 다르게 2차원 누적은 뺴줘야함
*/


// 사각형 합을 O(1)로 구하는 함수
int getRectSum(int r1, int c1, int r2, int c2) {
    return prefixSum[r2 + 1][c2 + 1] - prefixSum[r1][c2 + 1] - prefixSum[r2 + 1][c1] + prefixSum[r1][c1];
}

// 조건 검사 함수 (네 변에 숫자가 하나 이상 있는지와 합이 10인지)
bool isValid(int r1, int c1, int r2, int c2) {
    int sum = getRectSum(r1, c1, r2, c2);
    if (sum != 10) return false;

    bool top = false, bottom = false, left = false, right = false;
    for (int c = c1; c <= c2; ++c) {
        if (board[r1][c] > 0) top = true;
        if (board[r2][c] > 0) bottom = true;
    }
    for (int r = r1; r <= r2; ++r) {
        if (board[r][c1] > 0) left = true;
        if (board[r][c2] > 0) right = true;
    }
    return top && bottom && left && right;
}

// 점수 평가 함수 예시
int evaluateRectangle(int r1, int c1, int r2, int c2) {
    int area = (r2 - r1 + 1) * (c2 - c1 + 1);
    int sum = getRectSum(r1, c1, r2, c2);
    return area * 10 + sum; // 넓이 우선, 내부 합도 반영
}

// 후보들 중 최고 점수 사각형을 반환
tuple<int,int,int,int> findBestRectangle() {
    int bestScore = -1;
    tuple<int,int,int,int> bestRect = {-1,-1,-1,-1};
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int c1 = 0; c1 < COLS; ++c1) {
            for (int r2 = r1; r2 < ROWS; ++r2) {
                for (int c2 = c1; c2 < COLS; ++c2) {
                    if (isValid(r1, c1, r2, c2)) {
                        int score = evaluateRectangle(r1, c1, r2, c2);
                        if (score > bestScore) {
                            bestScore = score;
                            bestRect = {r1, c1, r2, c2};
                        }
                    }
                }
            }
        }
    }
    return bestRect;
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
    buildPrefixSum(); // 매 턴마다 보드 상태에 맞게 누적합 갱신
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
            doTurn();
        }
        else if (cmd == "FINISH") {
            break;
        }
    }
    return 0;
}
