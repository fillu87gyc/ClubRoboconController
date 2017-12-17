# ClubRoboconController
----
* これは部内ロボコンで使うプログラムです
* DualShock3を用います
* USBシールドを使っております

### 通信内容

| Left align | Right align | 備考
|:----------:|:------------:|:------------:|
| | 'S'|一個目が正しくSlaveで処理されない事があるのでゴミを投げる|
|0byte |0xEB |実際の通信ヘッダーです |
|1byte	 |   joystick_LX | 0 ~ 100 (中央値:50)|
|2byte   |   joystick_LY | 0 ~ 100 (中央値:50)|
|3byte   | 	 joystick_RX | 0 ~ 100 (中央値:50)|
|4byte   |   joystick_RY | 0 ~ 100 (中央値:50)|
|5byte   |   button1     | bitごとに情報が入ってます。(1が押されている)|
|6byte   |   button2     | button1を参照|
|7byte   |   Sum         | チェックサムです|
|8byte   |   0xAF        | フッター(?)。通信の最後であることを示す|

今回は50±5は誤差として無視します  
また、経年劣化で0,100にならない場合もあるので閾値決め打ちしてます

### ボタンアサイン
#### 5byte目に入るデータ
| Left align | Right align |
|:----------:|:------------:|
 | bit0 | Select |
 | bit1 | L3     |
 | bit2 | R3     |
 | bit3 | Start  |
 | bit4 | Up     |
 | bit5 | Rigth  |
 | bit6 | Down   |
 | bit7 | Left   |

#### 6byte目に入るデータ
 | Left align | Right align |
 |:----------:|:------------:|
 | bit0 | L2     |
 | bit1 | R2     |
 | bit2 | L1     |
 | bit3 | R1     |
 | bit4 | triangle|
 | bit5 | Circle |
 | bit6 | Cross  |
 | bit7 | Square |
