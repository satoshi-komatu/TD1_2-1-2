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

// ステージ
enum STAGE { TITLE, YESJAMP, MENU };
STAGE stage = TITLE;
// ブロック
enum BLOCK {

	// ブロックなし
	NOBLOCK,
	// ただのブロック
	NOMALBLOCK,
	// 敵
	//マップ
	ENEMY1,
	ENEMY2,
	// 加算ブロック
	ADD1,
	ADD2,
	ADD3,
	ADD4,
	ADD5,
	// 減算ブロック
	DEL1,
	DEL2,
	DEL3,
	DEL4,
	DEL5,
	// 掛け算ブロック
	MUL1,
	MUL2,
	MUL3,
	MUL4,
	MUL5,
	// 割り算ブロック
	DIV1,
	DIV2,
	DIV3,
	DIV4,
	DIV5
};

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
	int titleTexture = Novice::LoadTexture("./Resources./Title.png");
	int falseStageYesJump = Novice::LoadTexture("./Resources./falseYesJamp.png");
	int trueStageYesJump = Novice::LoadTexture("./Resources./trueYesJamp.png");

	// マップサイズ
	const int mapX = 50;
	const int mapY = 25;
	// タイルサイズ
	int mapTileSize = 64;
	int playerTileSize = 64;
	int enemyTileSize = 64;

	// マップ
	int map[mapY][mapX] = {
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	     1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	    {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
	// int Num = Novice::LoadTexture("./1.1.png");

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
	int stockNumber = 0;

	// ↓演算ブロックの持っている数字
	//足し算
	int addNumber1 = 1;
	int addNumber2 = 2;
	int addNumber3 = 3;
	int addNumber4 = 4;
	int addNumber5 = 5;
	//引き算
	int subNumber1 = 1;
	int subNumber2 = 2;
	int subNumber3 = 3;
	int subNumber4 = 4;
	int subNumber5 = 5;
	//掛け算
	int mulNumber1 = 1;
	int mulNumber2 = 2;
	int mulNumber3 = 3;
	int mulNumber4 = 4;
	int mulNumber5 = 5;
	//割り算
	int divNumber1 = 1;
	int divNumber2 = 2;
	int divNumber3 = 3;
	int divNumber4 = 4;
	int divNumber5 = 5;
	// ↓演算ブロックのフラグ
	int isAddNumber = 0;
	int isSubNumber = 0;
	int isMulNumber = 0;
	int isDivNumber = 0;

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

			// map番号2の場合↓
			if (isAddNumber == 0) {
				if (map[player.leftTopY][player.leftTopX] == ADD2 ||
				    map[player.rightTopY][player.rightTopX] == ADD2) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
					stockNumber = stockNumber + addNumber2; // 足し算
					isAddNumber = 1;
				}
			}
			// map番号3の場合↓
			if (isSubNumber == 0) {
				if (map[player.leftTopY][player.leftTopX] == 3 ||
				    map[player.rightTopY][player.rightTopX] == 3) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
					stockNumber = stockNumber - subNumber2; // 引き算
					isSubNumber = 1;
				}
			}
			// map番号4の場合↓
			if (isMulNumber == 0) {
				if (map[player.leftTopY][player.leftTopX] == 4 ||
				    map[player.rightTopY][player.rightTopX] == 4) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
					stockNumber = stockNumber * mulNumber4; // 掛け算
					isMulNumber = 1;
				}
			}
			// map番号4の場合↓
			if (isDivNumber == 0) {
				if (map[player.leftTopY][player.leftTopX] == 5 ||
				    map[player.rightTopY][player.rightTopX] == 5) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
					stockNumber = stockNumber / divNumber2; // 割り算
					isDivNumber = 1;
				}
			}

			if (isAddNumber == 1) { // 一度叩いたらただのブロックに戻る
				if (map[player.leftTopY][player.leftTopX] == 2 ||
				    map[player.rightTopY][player.rightTopX] == 2) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
				}
			}
			if (isSubNumber == 1) { // 一度叩いたらただのブロックに戻る
				if (map[player.leftTopY][player.leftTopX] == 3 ||
				    map[player.rightTopY][player.rightTopX] == 3) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
				}
			}

			if (isMulNumber == 1) { // 一度叩いたらただのブロックに戻る
				if (map[player.leftTopY][player.leftTopX] == 4 ||
				    map[player.rightTopY][player.rightTopX] == 4) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
				}
			}

			if (isDivNumber == 1) { // 一度叩いたらただのブロックに戻る
				if (map[player.leftTopY][player.leftTopX] == 5 ||
				    map[player.rightTopY][player.rightTopX] == 5) {
					ball.velocity.y = 0;
					playerPos.y = (player.leftTopY + 1) * mapTileSize;
				}
			}

			// 自由落下したときに地面にのめりこむかのめりこまないか
			if (map[player.leftBottomY][player.leftBottomX] == 1 ||
			    map[player.rightBottomY][player.rightBottomX] == 1) {
				isJamp = false;
				ball.velocity.y = 0;
				playerPos.y = (player.leftBottomY - 1) * mapTileSize;

			} // 自由落下中のスクロール
			  // スクロールする

			// 上からへの当たり判定
			// map番号2の場合↓

			if (map[player.leftBottomY][player.leftBottomX] == 2 ||
			    map[player.rightBottomY][player.rightBottomX] == 2) {
				isJamp = false;
				ball.velocity.y = 0;
				playerPos.y = (player.leftBottomY - 1) * mapTileSize;
			}

			// map番号3の場合↓
			if (map[player.leftBottomY][player.leftBottomX] == 3 ||
			    map[player.rightBottomY][player.rightBottomX] == 3) {
				isJamp = false;
				ball.velocity.y = 0;
				playerPos.y = (player.leftBottomY - 1) * mapTileSize;
			}

			// map番号4の場合↓
			if (map[player.leftBottomY][player.leftBottomX] == 4 ||
			    map[player.rightBottomY][player.rightBottomX] == 4) {
				isJamp = false;
				ball.velocity.y = 0;
				playerPos.y = (player.leftBottomY - 1) * mapTileSize;
			}

			// map番号5の場合↓
			if (map[player.leftBottomY][player.leftBottomX] == 5 ||
			    map[player.rightBottomY][player.rightBottomX] == 5) {
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
				// 進む先がブロックじゃなかったら右にスピード分進む
				if (map[player.rightTopY][player.rightTopX] == ENEMY1) {
					map[player.rightTopY][player.rightTopX] = NOBLOCK;
				}
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
						case NOBLOCK:
							Novice::DrawBox(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    mapTileSize, mapTileSize, 0.0f, BLACK, kFillModeSolid);
							break;
						case NOMALBLOCK:
							Novice::DrawBox(
							    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
							    mapTileSize, mapTileSize, 0.0f, RED, kFillModeSolid);
							break;
						case ENEMY1:
							if (isEnemyBlock == true) {
								Novice::DrawBox(
								    (x * enemyTileSize) - scroll.x, (y * enemyTileSize) - scroll.y,
								    enemyTileSize, enemyTileSize, 0.0f, GREEN, kFillModeSolid);
							}
							if (isEnemyBlock == false) {
								Novice::DrawBox(
								    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
								    mapTileSize, mapTileSize, 0.0f, BLACK, kFillModeSolid);
							}
							// map2の時
							if (isAddNumber == 0) {
								if (map[y][x] == 2) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, BLUE, kFillModeSolid);
								}
							}
							// 一度叩いたらただのブロックに戻る描画
							if (isAddNumber == 1) {
								if (map[y][x] == 2) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, RED, kFillModeSolid);
								}
							}

							// map3の時
							if (isSubNumber == 0) {
								if (map[y][x] == 3) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, GREEN, kFillModeSolid);
								}
							}
							if (isSubNumber == 1) { // 一度叩いたらただのブロックに戻る描画
								if (map[y][x] == 3) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, RED, kFillModeSolid);
								}
							}

							// map4の時
							if (isMulNumber == 0) {
								if (map[y][x] == 4) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, WHITE, kFillModeSolid);
								}
							}
							if (isMulNumber == 1) { // 一度叩いたらただのブロックに戻る描画
								if (map[y][x] == 4) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, RED, kFillModeSolid);
								}
							}

							// map5の時
							if (isDivNumber == 0) {

								if (map[y][x] == 5) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, BLUE, kFillModeSolid);
								}
							}
							if (isDivNumber == 1) { // 一度叩いたらただのブロックに戻る描画

								if (map[y][x] == 5) {
									Novice::DrawBox(
									    (x * mapTileSize) - scroll.x, (y * mapTileSize) - scroll.y,
									    mapTileSize, mapTileSize, 0.0f, RED, kFillModeSolid);
								}
							}
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
