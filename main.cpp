// Bulat Latypov
#include <bits/stdc++.h>
using namespace std;

class Matrix {
protected:
    int n;
    int m;
    double** table;
    double** aug;
    class Row {
    private:
        double *row;
    public:
        Row (double *row) {
            this->row = row;
        }
        double& operator[](int ind) {
            return row[ind];
        }
    };
public:
    Matrix () {
        n = m = 0;
        table = nullptr;
    }
    Matrix (int n, int m = -1) {
        if (m < 1) {
            m = n;
        }
        this->n = n;
        this->m = m;
        table = new double*[n];
        aug = new double*[n];
        for (int i = 0; i < n; i++) {
            table[i] = new double[m];
            aug[i] = new double[n];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                table[i][j] = 0;
            }
            for (int j = 0; j < n; j++) {
                aug[i][j] = i == j ? 1 : 0;
            }
        }
    }
    virtual ~Matrix() {
        if (table || !table) {
            for (int i = 0; i < n; i++) {
                delete[] table[i];
            }
            delete[] table;
        }
    }
    int getN() {
        return n;
    }
    int getM() {
        return m;
    }
    void setN(int n) {
        this->n = n;
    }
    void setM(int m) {
        this->m = m;
    }
    double **getTable() {
        return table;
    }
    double **getAug() {
        return aug;
    }
    void setElimination (int x, int y) {
        int ind = y;
        if (table[ind][y] == 0) {
            for (ind; ind < n; ind++) {
                if (ind != x && table[ind][y] != 0) {
                    break;
                }
            }
            if (ind == n) {
                ind--;
            }
            if (table[ind][y] == 0) {
                ind = x;
            }
        }
        if (abs(table[ind][y]) == 0) {
            return;
        }
        double val = table[x][y] / table[ind][y];
        for (int i = 0; i < m; i++) {
            table[x][i] -= table[ind][i] * val;
            if (abs(table[x][i]) == 0) {
                table[x][i] = 0;
            }
        }
        for (int i = 0; i < m; i++) {
            aug[x][i] -= aug[ind][i] * val;
        }
    }
    void setPermutation (int r1, int r2) {
        swap(table[r1], table[r2]);
        swap(aug[r1], aug[r2]);
    }
    void diagonalNormalization() {
        for (int i = 0; i < n; i++) {
            double val = table[i][i];
            if (val != 0) {
                for (int j = 0; j < m; j++) {
                    table[i][j] /= val;
                }
                for (int j = 0; j < n; j++) {
                    aug[i][j] /= val;
                }
            }
        }
    }
    void displayAug() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << fixed << aug[i][j] << ' ';
            }
            cout << endl;
        }
    }
    void inverse() {
        for (int j = 0; j < n; j++) {
            double mx = table[j][j];
            int ind = j;
            for (int i = j + 1; i < n; i++) {
                if (mx < abs(table[i][j])) {
                    mx = abs(table[i][j]);
                    ind = i;
                }
            }
            if (ind != j) {
                this->setPermutation(j, ind);
            }
            for (int i = j + 1; i < n; i++) {
                if (table[i][j] != 0) {
                    this->setElimination(i, j);
                }
            }
        }
        for (int j = n - 1; j >= 0; j--) {
            for (int i = j - 1; i >= 0; i--) {
                if (table[i][j] != 0) {
                    this->setElimination(i, j);
                }
            }
        }
        this->diagonalNormalization();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                swap(table[i][j], aug[i][j]);
            }
        }
    }
    virtual void fill() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> table[i][j];
            }
        }
    }
    virtual void display() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << table[i][j] << ' ';
            }
            cout << endl;
        }
    }
    virtual void fullDisplay() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << fixed << table[i][j] << ' ';
            }
            for (int j = 0; j < n; j++) {
                cout << fixed << aug[i][j] << ' ';
            }
            cout << endl;
        }
    }
    double *getB(int a[], bool ag = false) {
        double b[n];
        if (ag) {
            for (int i = 0; i < n; i++) {
                double res = 0;
                for (int j = 0; j < n; j++) {
                    res += a[j] * aug[i][j];
                }
                b[i] = res;
            }
        } else {
            for (int i = 0; i < n; i++) {
                double res = 0;
                for (int j = 0; j < m; j++) {
                    res += a[j] * table[i][j];
                }
                b[i] = res;
            }
        }
        return b;
    }
    void displayB(double a[], bool ag = false) {
        if (ag) {
            for (int i = 0; i < n; i++) {
                double res = 0;
                for (int j = 0; j < n; j++) {
                    res += a[j] * aug[i][j];
                }
                cout << fixed << res << endl;
            }
        } else {
            for (int i = 0; i < n; i++) {
                double res = 0;
                for (int j = 0; j < m; j++) {
                    res += a[j] * table[i][j];
                }
                cout << fixed << res << endl;
            }
        }
    }
    virtual Row& operator[] (int ind) {
        Row *row = new Row(table[ind]);
        return *row;
    }
    virtual Matrix& operator+ (Matrix &a) {
        if (m != a.m || n != a.n) {
            throw "Error: the dimensional problem occurred";
        }
        Matrix *sum = new Matrix(a.n, a.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                (*sum)[i][j] = table[i][j] + a[i][j];
            }
        }
        return *sum;
    }
    virtual Matrix& operator- (Matrix &a) {
        if (m != a.m || n != a.n) {
            throw "Error: the dimensional problem occurred";
        }
        Matrix *dif = new Matrix(a.n, a.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                (*dif)[i][j] = table[i][j] - a[i][j];
            }
        }
        return *dif;
    }
    virtual Matrix& operator* (Matrix &a) {
        if (m != a.n) {
            throw "Error: the dimensional problem occurred";
        }
        Matrix *mult = new Matrix(n, a.m);
        for (int row = 0; row < n; row++) {
            for (int column = 0; column < a.m; column++) {
                for (int i = 0; i < m; i++) {
                    (*mult)[row][column] += table[row][i] * a.table[i][column];
                }
            }
        }
        return *mult;
    }
    virtual Matrix& operator !() {
        Matrix *transposed = new Matrix(m, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                transposed->table[j][i] = table[i][j];
            }
        }
        return *transposed;
    }
    virtual Matrix& operator =(Matrix &a) {
        for (int i = 0; i < n; i++) {
            delete[] table[i];
            delete[] aug[i];
        }
        delete[] table;
        delete[] aug;
        n = a.n;
        m = a.m;
        table = new double*[n];
        aug = new double*[n];
        for (int i = 0; i < n; i++) {
            table[i] = new double[m];
            aug[i] = new double[n];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                table[i][j] = a.table[i][j];
            }
            for (int j = 0; j < n; j++) {
                aug[i][j] = a.aug[i][j];
            }
        }
        return *this;
    }
};

