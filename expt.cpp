#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono> // 使用 C++11 的高精度計時器
#include <windows.h>

using namespace std;

// ==========================================
// 1. 五種排序演算法實作區
// ==========================================
void bubble_sort(vector<int>& a) {
    int n = a.size();
    for (int k = n - 1; k > 0; k--) {
        bool swapped = false;
        for (int i = 0; i < k; i++) {
            if (a[i] > a[i + 1]) {
                int t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void selection_sort(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            swap(a[i], a[min_idx]);
        }
    }
}

void insertion_sort(vector<int>& a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

// ------------------------------------------
// 優化版合併排序 (依照老師講義實作)
// ------------------------------------------

// 將輔助陣列宣告為全域變數，避免在迴圈中重複配置造成堆疊溢位
vector<int> g_reg;

void merge_sort_recursive(vector<int>& arr, vector<int>& reg, int start, int end) {
    // 不需要排了，返回 (終止條件)
    if (start >= end) return;
    int mid = start + (end - start) / 2; // 計算中點
    int start1 = start, end1 = mid;      // 左半的起點和終點
    int start2 = mid + 1, end2 = end;    // 右半的起點和終點
    // 排左半邊與右半邊
    merge_sort_recursive(arr, reg, start1, end1);
    merge_sort_recursive(arr, reg, start2, end2);
    // 合併部分
    int k = start;
    while (start1 <= end1 && start2 <= end2) {
        reg[k++] = (arr[start1] < arr[start2]) ? arr[start1++] : arr[start2++];
    }
    while (start1 <= end1) {
        reg[k++] = arr[start1++];
    }
    while (start2 <= end2) {
        reg[k++] = arr[start2++];
    }
    // 把 reg[] 複製回 arr[]
    for (k = start; k <= end; k++) {
        arr[k] = reg[k];
    }
}

// 標準的排序函式包裝介面
void merge_sort(vector<int>& arr) {
    int len = arr.size();
    if (g_reg.size() < len) {
        g_reg.resize(len); // 確保全域輔助陣列足夠大
    }
    merge_sort_recursive(arr, g_reg, 0, len - 1);
}

// ------------------------------------------
// 快速排序 (加入取中間值為基準的優化)
// ------------------------------------------
void quick_sort(vector<int>& a, int low, int high) {
    if (low < high) {
        // 優化：在完全逆序的情況下，取中間元素為 Pivot，避免退化為 O(n^2)
        int mid = low + (high - low) / 2;
         swap(a[mid], a[high]); 
        
        int pivot = a[high];
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (a[j] < pivot) {
                i++;
                swap(a[i], a[j]);
            }
        }
        swap(a[i + 1], a[high]);
        int pi = i + 1;

        quick_sort(a, low, pi - 1);
        quick_sort(a, pi + 1, high);
    }
}

// ==========================================
// 2. 實驗測試與計時模組
// ==========================================

// 將主要測試陣列宣告為全域變數，避免在迴圈中重複配置造成堆疊溢位
vector<int> base_array;
vector<int> test_array;

int main() {
    SetConsoleOutputCP(65001);  
    // 依據講義擴充的資料規模
    vector<int> sizes = {1000, 2000, 5000, 10000, 20000, 50000, 100000}; 
    vector<string> algo_names = {"氣泡排序", "選擇排序", "插入排序", "合併排序", "快速排序"};
    
    vector<vector<double>> results(5, vector<double>(sizes.size()));

    cout << "開始進行最差情況（完全逆序）排序效能測試...\n\n";

    for (size_t s_idx = 0; s_idx < sizes.size(); s_idx++) {
        int N = sizes[s_idx];
        // 使用 std::flush 確保即時輸出，避免因程式崩潰而看不到訊息
        cout << "正在測試資料規模 N = " << N << " ...\n" << std::flush;
        
        // 建立完全逆序陣列 (N, N-1, ..., 1)
        base_array.resize(N);
        for (int i = 0; i < N; i++) {
            base_array[i] = N - i;
        }

        for (int algo_idx = 0; algo_idx < 5; algo_idx++) {
            test_array = base_array; // 複製資料到測試陣列
            
            // 使用高精度計時器
            auto start = chrono::high_resolution_clock::now();
            
            if (algo_idx == 0) bubble_sort(test_array);
            else if (algo_idx == 1) selection_sort(test_array);
            else if (algo_idx == 2) insertion_sort(test_array);
            else if (algo_idx == 3) merge_sort(test_array); // 呼叫優化版的合併排序
            else if (algo_idx == 4) quick_sort(test_array, 0, N - 1);

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = end - start;

            results[algo_idx][s_idx] = diff.count();
        }
    }

    // ==========================================
    // 3. 輸出 Markdown 格式表格
    // ==========================================
    cout << "\n\n測試完成！請將以下表格複製到你的 README.md 中：\n\n";
    
    cout << "| 演算法 | ";
    for (int N : sizes) cout << "N=" << N << " | ";
    cout << "\n| :--- | ";
    for (size_t i = 0; i < sizes.size(); i++) cout << ":---: | ";
    cout << "\n";
    
    for (int algo_idx = 0; algo_idx < 5; algo_idx++) {
        cout << "| " << algo_names[algo_idx] << " | ";
        for (size_t s_idx = 0; s_idx < sizes.size(); s_idx++) {
            cout << fixed << setprecision(4) << results[algo_idx][s_idx] << "s | ";
        }
        cout << "\n";
    }

    return 0;
}