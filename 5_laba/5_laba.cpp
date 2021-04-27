// 5_laba.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>


using namespace std;

// Varibles
struct Point
{
    double x, y;	
};

vector<Point> Points;

#define CT_A 0	//константы для задания режима «заморозки» переменных a,b,r
#define CT_B 1	// константы для задания режима «заморозки» переменных b,a,r
#define CT_C 2	// константы для задания режима «заморозки» переменных b,a,r
#define CT_R 3	// константы для задания режима «заморозки» переменных r,a,b


// Functions
void Alert(string mes);															//функция вывода предупреждения
double MSQ(double a, double b, double r);										//функция вычисления среднеквадратичного отклонения для текущих значений a,b,r
void LinearDixotomy(double& value1, double value2, double value3, int type);	//функцияреализации метода половинногоделения


int main()
{
	int nRetCode = 0;
	setlocale(LC_ALL, "RUS");

	//читаем данные
	FILE* fl;
	fopen_s(&fl, "./data.txt", "rt");

	if (!fl)//что-то не так с файлом
	{
		Alert("Проблема с файлом");
		return 0;//выходим
	}

	char buf[100];//буфер под текстовые строки
	double x, y;//буфер под координаты
	while (!feof(fl))//цикл пока не кончится файл
	{
		fgets(buf, 100, fl);//берем строку в текстовый буфер

		if (sscanf_s(buf, "%lf %lf", &x, &y) == 2) //парсимвзятуюизфайластроку
		{//распарсилось две координаты
			Point pt;//структура-буфер под точки
			pt.x = x;//запоминаем х
			pt.y = y;//запоминаем y
			Points.push_back(pt);//добавляем точку в массив структур
		}
		else//в противном случае
		{
			Alert("Проблема с данными");//предупреждение об ошибке
			return 0;//выходим
		}

	}
	//закрываем файл
	fclose(fl);

	double A, B, R;//переменные под координаты центра и радиус окружности
	A = B = R = 0.f;//обнуляем 

	//находим начальные приближения
	for (int i = 0; i < Points.size(); i++)	//цикл по всем точкам
	{
		A += Points[i].x; //суммируемх
		B += Points[i].y; //суммируемy
	}

	A /= Points.size();//находим среднее значения
	B /= Points.size();//находим среднее значения

	for (int i = 0; i < Points.size(); i++)
	{//радиускаждойточки
		R += sqrt(pow(Points[i].x - A, 2) + pow(Points[i].y - B, 2));
	}

	R /= Points.size(); //вычисляем среднее значение радиуса

	double A_old, B_old, R_old; //буфер под предыдущие значения

	while (true)//цикл пока не достигнем требуемой точности
	{
		A_old = A;//запоминаем старое значение А
		B_old = B;//запоминаем старое значение B
		R_old = R;//запоминаем старое значение R

		LinearDixotomy(A, B, R, CT_A); //запускаем дихотомию по А
		LinearDixotomy(B, A, R, CT_B); //запускаем дихотомию по B
		LinearDixotomy(R, A, B, CT_R); //запускаем дихотомию по R

		//вычисляем «длину» интервала и сравниваем с допустимой погрешностью
		if (sqrt(pow(A - A_old, 2) + pow(B - B_old, 2) + pow(R - R_old, 2)) < 0.0001)
			break; //если достигли требуемой точности - выходим
	}

	cout << "x - " << A << endl; //выводим результат по А
	cout << "y - " << B << endl; //выводим результат по B
	cout << "r - " << R << endl; //выводим результат по R
	cout << endl;

	system("pause");

	return nRetCode;
}


// implementing functions
void Alert(string mes) {
	cout << "Warning: " << mes << endl;
};

double MSQ(double a, double b, double r)
{
	double res = 0;
	//цикл по всем точкам 
	for (int i = 0; i < Points.size(); i++)
	{
		//отклонение по нормали к дуге окружности
		double delta = pow(Points[i].x - a, 2) + pow(Points[i].y - b, 2) - r * r;
		res += delta * delta;//возводим отклонение в квадрат
	}
	return res;//возвращаем результат в программу верхнего уровня
}

void LinearDixotomy(double& value1, double value2, double value3, int type) {
	double epsilon = 0.000001f; //погрешностьвычисления

	double step = epsilon * 100; //шаг

	double V_Left, V_Right, MSQ1, MSQ2; //значения на границах интервала и СКО

	switch (type) //переключатель по типам расчета
	{
	case CT_A://расчет по а
		MSQ1 = MSQ(value1, value2, value3);
		MSQ2 = MSQ(value1 - step, value2, value3);
		break;
	case CT_B: //расчетпоb
		MSQ1 = MSQ(value2, value1, value3);
		MSQ2 = MSQ(value2, value1 - step, value3);
		break;
	case CT_R: //расчетпоr
		MSQ1 = MSQ(value2, value3, value1);
		MSQ2 = MSQ(value2, value3, value1 - step);
		break;
	}

	if (MSQ1 > MSQ2)
	{
		V_Right = V_Left = value1;
		while (true)
		{
			V_Left = V_Left - step;

			switch (type)
			{
			case CT_A: //расчетпоа
				MSQ1 = MSQ(V_Left, value2, value3);
				MSQ2 = MSQ(V_Left + step, value2, value3);
				break;
			case CT_B: //расчетпо b
				MSQ1 = MSQ(value2, V_Left, value3);
				MSQ2 = MSQ(value2, V_Left + step, value3);
				break;
			case CT_R: //расчетпо r
				MSQ1 = MSQ(value2, value3, V_Left);
				MSQ2 = MSQ(value2, value3, V_Left + step);
				break;
			}

			if (MSQ1 > MSQ2)
				break;
		}
	}
	else
	{
		V_Right = V_Left = value1;
		while (true)
		{
			V_Right = V_Right + step;
			switch (type)
			{
			case CT_A: //расчетпо а
				MSQ1 = MSQ(V_Right, value2, value3);
				MSQ2 = MSQ(V_Right + step, value2, value3);
				break;
			case CT_B: //расчетпо b
				MSQ1 = MSQ(value2, V_Right, value3);
				MSQ2 = MSQ(value2, V_Right + step, value3);
				break;
			case CT_R: //расчетпо c
				MSQ1 = MSQ(value2, value3, V_Right);
				MSQ2 = MSQ(value2, value3, V_Right + step);
				break;
			}
			if (MSQ1 < MSQ2)
				break;
		}
	}

	double V_Mid;//значение в середине интервала

	//финальная дихотомия	
	while (V_Right - V_Left > epsilon) //циклдихотомии
	{
		V_Mid = (V_Right + V_Left) / 2.f;//вычисление середины интервала
		switch (type)//переключатель по типам расчета
		{
		case CT_A://расчет по а
			MSQ1 = MSQ(V_Mid + epsilon / 3, value2, value3);
			MSQ2 = MSQ(V_Mid - epsilon / 3, value2, value3);
			break;
		case CT_B://расчет по b
			MSQ1 = MSQ(value2, V_Mid + epsilon / 3, value3);
			MSQ2 = MSQ(value2, V_Mid - epsilon / 3, value3);
			break;
		case CT_R: //расчетпо r
			MSQ1 = MSQ(value2, value3, V_Mid + epsilon / 3);
			MSQ2 = MSQ(value2, value3, V_Mid - epsilon / 3);
			break;
		}
		if (MSQ1 < MSQ2)
		{
			V_Left = V_Mid;
		}
		else
		{
			V_Right = V_Mid;
		}
	}
	value1 = (V_Right + V_Left) / 2.f; //середина интервала
}
