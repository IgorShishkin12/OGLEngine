#pragma once
#include <vector>
#include <iostream>
class MatrixF
{
private:
    std::vector<std::vector<float>> vec;
    unsigned long m_widthO, m_heightO;

public:
    const unsigned long& m_width = m_widthO, m_height = m_heightO;
    /*
    матрицы
    доступные типы:
    обычная
    единичная
    доступные операции:
    сложение,умножение((a*b)*(b*c))=(a*c),транспозиция
    */
    MatrixF(unsigned long height, unsigned long width) : m_widthO{ width }, m_heightO{ height } //обычная матрица
    {
        if (width == 0 || height == 0)
        {
            std::cerr << "matrix has zero dimentions";
        }

        vec.resize(height);
        for (unsigned long i = 0; i < height; ++i)
        {
            vec[i].resize(width);
        };
    }
    MatrixF(unsigned long side = 1) : MatrixF(side, side) //единичная матрица
    {
        for (unsigned long i = 0; i < side; ++i)
        {
            vec[i][i] = 1;
        }
    }
    MatrixF* change(unsigned long heightArr, unsigned long widthArr, float* arr, unsigned long height = 0, unsigned long width = 0)
    {
        if (heightArr + height > m_height || widthArr + width > m_width)
        {
            std::cerr << "dimentions err in change";
        }
        for (unsigned long i = 0; i < widthArr * heightArr; ++i)
        {
            vec[height + i / heightArr][width + i % widthArr] = arr[i];
        }
        return this;
    }

    MatrixF transpose()
    {
        MatrixF ans{ m_width, m_height };
        for (unsigned long i = 0; i < m_height; ++i)
        {
            for (unsigned long j = 0; j < m_width; ++j)
            {
                ans.vec[j][i] = vec[i][j];
            }
        }
        return ans;
    }
    friend MatrixF operator+(const MatrixF& m1, const MatrixF& m2);
    friend MatrixF operator*(const MatrixF& m1, const MatrixF& m2);

    float& get(unsigned long h, unsigned long w)
    {
        return vec[h - 1][w - 1];
    }
    float& getIF(unsigned long h, unsigned long w) //get in "for"
    {
        return (vec[h][w]);
    }

    MatrixF& operator=(const MatrixF& m1)
    {
        unsigned long sizeh = m1.m_height;
        unsigned long sizew = m1.m_width;
        this->m_heightO = sizeh;
        this->m_widthO = sizew;
        for (unsigned long i = 0; i < sizeh; ++i)
        {
            for (unsigned long j = 0; j < sizew; ++j)
            {
                this->vec[i][j] = m1.vec[i][j];
            }
        }
    }
    // MatrixF& operator=(const MatrixF m1)
    // {
    //     auto m2 = m1;
    //     return m2;
    // }
};

MatrixF operator+(const MatrixF& m1, const MatrixF& m2)
{
    if (m1.m_width != m2.m_width || m1.m_height != m2.m_height)
    {
        std::cerr << "can't sum matrix with different dimentions";
    }

    MatrixF ans{ m1.m_height, m1.m_width };
    for (unsigned long i = 0; i < m1.m_height; ++i)
    {
        for (unsigned long j = 0; j < m1.m_width; ++j)
        {
            ans.vec[i][j] = m1.vec[i][j] + m2.vec[i][j];
        }
    }
    return ans;
}

MatrixF operator*(const MatrixF& m1, const MatrixF& m2)
{
    if (m1.m_width != m2.m_height)
    {
        std::cerr << "can't multiplex matrix with different dimentions";
    }
    float ans1 = 0;
    MatrixF ans(m1.m_height, m2.m_width);
    for (unsigned long i = 0; i < m1.m_height; ++i)
    {
        for (unsigned long j = 0; j < m2.m_width; ++j)
        {
            ans1 = 0;

            for (unsigned long r = 0; r < m1.m_width; ++r)
            {
                ans1 += m1.vec[i][r] * m2.vec[r][j];
            }
            ans.vec[i][j] = ans1;
        }
    }
    return ans;
}
// MatrixF operator*(const MatrixF m1,const MatrixF m2)
// {
//     return (m1*m2);
// }
// MatrixF operator+(const MatrixF m1,const MatrixF m2)
// {
//     return (m1+m2);
// }

/***********************************************************вектора**********************************/

/*
вектора
типы:
h-
v-
операции:
нормализация,умножение на матрицу
*/
class VectorF
{
    MatrixF mx;
    //bool flagIsChanged = 1;
    unsigned long sizeO;
    double len=0;
    void recountLen()
    {
        //if(flagIsChanged)
        //{
        double sumsq = 0;
        for (unsigned long i = 0; i < len; ++i)
        {
            sumsq += (this->getIF(i)) * (this->getIF(i));
        }
        len = sqrt(sumsq);
        //flagIsChanged = 0;
        //}
    }
    float& getIF(unsigned long num)
    {
        return mx.getIF((type == 'v') ? num : 1, (type == 'v') ? 1 : num);
    }

public:
    const unsigned long& size = sizeO;
    const char type;
    VectorF(unsigned long size1, char type1 = 'v') : sizeO{ size1 }, type{ type1 }
    {
        if (type1 == 'v')
        {
            mx = MatrixF{ size, 1 };
        }
        else if (type == 'h')
        {
            mx = MatrixF{ 1, size };
        }
    }
    float& get(unsigned long num)
    {
        return mx.get((type == 'v') ? num : 1, (type == 'v') ? 1 : num);
    }
    void normalize()
    {
        recountLen();
        for (unsigned long i = 0; i < len; ++i)
        {
            this->getIF(i) /= len;
        }
    }

    friend MatrixF operator*(const VectorF& v1, const MatrixF& m1);
    friend MatrixF operator*(const MatrixF& m1, const VectorF& v1);
};
MatrixF operator*(const VectorF& v1, const MatrixF& m1)
{
    return v1.mx * m1;
}
MatrixF operator*(const MatrixF& m1, const VectorF& v1)
{
    return m1 * v1.mx;
}

/**********************************************************разные виды матриц**************************/

MatrixF createTurnMx(unsigned long dimensions, unsigned long rDim1, unsigned long rDim2, float angle)
{
    MatrixF mx{ dimensions };
    mx.get(rDim1, rDim1) = cos(angle);
    mx.get(rDim2, rDim2) = cos(angle);
    mx.get(rDim1, rDim2) = -sin(angle);
    mx.get(rDim2, rDim1) = sin(angle);
    return mx;
}

MatrixF& changeRMx(MatrixF& mx, unsigned long rDim1, unsigned long rDim2, float angle) //порядок важен!!! вращение против часовой стрелки если направление оси на нас
{
    mx.get(rDim1, rDim1) = cos(angle);
    mx.get(rDim2, rDim2) = cos(angle);
    mx.get(rDim1, rDim2) = -sin(angle);
    mx.get(rDim2, rDim1) = sin(angle);
    return mx;
}

MatrixF changeRMx3(char axis, float angle)
{
    MatrixF mx{ 3 };
    switch (axis)
    {
    case 'x':
        changeRMx(mx, 2, 3, angle);
        break;

    case 'y':
        changeRMx(mx, 3, 1, angle);
        break;

    case 'z':
        changeRMx(mx, 1, 2, angle);
        break;
    default:
        std::cerr << "axis err";
        break;
    }
    return mx;
}
