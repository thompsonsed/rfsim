//This file is part of necsim project which is released under MIT license and reused for RabbitsFoxesSimulation
//See file **LICENSE.txt** or visit https://opensource.org/licenses/MIT) for full license details.

/**
 * @author Samuel Thompson
 * @file Matrix.h
 * @copyright <a href="https://opensource.org/licenses/MIT"> MIT Licence.</a>
 * @brief Contains a template for a matrix with all the basic matrix operations overloaded.
 *
 * @details Provides an efficient, general purpose 2D matrix object with an efficient indexing system designed for
 * modern CPUs (where memory access times are often much longer than compute times for mathematical operations).
 * Most operations are low-level, but some higher level functions remain, such as importCsv().
 *
 * Contact: thompsonsed@gmail.com
 */


#ifndef MATRIX
#define MATRIX
#define null 0

#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>


#include <cstdint>

using namespace std;


/**
 * @brief A class containing the Matrix object, set up as an array of Row objects.
 * Includes basic operations, as well as the importCsv() function for more advanced reading from file.
 * @tparam T the type of the values in the matrix
 */
template<class T>
class Matrix
{

protected:

    // number of rows and columns
    unsigned long num_cols{};
    unsigned long num_rows{};
    // a matrix is an array of rows
    vector<T> matrix;
public:

    /**
     * @brief The standard constructor
     * @param rows optionally provide the number of rows.
     * @param cols optionally provide the number of columns.
     */
    explicit Matrix(unsigned long rows = 0, unsigned long cols = 0) : matrix(rows * cols, T())
    {
    }

//    /**
//     * @brief The copy constructor.
//     * @param m a Matrix object to copy from.
//     */
//    Matrix(const Matrix &m) : matrix()
//    {
//        this = m;
//    }

    /**
    * @brief The destructor.
    */
    virtual ~Matrix()
    = default;

    /**
     * @brief Sets the matrix size.
     * Similar concept to that for Rows.
     * @param rows the number of rows.
     * @param cols the number of columns.
     */
    void setSize(unsigned long rows, unsigned long cols)
    {
        if(!matrix.empty())
        {
            matrix.clear();
        }
        matrix.resize(rows * cols);
        num_cols = cols;
        num_rows = rows;
    }

    /**
     * @brief Getter for the number of columns.
     * @return the number of columns.
     */
    unsigned long getCols() const
    {
        return num_cols;
    }

    /**
     * @brief Getter for the number of rows.
     * @return the number of rows.
     */
    unsigned long getRows() const
    {
        return num_rows;
    }

    /**
     * @brief Gets the index of a particular row and column in the matrix.
     * @param row the row number to index
     * @param col the column number to index
     * @return the index of row and column within the matrix
     */
    unsigned long index(const unsigned long &row, const unsigned long &col) const
    {
#ifdef DEBUG
        if(num_cols == 0 || num_rows == 0)
        {
            throw out_of_range("Matrix has 0 rows and columns for indexing from.");
        }
        if(col + num_cols * row > matrix.size())
        {
            stringstream ss;
            ss << "Index of " << col + num_cols * row << ", (" << row << ", " << col << ")";
            ss << " is out of range of matrix vector with size " << matrix.size() << endl;
            throw out_of_range(ss.str());
        }
#endif // DEBUG
        return col + num_cols * row;
    }

    /**
     * @brief Gets the value at a particular index.
     * @param row the row number to get the value at
     * @param col the column number to get the value at
     * @return the value at the specified row and column
     */
    T &get(const unsigned long &row, const unsigned long &col)
    {
#ifdef DEBUG
        if(row < 0 || row >= num_rows || col < 0 || col >= num_cols)
        {
            stringstream ss;
            ss << "Index of " << row << ", " << col << " is out of range of matrix with size " << num_rows;
            ss << ", " << num_cols << endl;
            throw out_of_range(ss.str());
        }
#endif
        return matrix[index(row, col)];
    }

    /**
     * @brief Gets the value at a particular index.
     * @param row the row number to get the value at
     * @param col the column number to get the value at
     * @return the value at the specified row and column
     */
    T getCopy(const unsigned long &row, const unsigned long &col) const
    {
#ifdef DEBUG
        if(row < 0 || row >= num_rows || col < 0 || col >= num_cols)
        {
            stringstream ss;
            ss << "Index of " << row << ", " << col << " is out of range of matrix with size " << num_rows;
            ss << ", " << num_cols << endl;
            throw out_of_range(ss.str());
        }
#endif
        return matrix[index(row, col)];
    }

    /**
     * @brief Overloading the = operator.
     * @param m the matrix to copy from.
     */
    Matrix &operator=(const Matrix &m)
    {
        this->matrix = m.matrix;
        this->num_cols = m.num_cols;
        this->num_rows = m.num_rows;
        return *this;
    }

