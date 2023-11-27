#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "subject";

// 構造体
// 座標とスピード
typedef struct Vector2 {
	int x;
	int y;
} Vector2;

typedef struct Vector2f {
	float x;
	float y;
} Vector2f;

typedef struct Ball {
	Vector2f velocity = {4.0f, 4.0f};
	Vector2f acceleration = {0.0f, 1.0f};
} Ball;

Ball ball;

// player座標
Vector2 playerPos = {64, 64};
Vector2 enemyPos = {0, 0};

// playerのスクロール座標
// 背景のスクロール
Vector2 scroll;

// 当たり判定
typedef struct horn {
	// 左上
	int leftTopX;
	int leftTopY;
	// 左下
	int leftBottomX;
	int leftBottomY;
	// 右上
	int rightTopX;
	int rightTopY;
	// 右下
	int rightBottomX;
	int rightBottomY;

} horn;

// 敵の当たり判定
typedef struct EnemyHit {
	// 左上
	int leftTopX;
	int leftTopY;
	// 左下
	int leftBottomX;
	int leftBottomY;
	// 右上
	int rightTopX;
	int rightTopY;
	// 右下
	int rightBottomX;
	int rightBottomY;
} EnemyHit;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	// ウィンドウサイズ
	const int screenWidth = 2560;
	const int screenHeight = 1568;
	Novice::Initialize(kWindowTitle, screenWidth, screenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 絵
	// タイトル
	int titleTexture = Novice::LoadTexture("./Resources./Title.png");
	int falseStageYesJump = Novice::LoadTexture("./Resources./falseYesJamp.png");
	int trueStageYesJump = Novice::LoadTexture("./Resources./trueYesJamp.png");
	// ブロック
	//  ただのブロック
	int nomalBlock = Novice::LoadTexture("./Resources./block.png");
	// 足し算
	int add1Texture = Novice::LoadTexture("./Resources./p1.png");
	int add2Texture = Novice::LoadTexture("./Resources./p2.png");
	int add3Texture = Novice::LoadTexture("./Resources./p3.png");
	int add4Texture = Novice::LoadTexture("./Resources./p4.png");
	int add5Texture = Novice::LoadTexture("./Resources./p5.png");
	// 引き算
	int del1Texture = Novice::LoadTexture("./Resources./m1.png");
	int del2Texture = Novice::LoadTexture("./Resources./m2.png");
	int del3Texture = Novice::LoadTexture("./Resources./m3.png");
	int del4Texture = Novice::LoadTexture("./Resources./m4.png");
	int del5Texture = Novice::LoadTexture("./Resources./m5.png");
	// 掛け算
	int mul1Texture = Novice::LoadTexture("./Resources./k1.png");
	int mul2Texture = Novice::LoadTexture("./Resources./k2.png");
	int mul3Texture = Novice::LoadTexture("./Resources./k3.png");
	int mul4Texture = Novice::LoadTexture("./Resources./k4.png");
	int mul5Texture = Novice::LoadTexture("./Resources./k5.png");
	// 割り算
	int div1Texture = Novice::LoadTexture("./Resources./d1.png");
	int div2Texture = Novice::LoadTexture("./Resources./d2.png");
	int div3Texture = Novice::LoadTexture("./Resources./d3.png");
	int div4Texture = Novice::LoadTexture("./Resources./d4.png");
	int div5Texture = Novice::LoadTexture("./Resources./d5.png");
	// マップサイズ
	const int mapX = 50;
	const int mapY = 25;
	// タイルサイズ
	int mapTileSize = 64;
	int playerTileSize = 64;
	int enemyTileSize = 64;

	// マップ
	int map[mapY][mapX] = {
	    {0, 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 17, 1,  28, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 17, 22, 28, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {0, 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    {1, 1, 1, 1,  1,  1,  1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

	// ステージ
	enum STAGE { TITLE, YESJAMP, MENU };
	STAGE stage = TITLE;
	// ブロック
	enum BLOCK {

		// ブロックなし
		NOBLOCK, // 0
		// ただのブロック
		NOMALBLOCK, // 1
		// 敵
		ENEMY1,  // 2
		ENEMY2,  // 3
		ENEMY3,  // 4
		ENEMY4,  // 5
		ENEMY5,  // 6
		ENEMY6,  // 7
		ENEMY7,  // 8
		ENEMY8,  // 9
		ENEMY9,  // 10
		ENEMY10, // 11
		ENEMY11, // 12
		ENEMY12, // 13
		ENEMY13, // 14
		ENEMY14, // 15
		ENEMY15, // 16
		// 加算ブロック
		ADD1, //
		ADD2, //
		ADD3, //
		ADD4, //
		ADD5, //
		// 減算ブロック
		DEL1, //
		DEL2, //
		DEL3, //
		DEL4, //
		DEL5, //
		// 掛け算ブロック
		MUL1, //
		MUL2, //
		MUL3, //
		MUL4, //
		MUL5, //
		// 割り算ブロック
		DIV1, //
		DIV2, //
		DIV3, //
		DIV4, //
		DIV5  //
	};

	// スクロール最大・最小値
	// X
	int scrollMinY = 0;
	int scrollMaxY = 848;
	// Y
	int scrollMinX = 0;
	int scrollMaxX = 3072;

	// 移動する前にいる座標の変数
	int oldScrollX = scroll.x;
	int oldScrollY = scroll.y;

	int nowMapX = mapX;
	int nowMapY = mapY;

	horn player = {
	    // 左上
	    (playerPos.x / playerTileSize),
	    (playerPos.y / playerTileSize),
	    // 左下
	    (playerPos.x - playerTileSize) / mapTileSize,
	    (playerPos.y + playerTileSize - 1) / mapTileSize,
	    // 右上
	    (playerPos.x + playerTileSize - 1) / mapTileSize,
	    (playerPos.y - playerTileSize) / mapTileSize,
	    // 右下
	    (playerPos.x + playerTileSize - 1) / mapTileSize,
	    (playerPos.y + playerTileSize - 1) / mapTileSize,
	};

	EnemyHit enemyHit = {
	    // 左上
	    (enemyPos.x / enemyTileSize),
	    (enemyPos.y / enemyTileSize),
	    // 左下
	    (enemyPos.x - enemyTileSize) / mapTileSize,
	    (enemyPos.y + enemyTileSize - 1) / mapTileSize,
	    // 右上
	    (enemyPos.x + enemyTileSize - 1) / mapTileSize,
	    (enemyPos.y - enemyTileSize) / mapTileSize,
	    // 右下
	    (enemyPos.x + enemyTileSize - 1) / mapTileSize,
	    (enemyPos.y + enemyTileSize - 1) / mapTileSize,
	};

	// ↓playerの持ってる数字変数
	int stockPlayerNumber = 0;

	// ↓演算ブロックの持っている数字
	// 足し算
	int addNumber1 = 1;
	int addNumber2 = 2;
	int addNumber3 = 3;
	int addNumber4 = 4;
	int addNumber5 = 5;
	// 引き算
	int delNumber1 = 1;
	int delNumber2 = 2;
	int delNumber3 = 3;
	int delNumber4 = 4;
	int delNumber5 = 5;
	// 掛け算
	int mulNumber1 = 1;
	int mulNumber2 = 2;
	int mulNumber3 = 3;
	int mulNumber4 = 4;
	int mulNumber5 = 5;
	// 割り算
	int divNumber1 = 1;
	int divNumber2 = 2;
	int divNumber3 = 3;
	int divNumber4 = 4;
	int divNumber5 = 5;

	// ステージ選択
	bool isYesJamp = false;
	// ジャンプするかしないか
	bool isJamp = false;
	// player死亡フラグ
	int playerHit = 0;
	bool isPlayerHit = false;
	// クールタイム
	bool isPlayerHitTime = false;
	int playerHitTimeStartFrame = 0;
	int playerHitTimeEndFrame = 120;
	// 敵の当たり判定
	bool isEnemy = true;
	//
	bool isEnemyBlock = true;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		switch (stage) {

#pragma region TITLE
		case TITLE:
			if (keys[DIK_UP] || keys[DIK_W]) {
				isYesJamp = true;
			}
			if (isYesJamp == true && keys[DIK_RETURN]) {
				stage = YESJAMP;
			}
			if (keys[DIK_DOWN] || keys[DIK_S]) {
				isYesJamp = false;
			}
			break;
#pragma endregion

#pragma region YESJAMP
			// ジャンプアリ
		case YESJAMP:
			// playerの処理
#pragma region player
			// スクロール止める
			oldScrollX = scroll.x;
			oldScrollY = scroll.y;
			// 角の判定
			player.leftTopX = (playerPos.x) / mapTileSize;
			player.leftTopY = (playerPos.y) / mapTileSize;
			player.leftBottomX = (playerPos.x) / mapTileSize;
			player.leftBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
			player.rightTopX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			player.rightTopY = (playerPos.y) / mapTileSize;
			player.rightBottomX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			player.rightBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
			// ジャンプ
			// スペースでジャンプ
			if ((keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) && isJamp == false) {
				isJamp = true;
				ball.velocity.y = -25.0f;
			}
			// 速度に加速度加算
			ball.velocity.y += ball.acceleration.y;

			// 角の当たり判定
			player.leftTopX = (playerPos.x) / mapTileSize;
			player.leftTopY = (playerPos.y + int(ball.velocity.y)) / mapTileSize;
			player.leftBottomX = (playerPos.x) / mapTileSize;
			player.leftBottomY =
			    (playerPos.y + playerTileSize - 1 + int(ball.velocity.y)) / mapTileSize;
			player.rightTopX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			player.rightTopY = (playerPos.y + int(ball.velocity.y)) / mapTileSize;
			player.rightBottomX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			player.rightBottomY =
			    (playerPos.y + playerTileSize - 1 + int(ball.velocity.y)) / mapTileSize;
			// ジャンプしたときに地面にのめりこむかのめりこまないか
			if (map[player.leftTopY][player.leftTopX] == NOMALBLOCK ||
			    map[player.rightTopY][player.rightTopX] == NOMALBLOCK) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
			}
			// 自由落下したときに地面にのめりこむかのめりこまないか
			if (map[player.leftBottomY][player.leftBottomX] == NOMALBLOCK ||
			    map[player.rightBottomY][player.rightBottomX] == NOMALBLOCK) {
				isJamp = false;
				ball.velocity.y = 0;
				playerPos.y = (player.leftBottomY - 1) * mapTileSize;
			}

			// 足し算
			//  ジャンプしたとき足し算ブロックだったらブロックに書かれている数字分足す
			// 1
			//
			if (map[player.leftTopY][player.leftTopX] == ADD1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber1;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == ADD1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber1;
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 2
			//
			if (map[player.leftTopY][player.leftTopX] == ADD2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber2;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == ADD2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber2;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 3
			//
			if (map[player.leftTopY][player.leftTopX] == ADD3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber3;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == ADD3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber3;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 4
			//
			if (map[player.leftTopY][player.leftTopX] == ADD4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber4;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == ADD4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber4;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 5
			//
			if (map[player.leftTopY][player.leftTopX] == ADD5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber5;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == ADD5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber + addNumber5;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 引き算
			//  ジャンプしたとき引き算ブロックだったらブロックに書かれている数字分引く
			// 1
			//
			if (map[player.leftTopY][player.leftTopX] == DEL1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber1;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == DEL1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber1;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 2
			//
			if (map[player.leftTopY][player.leftTopX] == DEL2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber2;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == DEL2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber2;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 3
			//
			if (map[player.leftTopY][player.leftTopX] == DEL3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber3;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == DEL3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber3;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 4
			//
			if (map[player.leftTopY][player.leftTopX] == DEL4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber4;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == DEL4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber4;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 5
			//
			if (map[player.leftTopY][player.leftTopX] == DEL5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber5;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == DEL5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber - delNumber5;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 掛け算
			// ジャンプしたとき掛け算ブロックだったらブロックに書かれている数字分掛け算する
			// 1
			//
			if (map[player.leftTopY][player.leftTopX] == MUL1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber1;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == MUL1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber1;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 2
			//
			if (map[player.leftTopY][player.leftTopX] == MUL2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber2;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == MUL2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber2;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 3
			//
			if (map[player.leftTopY][player.leftTopX] == MUL3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber3;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == MUL3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber3;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 4
			//
			if (map[player.leftTopY][player.leftTopX] == MUL4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber4;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == MUL4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber4;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 5
			//
			if (map[player.leftTopY][player.leftTopX] == MUL5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber5;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == MUL5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber * mulNumber5;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 割り算
			// 1
			//
			if (map[player.leftTopY][player.leftTopX] == DIV1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber1;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			//
			if (map[player.rightTopY][player.rightTopX] == DIV1) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber1;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 2
			//
			if (map[player.leftTopY][player.leftTopX] == DIV2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber2;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			if (map[player.rightTopY][player.rightTopX] == DIV2) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber2;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 3
			//
			if (map[player.leftTopY][player.leftTopX] == DIV3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber3;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			if (map[player.rightTopY][player.rightTopX] == DIV3) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber3;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 4
			//
			if (map[player.leftTopY][player.leftTopX] == DIV4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber4;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			if (map[player.rightTopY][player.rightTopX] == DIV4) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber4;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}
			// 5
			//
			if (map[player.leftTopY][player.leftTopX] == DIV5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber5;
				// ブロック戻す
				map[player.leftTopY][player.leftTopX] = NOMALBLOCK;
			}
			if (map[player.rightTopY][player.rightTopX] == DIV5) {
				ball.velocity.y = 0;
				playerPos.y = (player.leftTopY + 1) * mapTileSize;
				stockPlayerNumber = stockPlayerNumber / divNumber5;
				// ブロック戻す
				map[player.rightTopY][player.rightTopX] = NOMALBLOCK;
			}

			// 自由落下したときに地面にのめりこむかのめりこまないか
			if (map[player.leftBottomY][player.leftBottomX] == NOMALBLOCK ||
			    map[player.rightBottomY][player.rightBottomX] == NOMALBLOCK ||
			    map[player.leftBottomY][player.leftBottomX] == ADD1 ||
			    map[player.rightBottomY][player.rightBottomX] == ADD1 ||
			    map[player.leftBottomY][player.leftBottomX] == ADD2 ||
			    map[player.rightBottomY][player.rightBottomX] == ADD2 ||
			    map[player.leftBottomY][player.leftBottomX] == ADD3 ||
			    map[player.rightBottomY][player.rightBottomX] == ADD3 ||
			    map[player.leftBottomY][player.leftBottomX] == ADD4 ||
			    map[player.rightBottomY][player.rightBottomX] == ADD4 ||
			    map[player.leftBottomY][player.leftBottomX] == ADD5 ||
			    map[player.rightBottomY][player.rightBottomX] == ADD5 ||
			    map[player.leftBottomY][player.leftBottomX] == DEL1 ||
			    map[player.rightBottomY][player.rightBottomX] == DEL1 ||
			    map[player.leftBottomY][player.leftBottomX] == DEL2 ||
			    map[player.rightBottomY][player.rightBottomX] == DEL2 ||
			    map[player.leftBottomY][player.leftBottomX] == DEL3 ||
			    map[player.rightBottomY][player.rightBottomX] == DEL3 ||
			    map[player.leftBottomY][player.leftBottomX] == DEL4 ||
			    map[player.rightBottomY][player.rightBottomX] == DEL4 ||
			    map[player.leftBottomY][player.leftBottomX] == DEL5 ||
			    map[player.rightBottomY][player.rightBottomX] == DEL5 ||
			    map[player.leftBottomY][player.leftBottomX] == MUL1 ||
			    map[player.rightBottomY][player.rightBottomX] == MUL1 ||
			    map[player.leftBottomY][player.leftBottomX] == MUL2 ||
			    map[player.rightBottomY][player.rightBottomX] == MUL2 ||
			    map[player.leftBottomY][player.leftBottomX] == MUL3 ||
			    map[player.rightBottomY][player.rightBottomX] == MUL3 ||
			    map[player.leftBottomY][player.leftBottomX] == MUL4 ||
			    map[player.rightBottomY][player.rightBottomX] == MUL4 ||
			    map[player.leftBottomY][player.leftBottomX] == MUL5 ||
			    map[player.rightBottomY][player.rightBottomX] == MUL5 ||
			    map[player.leftBottomY][player.leftBottomX] == DIV1 ||
			    map[player.rightBottomY][player.rightBottomX] == DIV1 ||
			    map[player.leftBottomY][player.leftBottomX] == DIV2 ||
			    map[player.rightBottomY][player.rightBottomX] == DIV2 ||
			    map[player.leftBottomY][player.leftBottomX] == DIV3 ||
			    map[player.rightBottomY][player.rightBottomX] == DIV3 ||
			    map[player.leftBottomY][player.leftBottomX] == DIV4 ||
			    map[player.rightBottomY][player.rightBottomX] == DIV4 ||
			    map[player.leftBottomY][player.leftBottomX] == DIV5 ||
			    map[player.rightBottomY][player.rightBottomX] == DIV5) {
				isJamp = false;
				ball.velocity.y = 0;
				playerPos.y = (player.leftBottomY - 1) * mapTileSize;
			}

			// 自由落下中のスクロール
			// スクロールする
			scroll.y = playerPos.y - screenHeight / 2;

			if (scroll.y < scrollMinY) {
				scroll.y = scrollMinY;
			}
			if (scroll.y > scrollMaxY) {
				scroll.y = scrollMaxY;
			}

			// Yに速度加算
			playerPos.y += int(ball.velocity.y);

			// マップ座標更新
			nowMapX = mapX;
			nowMapY = mapY;
			// スクロール止める
			oldScrollX = scroll.x;
			oldScrollY = scroll.y;

			// 角の当たり判定
			player.leftTopX = (playerPos.x) / mapTileSize;
			player.leftTopY = (playerPos.y + int(ball.velocity.y)) / mapTileSize;
			player.leftBottomX = (playerPos.x) / mapTileSize;
			player.leftBottomY =
			    (playerPos.y + playerTileSize - 1 + int(ball.velocity.y)) / mapTileSize;
			player.rightTopX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			player.rightTopY = (playerPos.y + int(ball.velocity.y)) / mapTileSize;
			player.rightBottomX = (playerPos.x + playerTileSize - 1) / mapTileSize;
			player.rightBottomY =
			    (playerPos.y + playerTileSize - 1 + int(ball.velocity.y)) / mapTileSize;

			// キー入力
			//  左
			if ((keys[DIK_LEFT] || keys[DIK_A])) {
				// 左角の判定
				player.leftTopX = (playerPos.x - int(ball.velocity.x)) / mapTileSize;
				player.leftTopY = (playerPos.y) / mapTileSize;
				player.leftBottomX = (playerPos.x - int(ball.velocity.x)) / mapTileSize;
				player.leftBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
				// 進む先がブロックじゃなかったら左にスピード分進む
				if (map[player.leftTopY][player.leftTopX] == NOBLOCK &&
				    map[player.leftBottomY][player.leftBottomX] == NOBLOCK) {
					playerPos.x -= int(ball.velocity.x);
					// 左上か左下がブロックだったらスクロールが止まる
					if (map[player.leftTopY][player.leftTopX] == NOMALBLOCK ||
					    map[player.leftBottomY][player.leftBottomX] == NOMALBLOCK) {
						scroll.x = oldScrollX;
					}
				}
			}

			// 右
			if ((keys[DIK_RIGHT] || keys[DIK_D])) {
				// 右角の判定
				player.rightTopX =
				    (playerPos.x + playerTileSize - 1 + int(ball.velocity.x)) / mapTileSize;
				player.rightTopY = (playerPos.y) / mapTileSize;
				player.rightBottomX =
				    (playerPos.x + playerTileSize - 1 + int(ball.velocity.x)) / mapTileSize;
				player.rightBottomY = (playerPos.y + playerTileSize - 1) / mapTileSize;
				// 敵とplayerの当たり判定
				if (isPlayerHitTime == false && isEnemyBlock == true) {
					if (map[player.rightTopY][player.rightTopX] == ENEMY1 ||
					    map[player.rightBottomY][player.rightBottomX] == ENEMY1) {
						isPlayerHitTime = true;
						isPlayerHit = true;
					}
				}
				// 敵が倒された後右上の当たり判定をブロックがない判定にする
				if (map[player.rightTopY][player.rightTopX] == ENEMY1) {
					map[player.rightTopY][player.rightTopX] = NOBLOCK;
				}
				// 進む先がブロックじゃなかったら右にスピード分進む
				if (map[player.rightTopY][player.rightTopX] == NOBLOCK &&
				    map[player.rightBottomY][player.rightBottomX] == NOBLOCK) {
					playerPos.x += int(ball.velocity.x);
					// スクロールする
					if (scrollMinX < playerPos.x || playerPos.x > scrollMaxX) {
						scroll.x = playerPos.x + screenWidth / 2;
					}
					// 右上か右下がブロックだったらスクロールが止まる
					if (map[player.rightTopY][player.rightTopX] == NOMALBLOCK ||
					    map[player.rightBottomY][player.rightBottomX] == NOMALBLOCK) {
						scroll.x = oldScrollX;
					}
				}
			}
			// スクロールする
			scroll.x = playerPos.x - screenWidth / 2;
			// スクロール止まる
			if (scroll.x < scrollMinX) {
				scroll.x = scrollMinX;
			}
			// スクロール止まる
			if (scroll.x > scrollMaxX) {
				scroll.x = scrollMaxX;
			}

#pragma endregion
			// 敵の処理
#pragma region enemy
			// 敵の当たり判定
			if (isEnemy == true) {
				enemyHit.leftTopX = enemyPos.x / mapTileSize;
				enemyHit.leftTopY = enemyPos.y / mapTileSize;
				enemyHit.leftBottomX = enemyPos.x / mapTileSize;
				enemyHit.leftBottomY = (enemyPos.y + enemyTileSize - 1) / mapTileSize;
				enemyHit.rightTopX = (enemyPos.x + enemyTileSize - 1);
				enemyHit.rightTopY = enemyPos.y / mapTileSize;
				enemyHit.rightBottomX = (enemyPos.x + enemyTileSize - 1) / mapTileSize;
				enemyHit.rightBottomY = (enemyPos.y + enemyTileSize - 1) / mapTileSize;
				if (map[player.rightBottomY][player.rightBottomX] == ENEMY1 &&
				    map[player.leftBottomY][player.leftBottomX] == ENEMY1) {
					isEnemy = false;
				}
			}

			if (isPlayerHitTime == true) {
				playerHitTimeStartFrame++;
			}
			if (playerHitTimeStartFrame == playerHitTimeEndFrame) {
				isPlayerHitTime = false;
				playerHitTimeStartFrame = 0;
			}
			if (isPlayerHit == true) {
				playerHit = playerHit + 1;
				isPlayerHit = false;
			}

			if (isEnemy == false) {
				isEnemyBlock = false;
				map[enemyPos.y][enemyPos.x] = NOBLOCK;
			}

#pragma endregion

			break;
#pragma endregion

			// シーン終了
			break;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (stage) {
#pragma region Title
			// タイトル
		case TITLE:
			Novice::DrawSprite(0, 0, titleTexture, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(0, 0, titleTexture, 1.0f, 1.0f, 0.0f, WHITE);
			if (isYesJamp == false) {
				Novice::DrawSprite(0, 500, falseStageYesJump, 1.0f, 1.0f, 0.0f, WHITE);
			}
			if (isYesJamp == true) {
				Novice::DrawSprite(0, 500, trueStageYesJump, 1.0f, 1.0f, 0.0f, WHITE);
			}
			break;
#pragma endregion

#pragma region YESTITLE
			// ジャンプあり
		case YESJAMP:
			for (int y = 0; y < mapY; y++) {
				for (int x = 0; x < mapX; x++) {
					if (playerPos.x + screenWidth > screenWidth ||
					    playerPos.y + screenHeight > screenHeight) {
						switch (map[y][x]) {
							///////////////// ブロックなし
						case NOBLOCK:
							break;
							//////////////// ただのブロック
						case NOMALBLOCK:
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    nomalBlock, 1, 1, 0.0f, WHITE);
							break;
							/////////////// 敵
						case ENEMY1:
							// 生きてるとき
							if (isEnemyBlock == true) {
								Novice::DrawBox(
								    (x * enemyTileSize) - scroll.x, (y * enemyTileSize) - scroll.y,
								    enemyTileSize, enemyTileSize, 0.0f, GREEN, kFillModeSolid);
							}
							// 死んだとき
							if (isEnemyBlock == false) {
								Novice::DrawBox(
								    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
								    mapTileSize, mapTileSize, 0.0f, BLACK, kFillModeSolid);
							}
							break;
							//////////// 足し算
						case ADD1: // 1
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    add1Texture, 1, 1, 0.0f, WHITE);
							break;
						case ADD2: // 2
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    add2Texture, 1, 1, 0.0f, WHITE);
							break;
						case ADD3: // 3
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    add3Texture, 1, 1, 0.0f, WHITE);
							break;
						case ADD4: // 4
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    add4Texture, 1, 1, 0.0f, WHITE);
							break;
						case ADD5: // 5
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    add5Texture, 1, 1, 0.0f, WHITE);
							break;
							/////////// 引き算
						case DEL1: // 1
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    del1Texture, 1, 1, 0.0f, WHITE);
							break;
						case DEL2: // 2
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    del2Texture, 1, 1, 0.0f, WHITE);
							break;
						case DEL3: // 3
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    del3Texture, 1, 1, 0.0f, WHITE);
							break;
						case DEL4: // 4
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    del4Texture, 1, 1, 0.0f, WHITE);
							break;
						case DEL5: // 5
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    del5Texture, 1, 1, 0.0f, WHITE);
							break;
							//////////// 掛け算
						case MUL1: // 1
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    mul1Texture, 1, 1, 0.0f, WHITE);
							break;
						case MUL2: // 2
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    mul2Texture, 1, 1, 0.0f, WHITE);
							break;
						case MUL3: // 3
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    mul3Texture, 1, 1, 0.0f, WHITE);
							break;
						case MUL4: // 4
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    mul4Texture, 1, 1, 0.0f, WHITE);
							break;
						case MUL5: // 5
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    mul5Texture, 1, 1, 0.0f, WHITE);
							break;
							////////////// 割り算
						case DIV1: // 1
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    div1Texture, 1, 1, 0.0f, WHITE);
							break;
						case DIV2: // 2
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    div2Texture, 1, 1, 0.0f, WHITE);
							break;
						case DIV3: // 3
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    div3Texture, 1, 1, 0.0f, WHITE);
							break;
						case DIV4: // 4
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    div4Texture, 1, 1, 0.0f, WHITE);
							break;
						case DIV5: // 5
							Novice::DrawSprite(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    div5Texture, 1, 1, 0.0f, WHITE);
							break;
						}
					}
				}
			}
			// player表記
			Novice::DrawBox(
			    (playerPos.x) - scroll.x, (int)(playerPos.y) - scroll.y, playerTileSize,
			    playerTileSize, 0.0f, BLUE, kFillModeSolid);
			//
			Novice::ScreenPrintf(0, 0, "playerHit %d", playerHit);
			//
			Novice::ScreenPrintf(0, 50, "playerHitTimeStartFrame %d", playerHitTimeStartFrame);
			Novice::ScreenPrintf(0, 100, "playerHitTimeEndFrame %d", playerHitTimeEndFrame);

			Novice::ScreenPrintf(0, 150, "isPlayerHitTime %d", isPlayerHitTime);

			Novice::ScreenPrintf(0, 200, "%d", stockPlayerNumber);

			break;
#pragma endregion

			// シーン終了
			break;
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
