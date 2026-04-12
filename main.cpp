#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ==============================
// 資料結構
// ==============================
struct Point {
    int x;
    int y;
};

// ==============================
// 工具函式
// ==============================
void printPoints(const vector<Point>& pts, const string& title) {
    cout << title << "\n";
    for (const auto& p : pts) {
        cout << "(" << p.x << "," << p.y << ") ";
    }
    cout << "\n\n";
}

// 判斷 p1 是否被 p2 凌駕
// 若 p2.x >= p1.x 且 p2.y >= p1.y，並且至少一項嚴格大於，則 p1 被 p2 凌駕
bool isDominated(const Point& p1, const Point& p2) {
    // TODO:
    // 依照凌駕定義完成判斷
    return false; // 請修改
}

// ==============================
// 基本情況：n <= 3
// 直接比對所有點之間的凌駕關係
// ==============================
vector<Point> bruteForceMaximal(const vector<Point>& S) {
    vector<Point> result;

    // TODO:
    // 1. 對每個點檢查是否被其他點凌駕
    // 2. 若沒有被任何點凌駕，則加入 result
    // 3. 回傳 result

    return result;
}

// ==============================
// 取得 x 座標的中位數
// 題目只要求依照演算法分割，這裡可直接排序後取中間位置
// ==============================
int getMedianX(const vector<Point>& S) {
    vector<int> xs;
    for (const auto& p : S) {
        xs.push_back(p.x);
    }

    // TODO:
    // 1. 將 xs 排序
    // 2. 回傳中位數
    //    可直接取 xs[xs.size()/2]
    return 0; // 請修改
}

// ==============================
// 2D_Maximal 遞迴演算法
// ==============================
vector<Point> maximalPoints(const vector<Point>& S) {
    int n = S.size();

    // Base case
    if (n <= 3) {
        return bruteForceMaximal(S);
    }

    // Step 1: 找 x 座標中位數
    int medianX = getMedianX(S);

    // Step 2: 分割成 S_L 與 S_R
    vector<Point> SL, SR;

    // TODO:
    // 將所有 x <= medianX 的點放入 SL
    // 將所有 x >  medianX 的點放入 SR

    // Step 3: 遞迴求左右兩側極點
    vector<Point> leftMaximal = maximalPoints(SL);
    vector<Point> rightMaximal = maximalPoints(SR);

    // Step 4: 找出 S_R 極點中 y 最大值
    int ymax = -1;

    // TODO:
    // 從 rightMaximal 中找出最大的 y 值
    // 若 rightMaximal 為空，要小心處理

    // Step 5: 刪除 S_L 中 y < ymax 的點
    vector<Point> filteredLeft;

    // TODO:
    // 對 leftMaximal 中每一點：
    // 若其 y >= ymax，保留
    // 否則刪除
    //
    // 若 rightMaximal 為空，則 leftMaximal 可全部保留

    // Step 6: 合併結果
    vector<Point> result;

    // TODO:
    // 將 filteredLeft 與 rightMaximal 合併到 result

    return result;
}

// ==============================
// 主程式
// ==============================
int main() {
    // 測資 1
    vector<Point> case1 = {
        {5,12}, {12,10}, {2,9}, {5,7}, {10,6},
        {9,4}, {6,3}, {11,4}, {1,3}, {6,1}
    };

    // 測資 2
    vector<Point> case2 = {
        {6,0}, {11,3}, {10,9}, {8,5}, {9,7}, {6,10},
        {4,7}, {5,4}, {3,8}, {2,3}, {2,9}, {9,4}
    };

    printPoints(case1, "Input Case 1:");
    vector<Point> ans1 = maximalPoints(case1);
    printPoints(ans1, "Maximal Points of Case 1:");

    printPoints(case2, "Input Case 2:");
    vector<Point> ans2 = maximalPoints(case2);
    printPoints(ans2, "Maximal Points of Case 2:");

    return 0;
}
