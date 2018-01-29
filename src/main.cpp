#include "lib/framework.hpp"
#include <ctime>//時計を使う

enum Size {
  WIDTH  = 800,
  HEIGHT = 600
};
enum Colors {
	RED = 0,
	BLACK = 1,
	GREEN = 2
};
enum Player {
	PC1 = 0,
	PC2 = 1,
	PC3 = 2
};
int RedNums[64] = { 1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36 };//0-17
int BlackNums[64] = { 2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35 };//0-17

int main() {
	AppEnv env(Size::WIDTH, Size::HEIGHT);

	Random Roulette;//ルーレットの結果を出す
	int Result = 0;
	int ResultCount[38] = { 0 };
	int PlayCount = 0;
	int HisColor = -1;

	int Dealer = 0;
	int Money[3] = { 0 };
	int WinRate[3] = { 0 };

	bool RedBet = false;

	Font Font("res/07やさしさゴシック.ttf");
	Font.size(30);
	Roulette.setSeed(std::time(nullptr));


  while (env.isOpen()) {
	env.begin();
	
	char Txt[256];
	int DrawResult = 1;
	Color color;

	PlayCount++;
	Result = Roulette(0, 37);//0=0,37=00とする
	ResultCount[Result]++;
	
	HisColor = -1;
	for (int i = 0; i < 18; i++)
	{
		if (Result == RedNums[i])
		{
			HisColor = RED;
		}
		else if (Result == BlackNums[i])
		{
			HisColor = BLACK;
		}
	}
	if (HisColor == -1)HisColor = GREEN;

	if(Result == 37)printf("%d回目：00	", PlayCount);
	else printf("%d回目：%d	", PlayCount, Result);
	Dealer += 3;

	//ここからPC1の処理。赤か黒に賭け続ける。配当2倍
	if (Roulette() == 0)RedBet = true;
	else RedBet = false;

	Money[PC1]--;
	if (HisColor == RED && RedBet == true) {
		WinRate[PC1]++;
		Money[PC1] += 2;
		Dealer -= 2;
		printf("PC1 WIN	");
	}
	else if(HisColor == BLACK && RedBet == false) {
		WinRate[PC1]++;
		Money[PC1] += 2;
		Dealer -= 2;
		printf("PC1 WIN	");
	}
	else printf("	");

	//ここからPC2の処理。縦1列にかけ続ける。配当3倍
	int PC2Bet = Roulette(0, 2);
	Money[PC2]--;
	if (Result % 3 == PC2Bet && Result != 0 && Result != 37) {
		WinRate[PC2]++;
		Money[PC2] += 3;
		Dealer -= 3;
		printf("PC2 WIN	");
	}
	else printf("	");

	//ここからPC3の処理。1目賭け。配当36倍
	int PC3Bet = Roulette(0, 37);

	Money[PC3]--;
	if (Result == PC3Bet) {
		WinRate[PC3]++;
		Money[PC3] += 36;
		Dealer -= 36;
		printf("PC3 WIN	");
	}
	else printf("	");

	printf("\n");

	//ここから数字の描写部分
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{			
			sprintf(Txt,"%d : %d", DrawResult, ResultCount[DrawResult]);

			for (int i = 0; i < 18; i++)
			{
				if (DrawResult == RedNums[i])
				{
					color = Color(1, 0.5f, 0.5f);
				}
				else if(DrawResult == BlackNums[i])
				{
					color = Color(1,1,1);
				}
			}
			Font.draw(Txt, Vec2f(-395 + (i * 130) , -50 * j), color);
			DrawResult++;
		}
	}
	sprintf(Txt, "%d : %d", 0, ResultCount[0]);
	color = Color(0.5, 1, 0.5);
	Font.draw(Txt, Vec2f(-395 + (4 * 130), -50 * 6), color);
	sprintf(Txt, "00 : %d", ResultCount[37]);
	color = Color(0.5, 1, 0.5);
	Font.draw(Txt, Vec2f(-395 + (5 * 130), -50 * 6), color);

	sprintf(Txt, "Dealer Money = %d", Dealer);
	Font.draw(Txt, Vec2f(-350, 230), Color(1, 1, 1));

	sprintf(Txt, "PC1: Money = %d , WinRate = %4.2f Per",Money[PC1],((double)WinRate[PC1] / (double)PlayCount) * 100);
	Font.draw(Txt, Vec2f(-350,200), Color(1,1,1));

	sprintf(Txt, "PC2: Money = %d , WinRate = %4.2f Per", Money[PC2], ((double)WinRate[PC2] / (double)PlayCount) * 100);
	Font.draw(Txt, Vec2f(-350, 170), Color(1, 1, 1));

	sprintf(Txt, "PC3: Money = %d , WinRate = %4.2f Per", Money[PC3], ((double)WinRate[PC3] / (double)PlayCount) * 100);
	Font.draw(Txt, Vec2f(-350, 140), Color(1, 1, 1));


    env.end();
  }
}
