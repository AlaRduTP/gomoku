# 作業報告

- 40747011s 陳映達

## 參考資料

- https://587.renju.org.tw/

其中介紹了五子棋的基礎、術語，如雙活三等等。  
使我更加理解五子棋、有助於設計審局函數。

- https://gomoku.yjyao.com/

線上的五子棋 AI 網站。

- https://uier.github.io/gomoku-web/

同學的五子棋 AI，與我的 AI 對弈、測試。

- https://nodejs.org/api/addons.html

讓 C++ 與 Node.js 共舞的神奇魔法。

## 困難

單用審局函數的 AI，似乎較加上 minmax 的強。

## 測試環境

### 執行程式

解壓縮 `gomoku-darwin-x64-1.0.0.zip` 後執行 `gomoku.app`

### 硬體

- MacBook Pro (13-inch, 2017, Two Thunderbolt 3 ports)
- **處理器** 2.3 GHz 雙核心Intel Core i5
- **記憶體** 16 GB 2133 MHz LPDDR3
- **顯示卡** Intel Iris Plus Graphics 640 1536 MB

### 作業系統

- **MacOS** Catalina 10.15.7

### 聯絡電話

- 0921783206

## 說明

設計上，持黑第一子未限制於外圍。  
若要測試，可不斷隨機重新開局（ `command+A` / `command+H` ），直到落子於外圍為止。

### 介面

程式共有四種模式：

- No Agent
- Human first
- Agent first
- No Human

可以點開上方列表的 `New Game ▶` 選單做選擇，亦有快捷鍵使用。  
搭配清楚簡潔的介面，十分方便。

介面上清楚標示出子的順序，並於左上角提示當前玩家。  
形成連五時亦有特效提示，對使用者十分友善。

棋子上增加了陰影，讓整體盤面看起來更立體、讓使用者更加身歷其境。

### 演算法

因為搭配 minmax 的效果不佳，最後選擇單用審局函數。

也因為如此，雖然持白時難以扭轉情勢，  
但相對的思考時間極短、先手時也有一定實力。

#### 審局函數

對每一個未落子的位置，做「落子後」的分類並給予分數：

- 1 分：死一
- 2 分：活一、死二
- 4 分：活二、死三
- 8 分：活三、死四
- 16 分：活四
- 32 分：五

相關分類介紹可以參考上方提過的五子棋教學網站。

實作上，對於每個點的四個方向上（水平、垂直、左斜、又斜），  
前後各四個位置，以二進位紀錄表示。

例如 `..OXPOO..` 會紀錄為 `0010 1100`。  
（`P` 是要評分的位置，`O` 為我方落子，`X` 為對方落子）

因此可以用 8-bit 代表一種情況，全部 256 種情況紀錄於 `addon/map.txt` 中。  
每一列為一種情況，前 8-bit 紀錄我方落子，後接一個數字 n 代表該情況得 2^n 分。

最後還有一組 8-bit 代表對方落子的遮罩，若與實際情況有部份吻合，  
則該組合為「死」，得分減少為 2^(n - 1) 分。

### 資料結構

記錄盤面使用 17x17 大小的一維 `bool` 陣列，  
`black_board_[]`、`white_board_[]` 分別紀錄黑白子是否落子。  
邊界兩棋盤皆紀錄為 `true`。

### 軟體架構

遵照 MVC 模式，模組化使得軟體本身具有一定的可擴充性。  
例如抽象的 model 模組可以連接資料庫，儲存相關數據。

軟體主要由 Node.js 撰寫，唯 AI Agent 使用 C++ 撰寫插件。  
只要實作以下兩個介面，就可以替換為任何的 Agent。

```c++
static void NewGame(const v8::FunctionCallbackInfo<v8::Value> & args);
static void Play(const v8::FunctionCallbackInfo<v8::Value> & args);
```

- `NewGame` 負責初始化 Agent 的相關資料。
- `Play` 接收一個參數，代表對手的動作，並回傳決定落子的位置。  
  （eg., 接收 `157`，代表對手落子於 `white_board_[157]`）