    /**
     * @brief Overloading the + operator.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the matrix to add to this matrix.
     * @return the matrix object which is the sum of the two matrices.
     */
    Matrix operator+(const Matrix &m) const
    {
        //Since addition creates a new matrix, we don't want to return a reference, but an actual matrix object.
        unsigned long new_num_cols = findMinCols(this, m);
        unsigned long new_num_rows = findMinRows(this, m);
        Matrix result(new_num_rows, new_num_cols);
        for(unsigned long r = 0; r < new_num_rows; r++)
        {
            for(unsigned long c = 0; c < new_num_cols; c++)
            {
                result.get(r, c) = get(r, c) + m.get(r, c);
            }
        }
        return result;
    }

    /**
     * @brief Overloading the - operator.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the matrix to subtract from this matrix.
     * @return the matrix object which is the subtraction of the two matrices.
     * */
    Matrix operator-(const Matrix &m) const
    {
        unsigned long new_num_cols = findMinCols(this, m);
        unsigned long new_num_rows = findMinRows(this, m);
        Matrix result(new_num_rows, new_num_cols);
        for(unsigned long r = 0; r < new_num_rows; r++)
        {
            for(unsigned long c = 0; c < new_num_cols; c++)
            {
                result.get(r, c) = get(r, c) - m.get(r, c);
            }
        }
        return result;
    }

    /**
     * @brief Overloading the += operator so that the new object is written to the current object.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the Matrix object to add to this matrix.
     */
    Matrix &operator+=(const Matrix &m)
    {
        unsigned long new_num_cols = findMinCols(this, m);
        unsigned long new_num_rows = findMinRows(this, m);
        for(unsigned long r = 0; r < new_num_rows; r++)
        {
            for(unsigned long c = 0; c < new_num_cols; c++)
            {
                get(r, c) += m.get(r, c);
            }
        }
        return *this;
    }

    /**
     * @brief Overloading the -= operator so that the new object is written to the current object.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the Matrix object to subtract from this matrix.
     */
    Matrix &operator-=(const Matrix &m)
    {
        unsigned long new_num_cols = findMinCols(this, m);
        unsigned long new_num_rows = findMinRows(this, m);
        for(unsigned long r = 0; r < new_num_rows; r++)
        {
            for(unsigned long c = 0; c < new_num_cols; c++)
            {
                matrix.get(r, c) -= m.get(r, c);
            }
        }
        return *this;
    }

    /**
     * @brief Overloading the * operator for scaling.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param s the constant to scale the matrix by.
     * @return the scaled matrix.
     */
    Matrix operator*(const double s) const
    {
        Matrix result(num_rows, num_cols);
        for(unsigned long r = 0; r < num_rows; r++)
        {
            for(unsigned long c = 0; c < num_cols; c++)
            {
                result.get(r, c) = get(r, c) * s;
            }
        }
        return result;
    }

    /**
     * @brief Overloading the * operator for matrix multiplication.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * Multiplies each value in the matrix with its corresponding value in the other matrix.
     * @param m the matrix to multiply with
     * @return the product of each ith,jth value of the matrix.
     */
    Matrix operator*(Matrix &m) const
    {
        unsigned long new_num_cols = findMinCols(this, m);
        unsigned long new_num_rows = findMinRows(this, m);

        Matrix result(num_rows, m.num_cols);
        for(unsigned long r = 0; r < new_num_rows; r++)
        {
            for(unsigned long c = 0; c < new_num_cols; c++)
            {
                result.get(r, c) = get(r, c) * m.get(r, c);
            }
        }
        return result;
    }

    /**
     * @brief Overloading the *= operator so that the new object is written to the current object.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the Matrix object to add to this matrix.
     */
    Matrix &operator*=(const double s)
    {
        for(unsigned long r = 0; r < num_rows; r++)
        {
            for(unsigned long c = 0; c < num_cols; c++)
            {
                get(r, c) *= s;
            }
        }
        return *this;
    }

    /**
     * @brief Overloading the *= operator so that the new object is written to the current object.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the Matrix object to add to this matrix.
     */
    Matrix &operator*=(const Matrix &m)
    {
        unsigned long new_num_cols = findMinCols(this, m);
        unsigned long new_num_rows = findMinRows(this, m);
        for(unsigned long r = 0; r < new_num_rows; r++)
        {
            for(unsigned long c = 0; c < new_num_cols; c++)
            {
                get(r, c) *= m.get(r, c);
            }
        }
        return *this;
    }