class IdendityMatrix : public Matrix {
public:
    IdendityMatrix() : Matrix(){};
    IdendityMatrix (int n, int m = -1) : Matrix(n, m){
        for (int i = 0; i < min(n, m); i++) {
            table[i][i] = 1;
        }
    }
    IdendityMatrix& operator = (IdendityMatrix& a) {
        for (int i = 0; i < n; i++) {
            delete[] table[i];
        }
        delete[] table;
        n = a.getN();
        m = a.getM();
        table = new double*[n];
        for (int i = 0; i < n; i++) {
            table[i] = new double[m];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                table[i][j] = a.getTable()[i][j];
            }
        }
        return *this;
    }
};

ostream& operator<<(ostream& os, Matrix& a) {
    a.display();
    return os;
}
istream& operator>>(istream& os, Matrix& a) {
    a.fill();
    return os;
}

int main() {
//    freopen("input.txt", "r", stdin);
    cout.precision(4);

    int n;
    cin >> n;
    int ti[n], bi[n];
    for (int i = 0; i < n; i++) {
        cin >> ti[i] >> bi[i];
    }
    int degree;
    cin >> degree;
    degree++;
    Matrix a(n, degree), b(n, 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < degree; j++) {
            a[i][j] = pow(ti[i], j);
        }
        b[i][0] = bi[i];
    }
    Matrix a1(degree);
    a1 = (!a * a);
    Matrix a2(degree);
    a2 = a1;
    a2.inverse();
    Matrix a3(1);
    a3 = !a * b;
    cout << "A:\n" << fixed << a;
    cout << "A_T*A:\n" << fixed << a1;
    cout << "(A_T*A)^-1:\n" << fixed << a2;
    cout << "A_T*b:\n" << fixed << a3;
    a1.inverse();
    double b1[n];
    for (int i = 0; i < degree; i++) {
        b1[i] = a3[i][0];
    }
    cout << "x~:\n";
    a1.displayB(b1);
}
