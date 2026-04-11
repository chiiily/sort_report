import matplotlib.pyplot as plt
import numpy as np

# ==========================================
# 1. 實驗數據
# ==========================================
# 根據 C++ 實驗輸出的數據
sizes = [1000, 2000, 5000, 10000, 20000, 50000, 100000]
algorithms = {
    "氣泡排序 (Bubble Sort)": [0.0162, 0.0443, 0.2357, 0.9219, 3.7472, 23.4817, 109.4277],
    "選擇排序 (Selection Sort)": [0.0038, 0.0112, 0.0699, 0.2775, 1.1335, 7.1583, 57.1007],
    "插入排序 (Insertion Sort)": [0.0054, 0.0185, 0.1185, 0.4956, 1.9958, 12.3386, 83.4008],
    "合併排序 (Merge Sort)": [0.0002, 0.0004, 0.0010, 0.0021, 0.0045, 0.0125, 0.0501],
    "快速排序 (Quick Sort)": [0.0001, 0.0002, 0.0005, 0.0011, 0.0021, 0.0059, 0.0239]
}

# ==========================================
# 2. Matplotlib 繪圖設定
# ==========================================
# 設定圖表大小
plt.figure(figsize=(12, 8))

# 處理中文字體顯示問題
# 請確保你的系統中已安裝 'Microsoft JhengHei' 字體，或替換為其他支援中文的字體
plt.rcParams['font.sans-serif'] = ['Microsoft JhengHei']
# 嘗試使用一個常見的 Windows 中文字體列表，以增加成功率
# 如果 'Microsoft JhengHei' (微軟正黑體) 不可用，它會嘗試 'SimHei' (黑體)
plt.rcParams['font.sans-serif'] = ['Microsoft JhengHei', 'SimHei']
plt.rcParams['axes.unicode_minus'] = False  # 解決負號顯示問題

# ==========================================
# 3. 繪製折線圖
# ==========================================
# 遍歷每種演算法的數據並繪製
for name, times in algorithms.items():
    # 處理 log(0) 的問題：將 0 替換為 None，使其在圖上不顯示該數據點
    # 這樣可以避免對數尺度錯誤，同時真實反映 0 秒的情況
    plot_times = [t if t > 0 else None for t in times]
    plt.plot(sizes, plot_times, marker='o', linestyle='-', label=name)

# ==========================================
# 4. 設定圖表標題、軸標籤與圖例
# ==========================================
plt.title('排序演算法效能比較 (最差情況：完全逆序)', fontsize=18, pad=20)
plt.xlabel('陣列規模 (N) - 對數尺度', fontsize=14)
plt.ylabel('執行時間 (秒) - 對數尺度', fontsize=14)

# 使用對數尺度 (log scale) 來更好地呈現數量級差異
plt.yscale('log')
plt.xscale('log')

# 加入格線方便對照
plt.grid(True, which="both", ls="--", alpha=0.6)

# 顯示圖例
plt.legend(fontsize=12)

# ==========================================
# 5. 儲存圖檔
# ==========================================
output_filename = 'performance_chart.png'
plt.savefig(output_filename, dpi=300, bbox_inches='tight')

print(f"效能比較圖已成功儲存為 '{output_filename}'")
