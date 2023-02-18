#include <iostream>
#include <cmath>

class MyGauss
{
private:
    int n; // Размерность матрицы
    double** mk; // Значения коэффициентов в форме матрицы
    double* b; // b - заложены значения каждого уравнения в форме массива
    double* x; // Решение системы уравнений из матрицы

public:
    MyGauss()
    {   
        /*
        В данном конструкторе мы поэтапно заполняем все части матрицы
        Размерность, коэффициенты и значения
        */
        std::cout << "n=";
        std::cin >> n;
        mk = new double* [n];
        b = new double[n];
        x = new double[n];
        for (int i = 0; i < n; i++)
        {
            mk[i] = new double[n];
        }
        std::cout << "Coeficent matrix" << std::endl;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cout << "mk" << "[" << i << "]" << "[" << j << "]=";
                std::cin >> mk[i][j];
            }
        }
        std::cout << "Vector" << std::endl;
        for (int i = 0; i < n; i++)
        {
            std::cout << "b" << "[" << i << "]=";
            std::cin >> b[i];
        }
    }

        MyGauss& operator =(const MyGauss& temp)
    {
        //Если присвоили самого себя
        if (this == &temp)
        {
            return *this;
        }

        //освобождаем память
        delete[] mk;
        delete[] b;
        delete[] x;

        
        this->n  = temp.n;

        //Выделяем память под новые значения
        this->mk = new double* [n];
        this->b = new double[n];
        this->x = new double[n];

        //Выделяем память под каждую строку матрицы
        for (int i = 0; i < n; i++)
            this->mk[i] = new double[n];

        //Заполняем значения коэф матрицы
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {

                this->mk[i][j] = temp.mk[i][j];
            }
            for (int i = 0; i < n; i++)
            {
                this->b[i] = temp.b[i];
            }
        }
        //Заполняем значения решений СЛАУ
        for (int i = 0; i < n; i++)
        {
            this->x[i] = temp.x[i];
        }
    }

    MyGauss(const MyGauss &temp)
    {
        /*
        В данном конструкторе мы отрабатываем сценарий, когда в новый MyGauss нужно переместить
        данные из уже существубщего MyGauss
        */
        n = temp.n;
        mk = new double* [n];
        b = new double[n];
        x = new double[n];
        for (int i = 0; i < n; i++)
        {
            mk[i] = new double[n];
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                mk[i][j] = temp.mk[i][j];
            }
        }
        for (int i = 0; i < n; i++)
        {
            b[i] = temp.b[i];
        }
    }

    ~MyGauss()
    {
        //Освобождаем память, удаляя все переменные нашей матрицы
        delete[] b;
        delete[] x;
        for (int i = 0; i < n; i++)
        {
            delete[] mk[i];
        }
        delete[] mk;
    }

    bool SolveSystem();

    void print_result()
    {
        //Выводим результат решения системы уравнений из матрицы
        std::cout << "Result" << std::endl;
        for (int i = 0; i < n; i++)
        {
            std::cout << "x[" << i << "]=" << x[i] << std::endl;
        }
    }

	std::ostream& operator<<(std::ostream& t)
	{
		std::cout << "Ishodnaya rasshirennaya matrix" << std::endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				t << " " << mk[i][j];
			t << " " << b[i];
			t << std::endl;
		}
		return t;
	}
};

bool MyGauss::SolveSystem()
{
    //Решение системы линейных уравенений из матрицы
    int i, j, k;
    double m;
    bool hasSolved = true;
    //Проходим по каждой строке в матрице
    for (i = 0; i < n; i++)
    {
        int max_index = i;
        //Идём от следующей строки до последней строки  
        for (j = i + 1; j < n; j++) 
        {   
            //Если |текущего элемента в столбце i|
            // больше чем |элемента в том же столбце, в строке max_index|
            if (fabs(mk[j][i]) > fabs(mk[max_index][i]))
            {
                max_index = j;
            }
        }

        // Если i не равно max_index, то строки 
        // i и max_index меняются местами. 
        // Это делается для того, 
        // чтобы самый большой элемент в каждом столбце 
        // находился в i-й строке.
        if (i != max_index)
        {
            std::swap(mk[i], mk[max_index]);
            std::swap(b[i], b[max_index]);
        }
        //Идём от следующей строки до последней строки
        for (j = i + 1; j < n; j++)
        {
            //Считаем будущий коэфициент умножения
            m = mk[j][i] / mk[i][i];
            //Идем от столбца i+1 до последнего
            for (k = i + 1; k < n; k++)
            {
                
                mk[j][k] -= m * mk[i][k];
            }
            b[j] -= m * b[i];
        }
        for (int mainD = 0;  mainD < n; mainD++)
        {
            if (fabs(mk[mainD][mainD]) < 1e-12)
            {
                hasSolved = false;
            }
        }
    }



    if (hasSolved){
        //Значение последнего неизвестного
        x[n - 1] = b[n - 1] / mk[n - 1][n - 1];
        //Идём от предпоследней строки к первой
        for (i = n - 2; i >= 0; i--)
        {
            x[i] = b[i];
            for (j = i + 1; j < n; j++)
            {
                x[i] -= mk[i][j] * x[j];
            }
            x[i] /= mk[i][i];
        }
        return true;
    } else {
        std::cout << "Sorry, this matrix doesn't have solutions!" << std::endl;
        return false;
    }

}


int main(){

    MyGauss gauss;
    MyGauss gauss1(gauss);
    gauss << std::cout;
    gauss1 << std::cout;   
    if (gauss1.SolveSystem() == 1)
    {
        gauss << std::cout;
        gauss1 << std::cout;
        gauss1.print_result();
    }
    else
    {

    }

}