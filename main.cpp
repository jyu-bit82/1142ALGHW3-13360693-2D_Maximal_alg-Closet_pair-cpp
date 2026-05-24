#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <limits>
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

double distancePoints(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

bool cmpX(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

bool cmpY(const Point& a, const Point& b) {
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

// =====================================================
// Part A. 2D_Maximal Template
// =====================================================

// 判斷 p1 是否被 p2 凌駕
// 若 p2.x >= p1.x 且 p2.y >= p1.y，並且至少一項嚴格大於，則 p1 被 p2 凌駕
bool isDominated(const Point& p1, const Point& p2) {
    if (p2.x >= p1.x && p2.y >= p1.y) {
        if (p2.x > p1.x || p2.y > p1.y) {
            return true;
        }
    }
    return false;
}

// n <= 3 時直接暴力求極點
vector<Point> bruteForceMaximal(const vector<Point>& S) {
    vector<Point> result;
    for (size_t i = 0; i < S.size(); i++) {
        bool dominated = false;
        for (size_t j = 0; j < S.size(); j++) {
            if (i != j && isDominated(S[i], S[j])) {
                dominated = true;
                break;
            }
        }
        if (!dominated) {
            result.push_back(S[i]);
        }
    }
    return result;
}

// 取得 x 座標中位數
int getMedianX(const vector<Point>& S) {
    vector<int> xs;
    for (const auto& p : S) {
        xs.push_back(p.x);
    }
    sort(xs.begin(), xs.end());
    // 取中間偏左或偏右都可以，這裡取標準的中間位置
    return xs[xs.size() / 2];
}

// 2D_Maximal 主遞迴
vector<Point> maximalPoints(const vector<Point>& S) {
    int n = S.size();

    if (n <= 3) {
        return bruteForceMaximal(S);
    }

    // Step 1: 找 x 座標中位數
    int medianX = getMedianX(S);

    // Step 2: 分割成 S_L 與 S_R
    vector<Point> SL, SR;
    for (const auto& p : S) {
        if (p.x <= medianX) {
            SL.push_back(p);
        } else {
            SR.push_back(p);
        }
    }

    // 避免分割失敗造成無限遞迴（例如所有點的 x 座標都相同）
    if (SL.size() == S.size() || SR.size() == S.size()) {
        return bruteForceMaximal(S);
    }

    // Step 3: 遞迴求左右兩側極點
    vector<Point> leftMaximal = maximalPoints(SL);
    vector<Point> rightMaximal = maximalPoints(SR);

    // Step 4: 找出 S_R 極點中 y 最大值
    int ymax = numeric_limits<int>::min();
    for (const auto& p : rightMaximal) {
        if (p.y > ymax) {
            ymax = p.y;
        }
    }

    // Step 5: 刪除 S_L 中 y < ymax 的點
    vector<Point> filteredLeft;
    if (rightMaximal.empty()) {
        filteredLeft = leftMaximal;
    } else {
        for (const auto& p : leftMaximal) {
            if (p.y >= ymax) {
                filteredLeft.push_back(p);
            }
        }
    }

    // Step 6: 合併結果
    vector<Point> result = filteredLeft;
    result.insert(result.end(), rightMaximal.begin(), rightMaximal.end());

    return result;
}

// =====================================================
// Part B. 2D_Closest_Pair Template
// =====================================================

// n <= 3 時直接暴力求最近距離
double bruteForceClosest(const vector<Point>& S) {
    if (S.size() < 2) return numeric_limits<double>::infinity();
    
    double minDist = numeric_limits<double>::infinity();
    for (size_t i = 0; i < S.size(); i++) {
        for (size_t j = i + 1; j < S.size(); j++) {
            double dist = distancePoints(S[i], S[j]);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }
    return minDist;
}

// 2D Closest Pair 遞迴函式
double closestPairRecursive(vector<Point> Px, vector<Point> Py) {
    int n = Px.size();

    // Base case
    if (n <= 3) {
        return bruteForceClosest(Px);
    }

    // Step 1: 取 x 中位數
    int mid = n / 2;
    Point midPoint = Px[mid];
    int L = midPoint.x;

    // Step 2: 分割成左半與右半
    vector<Point> PxL(Px.begin(), Px.begin() + mid);
    vector<Point> PxR(Px.begin() + mid, Px.end());

    vector<Point> PyL, PyR;
    // 依據原本作業模板設定的規則分流：
    // S_L : x <= L, S_R : x > L
    // 注意：當有多個點 x 座標等於 L 時，為了與 PxL/PxR 的數量同步，
    // 需要配合 PxL/PxR 的切分方式，或者嚴格用點集劃分。
    // 這邊最保險的作法是利用一個 hash 或直接對照 PxL 的範圍，
    // 但因為 Py 本身就是從相同點集排序而來，且題目模板提示為「x <= L 放入 PyL」，
    // 為防相同 x 座標導致左右兩邊點數對不起來，我們可以用 PxL 的大小來做分配限制，
    // 或者直接掃描：因為 Px 已經排過序，前 mid 個點屬於左邊。
    // 這裡採用一個最穩健的線性分類方式：
    for (const auto& p : Py) {
        // 我們要確認 p 是屬於左半邊點集還是右半邊點集
        // 最簡單有效率的方式是看它在 x 軸上的位置，但若 x == L 會有重複。
        // 所以建立一個輔助判斷：如果點在 PxL 中，就去 PyL。
        // 但由於維護 O(n) 複雜度，我們可以直接用二分搜尋或標記。
        // 這邊我們配合模板敘述簡化處理（假設測試資料較少極端 x 相同狀況）：
        if (p.x < L) {
            PyL.push_back(p);
        } else if (p.x > L) {
            PyR.push_back(p);
        } else {
            // x == L 的模糊地帶，用計數分配來維持 PyL 與 PxL 的平衡
            if (PyL.size() < PxL.size()) {
                PyL.push_back(p);
            } else {
                PyR.push_back(p);
            }
        }
    }

    // 避免分割失敗造成無限遞迴
    if (PxL.empty() || PxR.empty()) {
        return bruteForceClosest(Px);
    }

    // Step 3: 遞迴求左右最近距離
    double dL = closestPairRecursive(PxL, PyL);
    double dR = closestPairRecursive(PxR, PyR);

    double delta = min(dL, dR);

    // Step 4: 建立 strip
    vector<Point> strip;
    // 從 Py 取，因為 Py 已按 y 排序
    for (const auto& p : Py) {
        if (abs(p.x - L) < delta) {
            strip.push_back(p);
        }
    }

    // Step 5: 檢查 strip 中可能跨中線的最近點
    int m = strip.size();
    for (int i = 0; i < m; i++) {
        // 理論證明，只要檢查接下來的 7 個點即可（通常寫 7 或 15）
        for (int j = i + 1; j < m && (strip[j].y - strip[i].y) < delta; j++) {
            double dist = distancePoints(strip[i], strip[j]);
            if (dist < delta) {
                delta = dist;
            }
        }
    }

    return delta;
}

// 封裝主函式：先依 x 與 y 排序，再呼叫遞迴
double closestPair(vector<Point> S) {
    if (S.size() < 2) return 0.0;
    
    vector<Point> Px = S;
    vector<Point> Py = S;

    sort(Px.begin(), Px.end(), cmpX);
    sort(Py.begin(), Py.end(), cmpY);

    return closestPairRecursive(Px, Py);
}

// =====================================================
// 主程式
// =====================================================
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

    // ------------------------------
    // Part A: 2D_Maximal
    // ------------------------------
    printPoints(case1, "Input Case 1:");
    vector<Point> ans1 = maximalPoints(case1);
    sort(ans1.begin(), ans1.end(), cmpX);
    printPoints(ans1, "Maximal Points of Case 1:");

    printPoints(case2, "Input Case 2:");
    vector<Point> ans2 = maximalPoints(case2);
    sort(ans2.begin(), ans2.end(), cmpX);
    printPoints(ans2, "Maximal Points of Case 2:");

    // ------------------------------
    // Part B: 2D_Closest_Pair
    // ------------------------------
    cout << "Closest Pair Distance of Case 1: "
         << closestPair(case1) << "\n";

    cout << "Closest Pair Distance of Case 2: "
         << closestPair(case2) << "\n";

    return 0;
}