    /**
     * @brief Overloading the / operator for scaling.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param s the constant to scale the matrix by.
     * @return the scaled matrix.
     */
    Matrix operator/(const double s) const
    {
        Matrix result(num_rows, num_cols);
        for(unsigned long r = 0; r < num_rows; r++)
        {
            for(unsigned long c = 0; c < num_cols; c++)
            {
                result.get(r, c) = get(r, c) / s;
            }
        }
        return result;
    }

    /**
     * @brief Overloading the /= operator so that the new object is written to the current object.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the Matrix object to add to this matrix.
     */
    Matrix &operator/=(const double s)
    {
        for(unsigned long r = 0; r < num_rows; r++)
        {
            for(unsigned long c = 0; c < num_cols; c++)
            {
                get(r, c) /= s;
            }
        }
        return *this;
    }

    /**
     * @brief Overloading the /= operator so that the new object is written to the current object.
     * @note If matrices are of different sizes, the operation is performed on the 0 to minimum values of each
     *       dimension.
     * @param m the Matrix object to add to this matrix.
     */
    Matrix &operator/=(const Matrix &m)
    {
        unsigned long new_num_cols = findMinCols(this, m);
        unsigned long new_num_rows = findMinRows(this, m);
        for(unsigned long r = 0; r < new_num_rows; r++)
        {
            for(unsigned long c = 0; c < new_num_cols; c++)
            {
                get(r, c) /= m.get(r, c);
            }
        }
        return *this;
    }

    /**
     * @brief Writes the object to the output stream.
     * @note This is done slightly inefficiently to preserve the output taking the correct form.
     * @param os the output stream to write to
     * @param m the object to write out
     * @return the output stream
     */
    friend ostream &writeOut(ostream &os, const Matrix &m)
    {
        for(unsigned long r = 0; r < m.num_rows; r++)
        {
            for(unsigned long c = 0; c < m.num_cols; c++)
            {
                os << m.getCopy(r, c) << ",";
            }
            os << "\n";
        }
        return os;
    }

    /**
     * @brief Reads in from the input stream.
     * @param is the input stream to read from
     * @param m the object to read into
     * @return
     */
    friend istream &readIn(istream &is, Matrix &m)
    {
        char delim;
        for(unsigned long r = 0; r < m.num_rows; r++)
        {
            for(unsigned long c = 0; c < m.num_cols; c++)
            {
                is >> m.get(r, c);
                is >> delim;
            }
        }
        return is;
    }

    /**
     * @brief Overloading the << operator for outputting to an output stream.
     * This can be used for writing to console or storing to file.
     * @param os the output stream.
     * @param m the matrix to output.
     * @return the output stream.
     */
    friend ostream &operator<<(ostream &os, const Matrix &m)
    {
        return writeOut(os, m);
    }

    /**
     * @brief Overloading the >> operator for inputting from an input stream.
     * This can be used for writing to console or storing to file.
     * @param is the input stream.
     * @param m the matrix to input to.
     * @return the input stream.
     */
    friend istream &operator>>(istream &is, Matrix &m)
    {
        return readIn(is, m);
    }

    /**
     * @brief Sets the value at the specified indices, including handling type conversion from char to the template
     * class.
     * @param row the row index.
     * @param col the column index.
     * @param value the value to set
     */
    void setValue(const unsigned long &row, const unsigned long &col, const char* value)
    {
        matrix[index(row, col)] = static_cast<T>(*value);
    }

    /**
     * @brief Sets the value at the specified indices, including handling type conversion from char to the template
     * class.
     * @param row the row index.
     * @param col the column index.
     * @param value the value to set
     */
    void setValue(const unsigned long &row, const unsigned long &col, const T &value)
    {
        matrix[index(row, col)] = value;
    }

};

/**
 * @brief Find the minimum columns of the two objects.
 * @tparam T The type of the Matrix class
 * @param matrix1 the first matrix
 * @param matrix2 the second matrix
 * @return the minimum number of columns between the two matrices
 */
template<typename T>
const unsigned long findMinCols(const Matrix<T> &matrix1, const Matrix<T> &matrix2)
{
    if(matrix1.getCols() < matrix2.getCols())
    {
        return matrix1.getCols();
    }
    return matrix2.getCols();
}

/**
 * @brief Find the minimum rows of the two objects.
 * @tparam T The type of the Matrix class
 * @param matrix1 the first matrix
 * @param matrix2 the second matrix
 * @return the minimum number of rows between the two matrices
 */
template<typename T>
const unsigned long findMinRows(const Matrix<T> &matrix1, const Matrix<T> &matrix2)
{
    if(matrix1.getRows() < matrix2.getRows())
    {
        return matrix1.getRows();
    }
    return matrix2.getRows();
}

#endif // MATRIX
