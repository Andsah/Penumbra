/*
    Wrapper for a buffer, represents it as a 2D row and col array.
    Taken from Etay Meiri on GitHub 
    https://github.com/emeiri/ogldev/blob/master/Include/ogldev_array_2d.h
*/

#ifndef  ARRAY_2D_H
#define ARRAY_2D_H

#include <stdlib.h>
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif
// Template to represent a buffer in memory as a 2D array for a given datatype
template<typename Type>
class Array2D {
 public:

    // Create a new 2D array object with the desired number of columns and rows
    Array2D(int Cols, int Rows) { // send in (width-1)*6, height-1 for indexArray to work?
        InitArray2D(Cols, Rows);
    }

    // The initiator, allocates the appropriate amount of memory
    void InitArray2D(int Cols, int Rows) {
        m_cols = Cols;
        m_rows = Rows;

        if (m_p) {
            free(m_p);
        }

        m_p = (Type*)malloc(Cols * Rows * sizeof(Type));
    }

    // Destructor, frees the memory - not needed here
    ~Array2D() {}

    // Check to see if an address is valid (within the allowed col and row numbers)
    bool isValid(int Col, int Row) const {
        return !(Col < 0 || Col >= m_cols || Row < 0 || Row >= m_rows);
    }

    // Returns a reference to the element found at col and row
    Type* GetAddr(int Col, int Row) const {
        #ifndef NDEBUG
        if (Col < 0) {
            printf("%s:%d - negative col %d\n", __FILE__, __LINE__, Col);
        }

        if (Col >= m_cols) {
            printf("%s:%d - column overflow (%d vs %d)\n", __FILE__, __LINE__, Col, m_cols);
            exit(0);
        }

        if (Row < 0) {
            printf("%s:%d - negative row %d\n", __FILE__, __LINE__, Row);
        }

        if (Row >= m_rows) {
            printf("%s:%d - row overflow (%d vs %d)\n", __FILE__, __LINE__, Row, m_rows);
            exit(0);
        }
        #endif
        size_t Index = Row * m_cols + Col;

        return &m_p[Index];
    }

    // Returns the starting address of the array
    Type* GetBaseAddr() const {
        return m_p;
    }

    // Returns the size of the array as row * col
    int GetSize() const {
        return m_rows * m_cols;
    }

    // Returns the dereferenced value located at col, row
    Type Get(int Col, int Row) const {
        return *GetAddr(Col, Row);
    }

    // 1D array version of Get(col, row)
    Type Get(int Index) const {
        #ifndef NDEBUG
        if (Index >= m_rows * m_cols) {
            printf("%s:%d - index %d is out of bounds (max size %d)\n", __FILE__, __LINE__, Index, m_rows * m_cols);
            exit(0);
        }
        #endif

        return m_p[Index];
    }

    // Sets the value in the array at position col, row
    void Set(int Col, int Row, Type Val) {
        *GetAddr(Col, Row) = Val;
    }

    // 1D version of set
    void Set(int Index, Type Val) {
        #ifndef NDEBUG
        if (Index >= m_rows * m_cols) {
            printf("%s:%d - index %d is out of bounds (max size %d)\n", __FILE__, __LINE__, Index, m_rows * m_cols);
            exit(0);
        }
        #endif

        m_p[Index] = Val;
    }


    // Assigns the minimum and maximum values contained in the array to the variables entered
    void GetMinMax(Type& Min, Type& Max)  {
        Max = Min = m_p[0];

        for (int i = 1 ; i < m_rows * m_cols ; i++) {
            if (m_p[i] < Min) {Min = m_p[i];}

            if (m_p[i] > Max) {Max = m_p[i];}
        }
    }

    // Scales the values in the array to be between the values of MinRange and MaxRange
    void Normalize(Type MinRange, Type MaxRange) {
        Type Min, Max;

        GetMinMax(Min, Max);

        if (Max <= Min) {
            return;
        }

        Type MinMaxDelta = Max - Min;
        Type MinMaxRange = MaxRange - MinRange;

        for (int i = 0 ; i < m_rows * m_cols; i++) {
            m_p[i] = ((m_p[i] - Min)/MinMaxDelta) * MinMaxRange + MinRange;
        }
    }

    // mine :)
    void transpose() {
        for (int i = 0 ; i < m_rows ; i++) {
            for (int j = 0; j < m_cols - i; j++) {

                float temp = Get(i, j).y;
                Set(i, j, vec3(i, Get(m_rows - 1 - j, m_cols - 1 - i).y, j));
                Set(m_rows - 1 - j, m_cols - 1 - i, vec3(m_rows - 1 - j, temp, m_cols - 1 - i));
            }
        }
    }

 private:
    // The start of the 2D array
    Type* m_p = NULL;

    // The number of columns in the array (x-coord)
    int m_cols = 0;

    // The number of rows in the array (z-coord)
    int m_rows = 0;
};

#endif