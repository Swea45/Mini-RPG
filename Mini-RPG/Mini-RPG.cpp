
#include <iostream>
#include <string>
#include <conio.h>
#include <random>
#include <chrono>
#include <thread>

#define KEY_W 119
#define KEY_S 115
#define KEY_ENTER 13

enum StartingHp
{
	PlayerHp = 25,
	GoblinHp = 20,
};
enum CombatNumbers
{
	meleeDamage = 5,
	magicDamage = meleeDamage * 2,
	enemyDamage = 5,
	playerHealMax = 10,
	playerHealMin = 0,
};
enum PlayerOptions
{
	Melee = 0,
	Magic = 1,
	Heal = 2,
};

enum Color
{
	Green = 'G',
	Red = 'R',
	White = 'W'
};

void Paint(char colore) 
{
	switch (colore)
	{
	case Color::Green:
		{
		std::cout << "\x1B[32m";
		break;
		}

	case Color::Red:
		{
		std::cout << "\x1b[31m";
			break;
		}

	case Color::White:
		{
		std::cout << "\x1B[37m";
		break;
		}
	default:
		break;
	}

}
void Sleep(int milliSeconds = 1000)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliSeconds));
}
void Clear()
{
	system("cls");
}
void Pause()
{
	system("pause>0");
}
void Print(std::string aText)
{
	std::cout << aText << std::endl;
}
int RandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrub(min, max);

	return distrub(gen);
}

int ButtonPress()
{
	int button = 0;
	button = _getch();
	while (true)
	{
		if (button == KEY_W)
		{
			return 1;
		}
		else if (button == KEY_S)
		{
			return 2;
		}
		else if (button == KEY_ENTER)
		{
			return 3;
		}
	}
}
int MenuList(std::string menuList[], int menuSize, int playerChoiseInMenu = 0, std::string extraText = "")
{
	if (extraText != "")
	{
		Print(extraText);
	}

	for (int i = 0; i < menuSize; ++i)
	{
		if (i == playerChoiseInMenu)
		{
			Paint(Color::Green);
			Print("\t" + menuList[i] + " <<");
		}
		else
		{
			Paint(Color::White);
			Print(menuList[i]);
		}
		Paint(Color::White);
	}
	return ButtonPress();

}

void EnemyAttack(int& aPlayerHp, int& aEnemyHp)
{
	int damageDelt = CombatNumbers::enemyDamage + RandomNumber(-1, 1);
	int goblinAttack = RandomNumber(1, 10);
	Clear();
	Print("The goblin hits you with his sword...");
	Sleep(500);
	if (goblinAttack == 10)
	{
		damageDelt *= 2;
		Print("And gets a critical hit!");
		Paint(Color::Red);
		Print("Dealing: " + std::to_string(damageDelt) + " damage");
		Paint(Color::White);
		aPlayerHp -= damageDelt;
	}
	else if (goblinAttack == 0)
	{
		Print("But missing you, you take no damage!");

	}
	else
	{
		Paint(Color::Red);
		Print("Dealing: " + std::to_string(damageDelt) + " damage");
		Paint(Color::White);
		aPlayerHp -= damageDelt;
	}
	Sleep();
	return;
}

void PlayerAttack(int& aPlayerHp, int& aEnemyHp, int& attackChoise)
{
	std::string attackMenu[] = { "Melee Attack", "Magic Attack", "Heal", "Nothing" };
	std::string anHpUppdate = "You have:\t" + std::to_string(aPlayerHp) + "HP\nThe enemy have:\t" + std::to_string(aEnemyHp) + "HP\n\nWhat attack do you wana do? ";
	int damageDealt;
	while (true)
	{
		Clear();
		switch (MenuList(attackMenu, static_cast<int>(size(attackMenu)), attackChoise, anHpUppdate))
		{
		case 1:
		{
			if (attackChoise > 0)
			{
				attackChoise--;
			}
			else
			{
				attackChoise = static_cast<int>(size(attackMenu) - 1);
			}
			break;
		}
		case 2:
		{
			if (attackChoise < size(attackMenu) - 1)
			{
				attackChoise++;
			}
			else
			{
				attackChoise = 0;
			}
			break;
		}
		case 3:
		{
			Clear();
			switch (attackChoise)
			{
			case PlayerOptions::Melee:
			{
				Print("You attack with the sword...");
				Sleep(500);
				Paint(Color::Red);
				Print("You deal " + std::to_string(CombatNumbers::meleeDamage) + " damage");
				Paint(Color::White);
				Sleep();
				aEnemyHp -= CombatNumbers::meleeDamage;
				return;
				break;
			}
			case PlayerOptions::Magic:
			{
				Print("You attac with a magic bolt...");
				Sleep(500);
				if (RandomNumber(0, 1) == 1)
				{
					Paint(Color::Red);
					Print("You hit the enemy dealing " + std::to_string(CombatNumbers::magicDamage) + " damage");
					Paint(Color::White);
					Sleep();
					aEnemyHp -= CombatNumbers::magicDamage;
					return;
				}
				else
				{
					Print("You missed...");
					Sleep(250);
					Print("Dealing no damage");
					Sleep();
					return;
				}
			}
			case PlayerOptions::Heal:
			{
				int healAmount = RandomNumber(playerHealMin, playerHealMax);
				Paint(Color::Green);
				Print("You mend to your wounds and heal " + std::to_string(healAmount) + " HP");
				Paint(Color::White);
				aPlayerHp += healAmount;
				Sleep();
				return;
				break;
			}
			default:
				Print("You did nothing");
				Sleep();
				return;
				break;
			}

			break;
		}
		}
	}

	return;
}

void Combat(int& aPlayerHp, int& aEnemyHp)
{
	int menuChoise = 0;
	while (aPlayerHp > 0 && aEnemyHp > 0)
	{
		if (aPlayerHp > 0)
		{
			PlayerAttack(aPlayerHp, aEnemyHp, menuChoise);
		}

		if (aEnemyHp > 0)
		{
			EnemyAttack(aPlayerHp, aEnemyHp);
		}
	}
}

int main()
{
	int playerHp = StartingHp::PlayerHp;
	int enemyHp = StartingHp::GoblinHp;
	int enemyDefeted = 0;

	while (playerHp > 0)
	{
		Combat(playerHp, enemyHp);
		if (enemyHp <= 0)
		{
			Print("You have killd the goblin");
			++enemyDefeted;
			enemyHp = StartingHp::GoblinHp;
		}
	}

	if (playerHp <= 0)
	{
		Print("You have Died...");
		Print("You defeted: " + std::to_string(enemyDefeted) + " Enemis");
	}
	return 0;

}
