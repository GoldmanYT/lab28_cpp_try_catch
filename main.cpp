#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cassert>

using namespace std;


const double h = 0.5;
const int a = -5 / h, b = 5 / h;
int NErrors;


struct errors
{
    string function_name;
    string message;
    double parameter;
};



double my_y(double x);
double y(double x);
double my_cos(double x);
double my_sqrt(double x);
double my_log(double x);
double my_div(double x, double y);


int main()
{
    cout << "y = cos(x) / (sqrt(x) - 2) * ln(x / 2)\n";
    cout << "Interval: [" << a * h << ", " << b * h << "]\n";
    cout << "Count of points: " << b - a + 1 << "\n";
    cout << "Step: " << h << "\n";
    cout << "Enter NErrors: ";
    cin >> NErrors;

    ofstream file("output.txt");
    if (!file)
    {
        cout << "File open error\n";
        return 0;
    }
    int error_count = 0;
    for (int i = a; i <= b; i++)
    {
        bool computed = 0;
        double x = i * h, ans;
        file << x << "\t";
        cout << "x = " << x << "; y = ";
        try
        {
            ans = y(x);
            computed = 1;
            ans = my_y(x);
            cout << ans << "\n";
            file << ans << "\n";
        }
        catch(const string& err)
        {
            cout << ans << "; " << err << "\n";
            file << ans << "\n";
            error_count++;
        }
        catch(const errors& err)
        {
            cout << "NaN; " << err.message << "\n";
            file << "NaN\n";
            error_count++;
        }
        if (error_count >= NErrors)
        {
            break;
        }
    }
    file.close();
    assert(("Error limit has been reached\n", error_count < NErrors));
    return 0;
}


double my_y(double x)
{
    return my_div(my_cos(x), (my_sqrt(x) - 2)) * my_log(x / 2);
}


double y(double x)
{
    return my_div(cos(x), (my_sqrt(x) - 2)) * my_log(x / 2);
}


double my_cos(double x)
{
    if (x < 0 || x > M_PI)
    {
        throw (string) "Cosine argument is out of bounds";
    }
    return cos(x);
}


double my_sqrt(double x)
{
    if (x < 0)
    {
        throw errors {
            function_name: "sqrt", 
            message: "Square root argument is out of bounds", 
            parameter: x
        };
    }
    return sqrt(x);
}


double my_log(double x)
{
    if (x <= 0)
    {
        throw errors {
            function_name: "log", 
            message: "Natural logarithm argument is out of bounds", 
            parameter: x
        };
    }
    return log(x);
}


double my_div(double x, double y)
{
    if (y == 0)
    {
        throw errors {
            function_name: "div", 
            message: "Division by zero", 
            parameter: y
        };
    }
    return x / y;
}